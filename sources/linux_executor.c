/*
 * The program executes a command in a child process, redirects its output and
 * error to pipes, and handles timeouts and signals in the parent process.
 *
 * Command-line arguments:
 *  - [--timeout|-t TIMEOUT]: Optional flag to specify a timeout in seconds.
 * Defaults to 10 seconds if not provided.
 *  - COMMAND [ARGS...]: The command to execute and its arguments. This must be
 * provided.
 *
 * The program will:
 * 1. Set up pipes for stdout and stderr.
 * 2. Fork a child process.
 * 3. In the child process, redirect stdout and stderr to the pipes and execute the command.
 * 4. In the parent process, use `poll` to monitor pipes and handle timeouts and signals.
 * 5. Terminate the child process if it does not complete within the specified timeout.
 */

#define _GNU_SOURCE  // Enable GNU-specific extensions in the standard library.

// Linux-specific includes
#include <fcntl.h>         // For file control operations: fcntl
#include <poll.h>          // For event notification: poll, struct pollfd
#include <signal.h>        // For signal handling: sigprocmask, SIGCHLD
#include <sys/signalfd.h>  // For signalfd functions: signalfd
#include <sys/timerfd.h>   // For timerfd functions: timerfd_create, timerfd_settime
#include <sys/types.h>     // For defining data types: pid_t
#include <sys/wait.h>      // For macros related to process termination: waitpid
#include <unistd.h>        // For POSIX API functions: fork, execve, pipe, close, read, write, dup2

// C Standard Library includes
#include <errno.h>   // For error reporting: errno
#include <stdint.h>  // For fixed width integers
#include <stdio.h>   // For I/O functions: printf, perror
#include <stdlib.h>  // For utility functions: exit, atoi
#include <string.h>  // For string manipulation functions: strcmp
#include <time.h>    // For time utilities

// Print usage message for --help or incorrect usage
void print_usage(char const* program_name)
{
    printf("Usage: %s [--help|-h] [--timeout|-t seconds] command [args...]\n", program_name);
    printf("  --help    | -h   Show this help message\n");
    printf("  --timeout | -t   Specify the timeout (in seconds) before killing the process\n");
    printf("  command          The command to execute\n");
}

// Function to create and configure a timerfd
int create_timer_fd(int timeout)
{
    // Create a timer file descriptor with CLOCK_MONOTONIC and non-blocking mode
    // CLOCK_MONOTONIC ensures that the timer is not affected by system clock changes
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timer_fd == -1) {
        perror("timerfd_create failed");
        exit(EXIT_FAILURE);
    }

    // Configure the timer to expire after the specified timeout
    struct itimerspec timer_spec;
    timer_spec.it_interval.tv_sec  = 0;        // No periodic timer
    timer_spec.it_interval.tv_nsec = 0;        //
    timer_spec.it_value.tv_sec     = timeout;  // Set initial expiration time
    timer_spec.it_value.tv_nsec    = 0;        //

    // Set the timer with the specified timeout value
    if (timerfd_settime(timer_fd, 0, &timer_spec, NULL) == -1) {
        perror("timerfd_settime failed");
        close(timer_fd);
        exit(EXIT_FAILURE);
    }

    return timer_fd;
}

// Function to create a signalfd for handling SIGCHLD signals
int create_signalfd(void)
{
    sigset_t sigset;

    // Initialize an empty signal set
    sigemptyset(&sigset);

    // Add SIGCHLD to the signal set to handle child process termination
    sigaddset(&sigset, SIGCHLD);

    // Block SIGCHLD signals in the current process so they can be handled via signalfd
    if (sigprocmask(SIG_BLOCK, &sigset, NULL) == -1) {
        perror("sigprocmask failed");
        exit(EXIT_FAILURE);
    }

    // Create a file descriptor for handling signals with non-blocking mode
    int signal_fd = signalfd(-1, &sigset, SFD_NONBLOCK);
    if (signal_fd == -1) {
        perror("signalfd_create failed");
        exit(EXIT_FAILURE);
    }

    return signal_fd;
}

// Function to set a file descriptor to non-blocking mode
void set_fd_nonblocking(int fd)
{
    // Get the current file descriptor flags
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL failed");
        exit(EXIT_FAILURE);
    }
    // Set the file descriptor to non-blocking mode
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL failed");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[], char* envp[])
{
    // Parse command-line arguments for the help option
    if (argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;  // Exit after printing usage information
    }

    int timeout       = 10;  // Default timeout value in seconds
    int command_index = 1;   // Index of the command in argv

    // Parse command-line arguments for the timeout option
    if (argc > 1 && (strcmp(argv[1], "--timeout") == 0 || strcmp(argv[1], "-t") == 0)) {
        // Print error if timeout value is missing
        if (argc < 3) {
            fprintf(stderr, "Error: Timeout value is required\n");
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        // Convert timeout value from string to integer
        timeout = atoi(argv[2]);
        if (timeout <= 0) {
            fprintf(stderr, "Error: Timeout must be a positive integer\n");
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        // Update command index if timeout option is present
        command_index = 3;
    }

    // Ensure a command is provided
    if (argc <= command_index) {
        fprintf(stderr, "Error: No command provided\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Create pipes for stdout and stderr
    int stdout_pipe[2];
    int stderr_pipe[2];
    if (pipe2(stdout_pipe, O_CLOEXEC) == -1 || pipe2(stderr_pipe, O_CLOEXEC) == -1) {
        perror("pipe creation failed");
        return EXIT_FAILURE;
    }

    // Set pipe read ends to non-blocking mode to handle I/O efficiently
    set_fd_nonblocking(stdout_pipe[0]);
    set_fd_nonblocking(stderr_pipe[0]);

    // Fork the process to create a child
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {       // Child process
        close(stdout_pipe[0]);  // Close unused read end of stdout pipe
        close(stderr_pipe[0]);  // Close unused read end of stderr pipe

        // Redirect stdout to stdout_pipe
        if (dup2(stdout_pipe[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout failed");
            exit(EXIT_FAILURE);
        }

        // Redirect stderr to stderr_pipe
        if (dup2(stderr_pipe[1], STDERR_FILENO) == -1) {
            perror("dup2 stderr failed");
            exit(EXIT_FAILURE);
        }

        close(stdout_pipe[1]);  // Close the original write end of stdout pipe
        close(stderr_pipe[1]);  // Close the original write end of stderr pipe

        // Execute the specified command
        execve(argv[command_index], argv + command_index, envp);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(stdout_pipe[1]);  // Close unused write end of stdout pipe
    close(stderr_pipe[1]);  // Close unused write end of stderr pipe

    // Create and set up timerfd for handling timeouts
    int timer_fd = create_timer_fd(timeout);

    // Create and set up signalfd for handling SIGCHLD
    int signal_fd = create_signalfd();

    // Set up poll file descriptors
    struct pollfd poll_fds[4];
    poll_fds[0].fd     = signal_fd;       // File descriptor for signal handling
    poll_fds[0].events = POLLIN;          // Monitor for readability
    poll_fds[1].fd     = timer_fd;        // File descriptor for timer
    poll_fds[1].events = POLLIN;          // Monitor for readability
    poll_fds[2].fd     = stdout_pipe[0];  // File descriptor for stdout pipe
    poll_fds[2].events = POLLIN;          // Monitor for readability
    poll_fds[3].fd     = stderr_pipe[0];  // File descriptor for stderr pipe
    poll_fds[3].events = POLLIN;          // Monitor for readability

    char buffer[1024];          // Buffer for reading pipe data
    int  child_status     = 0;  // Child exit status
    int  child_terminated = 0;  // Flag to track if the child process has terminated

    // Main event loop
    while (1) {
        // Wait for events on the file descriptors
        int poll_result = poll(poll_fds, 4, -1);
        if (poll_result == -1) {
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
            if (siginfo.ssi_signo == SIGCHLD) {
                // Check the status of the child process
                if (waitpid(siginfo.ssi_pid, &child_status, WNOHANG) != child_pid) {
                    perror("waitpid returns incorrect pid");
                    exit(EXIT_FAILURE);
                }
                // Mark that the child process is terminated
                child_terminated = 1;
            }
        }

        // Handle timeout event from timerfd
        if (poll_fds[1].revents & POLLIN) {
            uint64_t expirations = 0;
            if (read(timer_fd, &expirations, sizeof(expirations)) == -1) {
                perror("read from timerfd failed");
                exit(EXIT_FAILURE);
            }
            // Timeout event indicates that the specified timeout period has expired
            if (!child_terminated) {
                // Forcefully kill the child process if not yet terminated
                kill(child_pid, SIGKILL);
            }
        }

        // Handle stdout pipe events
        if (poll_fds[2].revents & POLLIN) {
            ssize_t bytes_read = 0;

            // Continue reading until no more data is available
            while ((bytes_read = read(poll_fds[2].fd, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            }

            if (bytes_read == 0) {  // EOF
                // The pipe is closed, which means the child process has finished writing
                // Mark the file descriptor as invalid to stop polling it
                poll_fds[2].fd      = -1;
                poll_fds[2].events  = 0;
                poll_fds[2].revents = 0;
            } else {  // error
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read from stdout pipe failed");
                    // Exit loop on fatal error
                    break;
                }
            }
        }

        // Handle stderr pipe events
        if (poll_fds[3].revents & POLLIN) {
            ssize_t bytes_read = 0;

            // Continue reading until no more data is available
            while ((bytes_read = read(poll_fds[3].fd, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            }

            if (bytes_read == 0) {  // EOF
                // The pipe is closed, which means the child process has finished writing
                // Mark the file descriptor as invalid to stop polling it
                poll_fds[3].fd      = -1;
                poll_fds[3].events  = 0;
                poll_fds[3].revents = 0;
            } else {  // errors
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read from stderr pipe failed");
                    // Exit loop on fatal error
                    break;
                }
            }
        }

        // Break the loop if the child has terminated and no more data to read from both pipes
        if (child_terminated && !(poll_fds[2].revents & POLLIN) && !(poll_fds[3].revents & POLLIN)) {
            break;  // Exit the loop if the child has terminated and no more data to read
        }
    }

    // Close all open file descriptors
    close(stdout_pipe[0]);  // Close read end of stdout pipe
    close(stderr_pipe[0]);  // Close read end of stderr pipe
    close(timer_fd);        // Close timer file descriptor
    close(signal_fd);       // Close signalfd file descriptor

    return EXIT_SUCCESS;  // Exit the program successfully
}
