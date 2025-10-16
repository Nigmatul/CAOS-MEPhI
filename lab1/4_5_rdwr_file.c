#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void rdwr_file(const char *filename, const char *str_mode, int whence) {
  if (!filename || !str_mode) {
    fprintf(stderr, "> Error: inavlid arguments\n");
  }

  long mode = strtol(str_mode, NULL, 8);
  mode_t old = umask(0);
  
  printf("Old umask: %d, filename: %s, mode: %s(oct) <=> %ld(dec), whence: %d\n", 
      old, filename, str_mode, mode, whence);

  int fd = open(filename, O_CREAT|O_RDWR|O_TRUNC, mode);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));

  /* WRITE */
  int ret = lseek(fd, 0, whence);
  fprintf(stderr, "> Syscall lseek() returned: %d, error: %s\n", ret, strerror(errno));

  char str[] = "Hello!";
  int cnt = write(fd, str, strlen(str));
  fprintf(stderr, "> Syscall write() returned: %d bytes written, error: %s\n", cnt, strerror(errno));

  fprintf(stderr, "---------------------------------------\n");

  /* READ */
  ret = lseek(fd, 3, whence);
  fprintf(stderr, "> Syscall lseek() returned: %d, error: %s\n", ret, strerror(errno));

  char buf[10] = "";
  cnt = read(fd, buf, strlen(str));
  fprintf(stderr, "> Syscall read() returned: %d bytes read, error: %s\n", cnt, strerror(errno));
  fprintf(stdout, "> buf[] = %s\n", buf);

  ret = lseek(fd, -5, whence);
  fprintf(stderr, "> Syscall lseek() returned: %d, error: %s\n", ret, strerror(errno));
  errno = 0;

  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));

  umask(old);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stdout, "Usage: %s [filename] [mode]\n", argv[0]);
  }
  
  rdwr_file(argv[1], argv[2], SEEK_SET);
  fprintf(stdout, "#########################\n");
  rdwr_file(argv[1], argv[2], SEEK_CUR);
  fprintf(stdout, "#########################\n");
  rdwr_file(argv[1], argv[2], SEEK_END);

  return 0;
}