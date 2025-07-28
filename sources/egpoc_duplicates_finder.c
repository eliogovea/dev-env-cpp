#define _GNU_SOURCE
#include <dirent.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define XXH_IMPLEMENTATION
#define XXX_INLINE_ALL
#define XXH_STATIC_LINKING_ONLY
#include <xxhash.h>

typedef struct {
    void*  base;
    size_t size;
    size_t used;
} egpoc_memory_allocator_stack_t;

typedef void (*egpoc_file_find_callback_t)(void* /* user_data*/,
                                           char const* /* file_path */,
                                           struct stat const* /* file_stat */);

typedef struct {
    char const*  path_data;
    char*        file_data;
    size_t       file_data_size;
    XXH64_hash_t file_data_hash;
} egpoc_files_data_t;

typedef struct {
    egpoc_files_data_t* files_data;
    size_t              files_size;
    size_t              files_capacity;
    size_t              _padding;
} egpoc_files_data_container_t;

typedef struct {
    size_t file_path_count;
    size_t file_path_bytes;
} egpoc_files_callback_count_user_data_t;

typedef struct {
    egpoc_memory_allocator_stack_t* allocator;
    egpoc_files_data_container_t*   container;
} egpoc_files_callback_store_user_data_t;

inline static size_t egpoc_memory_align_up(size_t alignment, size_t size)
{
    assert(alignment > 0);
    assert((alignment & (alignment - 1)) == 0);
    return (size + alignment - 1) & ~(alignment - 1);
}

inline static int egpoc_memory_allocator_stack_create(egpoc_memory_allocator_stack_t* allocator, size_t size)
{
    if (allocator == NULL) {
        errno = EINVAL;
        return -1;
    }

    allocator->base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
    allocator->size = size;
    allocator->used = 0U;

    if (allocator->base == MAP_FAILED) {
        return -1;
    }

    return 0;
}

inline static int egpoc_memory_allocator_stack_destroy(egpoc_memory_allocator_stack_t* allocator)
{
    if (munmap(allocator->base, allocator->size) != 0) {
        return -1;
    }

    allocator->base = NULL;
    allocator->size = 0;
    allocator->used = 0;

    return 0;
}

inline static void* egpoc_memory_allocator_stack_allocate(egpoc_memory_allocator_stack_t* allocator,
                                                          size_t                          alignment,
                                                          size_t                          size)
{
    size_t used_prev = egpoc_memory_align_up(alignment, allocator->used);
    size_t used_next = egpoc_memory_align_up(alignment, allocator->used) + size;

    if (size == 0) {
        errno = EINVAL;
        return NULL;
    }

    if (used_next > allocator->size) {
        errno = ENOMEM;
        return NULL;
    }

    allocator->used = used_next;

    return (char*)allocator->base + used_prev;
}

static void egpoc_directory_walk(char*                      path_data,  //
                                 int                        path_size,
                                 int                        path_capacity,
                                 void*                      user_data,
                                 egpoc_file_find_callback_t user_callback)
{
    DIR* dir = opendir(path_data);

    if (dir == NULL) {
        fprintf(stderr, "error: opendir %s failed (errno=%d): %s\n", path_data, errno, strerror(errno));
        abort();
    }

    struct dirent* entry = NULL;

    while ((entry = readdir(dir)) != NULL) {
        int entry_name_size = strlen(entry->d_name);

        if (strcmp(entry->d_name, ".") == 0) {
            continue;
        }

        if (strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (path_capacity <= path_size + 1 + entry_name_size) {
            fprintf(stderr, "error: path size too big\n");
            abort();
        }

        int path_size_new = path_size;

        path_data[path_size_new] = '/';
        path_size_new++;

        memcpy(path_data + path_size_new, entry->d_name, entry_name_size);
        path_size_new += entry_name_size;

        path_data[path_size_new] = '\0';

        struct stat entry_stat;
        if (stat(path_data, &entry_stat) != 0) {
            fprintf(stderr, "error: stat %s failed (errno=%d): %s\n", path_data, errno, strerror(errno));
            continue;
        }

        user_callback(user_data, path_data, &entry_stat);

        if (S_ISDIR(entry_stat.st_mode)) {
            egpoc_directory_walk(path_data, path_size_new, path_capacity, user_data, user_callback);
        }
    }

    path_data[path_size] = '\0';

    if (closedir(dir) != 0) {
        fprintf(stderr, "error: closedir %s failed (errno=%d): %s\n", path_data, errno, strerror(errno));
        abort();
    }
}

static inline void egpoc_directory_walk_callback_count(void*              user_data,
                                                       char const*        file_path,
                                                       struct stat const* file_stat)
{
    if (S_ISREG(file_stat->st_mode)) {
        ((egpoc_files_callback_count_user_data_t*)user_data)->file_path_count += 1;
        ((egpoc_files_callback_count_user_data_t*)user_data)->file_path_bytes += 1 + strlen(file_path);
    }
}

static inline void egpoc_directory_walk_callback_store(void*              user_data,
                                                       char const*        file_path,
                                                       struct stat const* file_stat)
{
    egpoc_files_callback_store_user_data_t* user_data_ = (egpoc_files_callback_store_user_data_t*)user_data;

    if (S_ISREG(file_stat->st_mode)) {
        size_t file_path_size = strlen(file_path);
        char*  file_path_copy
            = (char*)egpoc_memory_allocator_stack_allocate(user_data_->allocator, sizeof(char), file_path_size + 1);

        if (file_path_copy == NULL) {
            fprintf(
                stderr, "error: egpoc_memory_allocator_stack_allocate failed (errno=%d): %s\n", errno, strerror(errno));
            abort();
        }

        memcpy(file_path_copy, file_path, file_path_size);
        file_path_copy[file_path_size] = '\0';

        user_data_->container->files_data[user_data_->container->files_size].path_data      = file_path_copy;
        user_data_->container->files_data[user_data_->container->files_size].file_data      = NULL;
        user_data_->container->files_data[user_data_->container->files_size].file_data_size = file_stat->st_size;
        user_data_->container->files_data[user_data_->container->files_size].file_data_hash = 0U;
        user_data_->container->files_size++;
    }
}

static inline int egpoc_files_data_size_compare(void const* lhs, void const* rhs)
{
    int64_t lhs_data_size = (int64_t)((egpoc_files_data_t*)lhs)->file_data_size;
    int64_t rhs_data_size = (int64_t)((egpoc_files_data_t*)rhs)->file_data_size;

    if (lhs_data_size < rhs_data_size) {
        return -1;
    }

    if (lhs_data_size > rhs_data_size) {
        return +1;
    }

    return 0;
}

static inline int egpoc_files_data_hash_compare(void const* lhs, void const* rhs)
{
    size_t lhs_data_hash = ((egpoc_files_data_t*)lhs)->file_data_hash;
    size_t rhs_data_hash = ((egpoc_files_data_t*)rhs)->file_data_hash;

    if (lhs_data_hash < rhs_data_hash) {
        return -1;
    }

    if (lhs_data_hash > rhs_data_hash) {
        return +1;
    }

    return 0;
}

#ifdef EGPOC_DUPLICATES_FINDER_USE_HASH_ONLY

static inline void egpoc_find_duplicates_last_step(egpoc_files_data_t* range_begin, egpoc_files_data_t* range_end)
{
    // NOTE: assume that files with the same hash are equal

    for (egpoc_files_data_t* range_iterator = range_begin; range_iterator != range_end; ++range_iterator) {
        fprintf(stdout, "%s (%zuB)\n", range_iterator->path_data, range_iterator->file_data_size);
    }

    fprintf(stdout, "\n");
}

#else

static inline int egpoc_files_data_compare(void const* lhs, void const* rhs)
{
    return memcmp(((egpoc_files_data_t*)lhs)->file_data,
                  ((egpoc_files_data_t*)rhs)->file_data,
                  ((egpoc_files_data_t*)lhs)->file_data_size);
}

static inline void egpoc_find_duplicates_last_step(egpoc_files_data_t* range_begin, egpoc_files_data_t* range_end)
{
    for (egpoc_files_data_t* range_iterator = range_begin; range_iterator != range_end; ++range_iterator) {
        int file_fd = open(range_iterator->path_data, O_RDONLY);

        if (file_fd == -1) {
            fprintf(
                stderr, "error: open %s failed (errno=%d): %s\n", range_iterator->path_data, errno, strerror(errno));
            abort();
        }

        // FIXME: this if the number of files on this range is bigger than /proc/sys/vm/max_map_count
        range_iterator->file_data = (char*)mmap(NULL, range_begin->file_data_size, PROT_READ, MAP_PRIVATE, file_fd, 0);

        if (close(file_fd) != 0) {
            fprintf(stderr,
                    "error: close %s (fd=%d) failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    file_fd,
                    errno,
                    strerror(errno));
            abort();
        }

        if (range_iterator->file_data == MAP_FAILED) {
            fprintf(stderr,
                    "error: mmap %s (fd=%d) failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    file_fd,
                    errno,
                    strerror(errno));
            abort();
        }
    }

    qsort(range_begin, range_end - range_begin, sizeof(*range_begin), &egpoc_files_data_compare);

    egpoc_files_data_t* equal_range_begin = range_begin;
    egpoc_files_data_t* equal_range_end   = range_begin;

    while (equal_range_begin != range_end) {
        ++equal_range_end;

        while (equal_range_end != range_end && egpoc_files_data_compare(equal_range_begin, equal_range_end) == 0) {
            ++equal_range_end;
        }

        size_t equal_range_size = equal_range_end - equal_range_begin;

        if (equal_range_size > 1) {
            for (egpoc_files_data_t* equal_range_iterator = equal_range_begin; equal_range_iterator != equal_range_end;
                 equal_range_iterator++) {
                fprintf(stdout, "%s\n", equal_range_iterator->path_data);
            }

            fprintf(stdout, "\n");
        }

        equal_range_begin = equal_range_end;
    }

    for (egpoc_files_data_t* range_iterator = range_begin; range_iterator != range_end; ++range_iterator) {
        if (munmap(range_iterator->file_data, range_iterator->file_data_size) != 0) {
            fprintf(stderr,  //
                    "error: munmap %s failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    errno,
                    strerror(errno));
            abort();
        }

        range_iterator->file_data = NULL;
    }
}

#endif  // EGPOC_DUPLICATES_FINDER_USE_HASH_ONLY

void egpoc_find_duplicates(XXH3_state_t*       file_hash_state,
                           size_t              file_hash_bytes,
                           egpoc_files_data_t* range_begin,
                           egpoc_files_data_t* range_end)
{
    if (range_end - range_begin <= 1) {
        return;
    }

    if (range_begin->file_data_size == 0) {
        fprintf(stderr, "error: range_begin->file_data_size == 0\n");
        abort();
    }

    if (file_hash_bytes > range_begin->file_data_size) {
        fprintf(stderr, "error: file_hash_bytes > range_begin->file_data_size\n");
        abort();
    }

    if (file_hash_bytes == range_begin->file_data_size) {
        egpoc_find_duplicates_last_step(range_begin, range_end);
        return;
    }

    file_hash_bytes = 1024 * file_hash_bytes;

    if (file_hash_bytes < 4096) {
        file_hash_bytes = 4096;
    }

    if (file_hash_bytes > range_begin->file_data_size) {
        file_hash_bytes = range_begin->file_data_size;
    }

    for (egpoc_files_data_t* range_iterator = range_begin; range_iterator != range_end; ++range_iterator) {
        int file_fd = open(range_iterator->path_data, O_RDONLY);

        if (file_fd == -1) {
            fprintf(stderr,  //
                    "error: open %s failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    errno,
                    strerror(errno));
            abort();
        }

        if (flock(file_fd, LOCK_EX) == -1) {
            fprintf(stderr,
                    "error: flock %s (fd=%d) failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    file_fd,
                    errno,
                    strerror(errno));
        }

        range_iterator->file_data
            = (char*)mmap(NULL, range_iterator->file_data_size, PROT_READ, MAP_PRIVATE, file_fd, 0);

        if (range_iterator->file_data == MAP_FAILED) {
            fprintf(stderr,
                    "error: mmap %s (fd=%d) failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    file_fd,
                    errno,
                    strerror(errno));
            abort();
        }

        XXH3_64bits_reset(file_hash_state);
        XXH3_64bits_update(file_hash_state, range_iterator->file_data, file_hash_bytes);
        range_iterator->file_data_hash = XXH3_64bits_digest(file_hash_state);

        if (munmap(range_iterator->file_data, range_begin->file_data_size) != 0) {
            fprintf(stderr,  //
                    "error: munmap %s failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    errno,
                    strerror(errno));
            abort();
        }

        if (flock(file_fd, LOCK_UN)) {
            fprintf(stderr,
                    "error: unlock %s (fd=%d) failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    file_fd,
                    errno,
                    strerror(errno));
        }

        if (close(file_fd) != 0) {
            fprintf(stderr,
                    "error: close %s (fd=%d) failed (errno=%d): %s\n",
                    range_iterator->path_data,
                    file_fd,
                    errno,
                    strerror(errno));
            abort();
        }

        range_iterator->file_data = NULL;
    }

    qsort(range_begin, range_end - range_begin, sizeof(*range_begin), &egpoc_files_data_hash_compare);

    egpoc_files_data_t* equal_range_begin = range_begin;
    egpoc_files_data_t* equal_range_end   = range_begin;

    while (equal_range_begin != range_end) {
        ++equal_range_end;

        while (equal_range_end != range_end && equal_range_begin->file_data_hash == equal_range_end->file_data_hash) {
            ++equal_range_end;
        }

        egpoc_find_duplicates(file_hash_state, file_hash_bytes, equal_range_begin, equal_range_end);

        equal_range_begin = equal_range_end;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "error: unexpected arguments\n");
        fprintf(stderr, "usage: %s DIRECTORY_PATH\n", argv[0]);
        return EXIT_FAILURE;
    }

    char   path_data[PATH_MAX] = {0};
    size_t path_size           = strlen(argv[1]);

    if (sizeof(path_data) <= path_size) {
        fprintf(stderr, "error: path size (%zu) exceeds the allowed limit (%zu)\n", path_size, sizeof(path_data));
        return EXIT_FAILURE;
    }

    memset(path_data, 0, sizeof(path_data));
    memcpy(path_data, argv[1], path_size);

    egpoc_files_callback_count_user_data_t user_data_count = {
        .file_path_count = 0U,
        .file_path_bytes = 0U,
    };

    egpoc_directory_walk(path_data,  //
                         path_size,
                         sizeof(path_data),
                         &user_data_count,
                         &egpoc_directory_walk_callback_count);

    size_t allocator_capacity = 0U;
    allocator_capacity += egpoc_memory_align_up(8, sizeof(egpoc_files_data_t)) * user_data_count.file_path_count;
    allocator_capacity += user_data_count.file_path_bytes;

    egpoc_memory_allocator_stack_t allocator = {0};

    if (egpoc_memory_allocator_stack_create(&allocator, allocator_capacity) != 0) {
        perror("egpoc_memory_allocator_stack_create");
        abort();
    }

    egpoc_files_data_container_t container = {0};

    container.files_capacity = user_data_count.file_path_count;
    container.files_size     = 0U;
    container.files_data     = (egpoc_files_data_t*)egpoc_memory_allocator_stack_allocate(
        &allocator, sizeof(void*), sizeof(egpoc_files_data_t) * user_data_count.file_path_count);

    egpoc_files_callback_store_user_data_t store_user_data = {
        .allocator = &allocator,
        .container = &container,
    };

    egpoc_directory_walk(path_data,  //
                         path_size,
                         sizeof(path_data),
                         &store_user_data,
                         &egpoc_directory_walk_callback_store);

    qsort(container.files_data,  //
          container.files_size,
          sizeof(container.files_data[0]),
          &egpoc_files_data_size_compare);

    XXH3_state_t* file_hash_state = XXH3_createState();

    if (file_hash_state == NULL) {
        fprintf(stderr, "XXH3_createState failed\n");
        abort();
    }

    egpoc_files_data_t* const files_range_end = container.files_data + container.files_size;

    // iterator for ranges of files with equal size
    egpoc_files_data_t* equal_range_begin = container.files_data;
    egpoc_files_data_t* equal_range_end   = container.files_data;

    // skip files with size 0
    while (equal_range_begin != files_range_end && equal_range_begin->file_data_size == 0) {
        ++equal_range_begin;
        ++equal_range_end;
    }

    if (equal_range_begin->file_data_size == 0) {
        fprintf(stderr, "equal_range_begin->file_data_size == 0");
        abort();
    }

    // iterate over all ranges of files with equal size
    while (equal_range_begin != files_range_end) {
        ++equal_range_end;

        while (equal_range_end < files_range_end
               && equal_range_begin->file_data_size == equal_range_end->file_data_size) {
            ++equal_range_end;
        }

        egpoc_find_duplicates(file_hash_state, 0, equal_range_begin, equal_range_end);

        equal_range_begin = equal_range_end;
    }

    if (XXH3_freeState(file_hash_state) != XXH_OK) {
        fprintf(stderr, "XXH3_freeState failed\n");
        abort();
    }

    if (egpoc_memory_allocator_stack_destroy(&allocator) != 0) {
        perror("egpoc_memory_allocator_stack_destroy\n");
        abort();
    }

    return EXIT_SUCCESS;
}
