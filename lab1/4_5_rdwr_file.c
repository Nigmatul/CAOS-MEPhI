#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>




void rdwr_file(const char *filename, const char *str_mode, const char *str_whence) {
  if (!filename || !str_mode || !str_whence) {
    fprintf(stderr, "> Error: inavlid arguments\n");
  }

  long mode = strtol(str_mode, NULL, 8);
  int whence = strtol(str_whence, NULL, 3);
  mode_t old = umask(0);
  
  printf("Old umask: %d, filename: %s, mode: %s(oct) <=> %ld(dec), whence: %d\n", 
      old, filename, str_mode, mode, whence);

#if USE_CREAT
  int fd = creat(filename, mode);
  fprintf(stderr, "> Syscall creat() returned: %d, error: %s\n", fd, strerror(errno));
#else
  int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, mode);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));
#endif

  int ret = lseek(fd, 0, whence);
  fprintf(stderr, "> Syscall lseek() returned: %d bytes written, error: %s\n", ret, strerror(errno));

  char str[] = "Hello!";
  int cnt = write(fd, str, strlen(str) - 1);
  fprintf(stderr, "> Syscall write() returned: %d bytes written, error: %s\n", cnt, strerror(errno));

  int ret = lseek(fd, 3, whence);
  fprintf(stderr, "> Syscall lseek() returned: %d bytes written, error: %s\n", ret, strerror(errno));

  char buf[10] = "";
  cnt = read(fd, buf, strlen(str) - 1);
  fprintf(stderr, "> Syscall read() returned: %d bytes read, error: %s\n", cnt, strerror(errno));

  int ret = lseek(fd, -5, whence);
  fprintf(stderr, "> Syscall lseek() returned: %d bytes written, error: %s\n", ret, strerror(errno));

  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));

  umask(old);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stdout, "Usage: %s [filename] [mode] [whence]\n", argv[0]);
  } else if (strlen(argv[3]) != 1) {
    fprintf(stdout, "Error: incorrect [whence] parameter\n");
  }
  
  rdwr_file(argv[1], argv[2], argv[3]);

  return 0;
}