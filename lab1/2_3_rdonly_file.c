#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void rdonly_file(const char *filename, const char *str_mode) {
  if (!filename || !str_mode) {
      fprintf(stderr, "> Error: invalid arguments\n");
  }

  mode_t old = umask(0);
  long mode = strtol(str_mode, NULL, 8);
  
  printf("Old umask: %d, filename: %s, mode: %s(oct) <=> %ld(dec)\n", old, filename, str_mode, mode);

  /* Creating file*/
#if USE_CREAT
  int fd = creat(filename, mode);
  fprintf(stderr, "> Syscall creat() returned: %d, error: %s\n", fd, strerror(errno));
#else
  int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, mode);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));
#endif
  /*--------------------------------*/

  /* First writing */
  char str[] = "Hello!";
  int cnt = write(fd, str, strlen(str));
  fprintf(stderr, "> Syscall write() returned: %d bytes written, error: %s\n", cnt, strerror(errno));

  /* EXPERIMENT: try to remove file at this step */
  fprintf(stdout, "EXPERIMENT: try to remove file at this step");
  getchar();

  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));

  umask(old);

  getchar();
  /*--------------------------------*/

  /* Reading */
  fd = open(filename, O_RDONLY);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));

  char buf[10] = "";
  cnt = read(fd, buf, strlen(str));
  fprintf(stderr, "> Syscall read() returned: %d bytes read, error: %s\n", cnt, strerror(errno));

  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));

  getchar();
  /*--------------------------------*/

  /* Try to open with inappropriate mode */
  fd = open(filename, O_RDWR);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));
  
  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));
  /*--------------------------------*/
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stdout, "Usage: %s [filename] [mode]\n", argv[0]);
  }
  
  rdonly_file(argv[1], argv[2]);

  return 0;
}