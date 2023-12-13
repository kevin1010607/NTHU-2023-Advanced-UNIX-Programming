#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>

#define err_quit(fmt, ...)                        \
    do                                            \
    {                                             \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
        exit(1);                                  \
    } while (0)

void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl = {};
    struct sigaction sa = {};

    /*
     * Clear file creation mask.
     */
    umask(0);

    /*
     * Get maximum number of file descriptors.
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("%s: can’t get file limit", cmd);

    /*
     * Become a session leader to lose controlling TTY.
     */
    if ((pid = fork()) < 0)
        err_quit("%s: can’t fork", cmd);
    else if (pid != 0) /* parent */
        exit(0);
    setsid();

    /*
     * Ensure future opens won’t allocate controlling TTYs.
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: can’t ignore SIGHUP", cmd);
    if ((pid = fork()) < 0)
        err_quit("%s: can’t fork", cmd);
    else if (pid != 0) /* parent */
        exit(0);
    /*
     * Change the current working directory to the root so
     * we won’t prevent file systems from being unmounted.
     */
    if (chdir("/") < 0)
        err_quit("%s: can’t change directory to /", cmd);
    /*
     * Close all open file descriptors.
     */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * Initialize the log file.
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

int main() {
    // Get current directory and set output file path
    char current_dir[100] = {}, file_path[100] = {};
    getcwd(current_dir, sizeof(current_dir));
    sprintf(file_path, "%s/assignment11.txt", current_dir);

    // Call the daemonize function correctly.
    daemonize("assignment11");

    // Create a text file and write the login name
    FILE *file = fopen(file_path, "w");
    if(file == NULL) exit(1);
    char *login_name = getlogin();
    if(login_name == NULL) exit(1);
    fprintf(file, "Login name: %s\n", login_name);
    fclose(file);

    return 0;
}
