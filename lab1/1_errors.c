#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void errors(const char *filename) {
    if (!filename) {
        fprintf(stderr, "> Error: invalid arguments\n");
    }

    fprintf(stderr, "###------OPEN------###\n\n");

    int fd = open(filename, O_RDONLY);
    int err = errno;

    /* errno only */
    fprintf(stderr, "/--------ERRNO--------/\n");
    fprintf(stderr, "> Syscall open() returned: %d, errno value: %d\n", fd, err);
    fprintf(stderr, "/---------------------/\n\n");

    /* sys_errlist (deprecated) */
    #if SYS_ERRLIST
    fprintf(stderr, "/--------SYS_ERRLIST--------/\n");
    fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, sys_errlist[err]);
    fprintf(stderr, "/---------------------------/\n\n");
    #endif

    /* strerror */
    fprintf(stderr, "/--------STRERROR--------/\n");
    fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(err));
    fprintf(stderr, "/------------------------/\n\n");

    /* perror */
    fprintf(stderr, "/--------PERROR--------/\n");
    fprintf(stderr, "> Syscall open() returned: %d", fd);
    errno = err;
    perror(", error");
    fprintf(stderr, "/----------------------/\n\n");

    fprintf(stderr, "\n\n###------CLOSE------###\n\n");

    fd = close(fd);
    err = errno;

    /* errno only */
    fprintf(stderr, "/--------ERRNO--------/\n");
    fprintf(stderr, "> Syscall close() returned: %d, errno value: %d\n", fd, err);
    fprintf(stderr, "/---------------------/\n\n");

    /* sys_errlist (deprecated) */
    #if SYS_ERRLIST
    fprintf(stderr, "/--------SYS_ERRLIST--------/\n");
    fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, sys_errlist[err]);
    fprintf(stderr, "/---------------------------/\n\n");
    #endif

    /* strerror */
    fprintf(stderr, "/--------STRERROR--------/\n");
    fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(err));
    fprintf(stderr, "/------------------------/\n\n");

    /* perror */
    fprintf(stderr, "/--------PERROR--------/\n");
    fprintf(stderr, "> Syscall close() returned: %d", fd);
    errno = err;
    perror(", error");
    fprintf(stderr, "/----------------------/\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stdout, "Usage: %s [filename]\n", argv[0]);
    }

    errors(argv[1]);

    return 0;
}