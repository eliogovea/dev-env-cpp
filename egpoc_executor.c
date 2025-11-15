/*
 * The program executes a command in a child process, redirects its output and
 * error to pipes, and handles timeouts and signals in the parent process.
 *
 * Command-line arguments:
 *  - [--timeout|-t TIMEOUT]: Optional flag to specify a timeout in seconds.
 * Defaults to 10 seconds if not provided.
 *  - COMMAND [ARGS...]: The command to execute and its arguments. This must be
 * provided.
 */

#define _GNU_SOURCE

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void egpoc_executor_usage(char const* program_name)
{
    fprintf(stderr, "Usage: %s [--help|-h] [--timeout|-t seconds] command [args...]\n", program_name);
    fprintf(stderr, "  --help    | -h   Show this help message\n");
    fprintf(stderr, "  --timeout | -t   Specify the timeout (in seconds) before killing the process\n");
    fprintf(stderr, "  command          The command to execute\n");
}

int main(int argc, char* argv[], char* envp[])
{
    char* egpoc_executor_name = argv[0];
    argc--;
    argv++;

    int    timeout      = 10;
    char*  command_path = NULL;
    char** command_argv = NULL;

    if (argc < 1) {
        egpoc_executor_usage(egpoc_executor_name);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[0], "--help") == 0 || strcmp(argv[0], "-h") == 0) {
        egpoc_executor_usage(egpoc_executor_name);
        exit(EXIT_SUCCESS);
    }

    if (strcmp(argv[0], "--timeout") == 0 || strcmp(argv[0], "-t") == 0) {
        if (argc < 2) {
            egpoc_executor_usage(egpoc_executor_name);
            exit(EXIT_FAILURE);
        }

        argc--;
        argv++;

        char* arg = argv[0];
        char* end = NULL;

        argc--;
        argv++;

        errno   = 0;
        timeout = strtol(arg, &end, 10);

        if (errno == ERANGE || !end || *end) {
            egpoc_executor_usage(egpoc_executor_name);
            exit(EXIT_FAILURE);
        }
    }

    if (argc < 1) {
        egpoc_executor_usage(egpoc_executor_name);
        exit(EXIT_FAILURE);
    }

    command_path = argv[0];
    command_argv = argv;

    int stdout_pipe[2] = {-1, -1};
    int stderr_pipe[2] = {-1, -1};

    if (pipe2(stdout_pipe, O_CLOEXEC | O_NONBLOCK) == -1) {
        perror("stdout pipe creation failed");
        exit(EXIT_FAILURE);
    }

    if (pipe2(stderr_pipe, O_CLOEXEC | O_NONBLOCK) == -1) {
        perror("stderr pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {       // Child process
        close(stdout_pipe[0]);  // Close unused read end of stdout pipe
        close(stderr_pipe[0]);  // Close unused read end of stderr pipe

        if (dup2(stdout_pipe[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout failed");
            exit(EXIT_FAILURE);
        }

        if (dup2(stderr_pipe[1], STDERR_FILENO) == -1) {
            perror("dup2 stderr failed");
            exit(EXIT_FAILURE);
        }

        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        execvpe(command_path, command_argv, envp);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }

    close(stdout_pipe[1]);  // Close unused write end of stdout pipe
    close(stderr_pipe[1]);  // Close unused write end of stderr pipe

    // CLOCK_MONOTONIC ensures that the timer is not affected by system clock changes
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timer_fd == -1) {
        perror("timerfd_create failed");
        exit(EXIT_FAILURE);
    }

    struct itimerspec timer_spec;
    timer_spec.it_interval.tv_sec  = 0;
    timer_spec.it_interval.tv_nsec = 0;
    timer_spec.it_value.tv_sec     = timeout;
    timer_spec.it_value.tv_nsec    = 0;

    if (timerfd_settime(timer_fd, 0, &timer_spec, NULL) == -1) {
        perror("timerfd_settime failed");
        close(timer_fd);
        exit(EXIT_FAILURE);
    }

    sigset_t sigset;
    sigemptyset(&sigset);

    // Add SIGCHLD to the signal set to handle child process termination
    sigaddset(&sigset, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &sigset, NULL) == -1) {
        perror("sigprocmask failed");
        exit(EXIT_FAILURE);
    }

    int signal_fd = signalfd(-1, &sigset, SFD_NONBLOCK);
    if (signal_fd == -1) {
        perror("signalfd_create failed");
        exit(EXIT_FAILURE);
    }

    struct pollfd poll_fds[4];
    poll_fds[0].fd     = signal_fd;       // File descriptor for signal handling
    poll_fds[0].events = POLLIN;          // Monitor for readability
    poll_fds[1].fd     = timer_fd;        // File descriptor for timer
    poll_fds[1].events = POLLIN;          // Monitor for readability
    poll_fds[2].fd     = stdout_pipe[0];  // File descriptor for stdout pipe
    poll_fds[2].events = POLLIN;          // Monitor for readability
    poll_fds[3].fd     = stderr_pipe[0];  // File descriptor for stderr pipe
    poll_fds[3].events = POLLIN;          // Monitor for readability

    char buffer[1024];
    int  child_status     = 0;
    int  child_timeout    = 0;
    int  child_terminated = 0;

    while (1) {
        if (poll(poll_fds, 4, -1) == -1) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }

        // Handle SIGCHLD event from signalfd
        if (poll_fds[0].revents & POLLIN) {
            struct signalfd_siginfo siginfo;
            if (read(signal_fd, &siginfo, sizeof(siginfo)) != sizeof(siginfo)) {
                perror("read from signalfd failed");
                exit(EXIT_FAILURE);
            }
            // SIGCHLD is received when the child process terminates
            if (siginfo.ssi_signo == SIGCHLD && siginfo.ssi_pid == child_pid) {
                child_terminated = 1;
            }
        }

        // Handle timeout event from timerfd
        if (poll_fds[1].revents & POLLIN) {
            uint64_t expirations = 0;
            if (read(timer_fd, &expirations, sizeof(expirations)) == -1) {
                perror("timerfd: failed to read expiration counter");
                exit(EXIT_FAILURE);
            }

            if (kill(child_pid, SIGKILL) == -1) {
                if (errno != ESRCH) {  // possible race condition: timeout vs child exit
                    perror("kill: failed to send SIGKILL to child");
                    exit(EXIT_FAILURE);
                }
            }

            child_timeout    = 1;
            child_terminated = 1;
        }

        // Handle stdout pipe events
        if (poll_fds[2].revents & POLLIN) {
            ssize_t bytes_read = 0;

            while ((bytes_read = read(poll_fds[2].fd, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            }

            if (bytes_read == 0) {  // EOF
                poll_fds[2].fd      = -1;
                poll_fds[2].events  = 0;
                poll_fds[2].revents = 0;
            } else {  // error
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read from stdout pipe failed");
                    break;
                }
            }
        }

        // Handle stderr pipe events
        if (poll_fds[3].revents & POLLIN) {
            ssize_t bytes_read = 0;

            while ((bytes_read = read(poll_fds[3].fd, buffer, sizeof(buffer))) > 0) {
                write(STDERR_FILENO, buffer, bytes_read);
            }

            if (bytes_read == 0) {  // EOF
                poll_fds[3].fd      = -1;
                poll_fds[3].events  = 0;
                poll_fds[3].revents = 0;
            } else {  // error
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read from stderr pipe failed");
                    break;
                }
            }
        }

        int finished = child_terminated                                            // child
                    && (poll_fds[2].fd == -1 || !(poll_fds[2].revents & POLLIN))   // child stdout
                    && (poll_fds[3].fd == -1 || !(poll_fds[2].revents & POLLIN));  // child stderr

        if (finished) {
            break;
        }
    }

    close(stdout_pipe[0]);
    close(stderr_pipe[0]);
    close(timer_fd);
    close(signal_fd);

    if (waitpid(child_pid, &child_status, 0) == -1) {
        perror("waitpid: failed while checking child status");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
