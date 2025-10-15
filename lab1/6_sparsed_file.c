#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void print_stat(const char *filename, struct stat *st, FILE *stream) {
  if (!filename || !st || !stream) {
      return;
  }

  fprintf(stream, "File: %s\n", filename);
  fprintf(stream, "Device: %ld   Inode: %ld   Links: %ld\n",
          (long)st->st_dev, (long)st->st_ino, (long)st->st_nlink);

  fprintf(stream, "Mode: %o  (permissions: %04o)\n", st->st_mode, st->st_mode & 07777);

  fprintf(stream, "UID: %d  GID: %d\n", st->st_uid, st->st_gid);

  fprintf(stream, "Size: %lld bytes  Blocks: %lld  Block size: %ld\n",
          (long long)st->st_size,
          (long long)st->st_blocks,
          (long)st->st_blksize);

  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&st->st_mtime));
  fprintf(stream, "Modify: %s\n", buf);

  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&st->st_atime));
  fprintf(stream, "Access: %s\n", buf);

  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&st->st_ctime));
  fprintf(stream, "Change: %s\n", buf);
}


void sparsed_file(const char *filename, const char *str_mode) {
  if (!filename || !str_mode) {
    fprintf(stderr, "> Error: invalid arguments\n");
  }

  long mode = strtol(str_mode, NULL, 8);
  mode_t old = umask(0);
  
  printf("Old umask: %d, filename: %s, mode: %s(oct) <=> %ld(dec)\n", 
      old, filename, str_mode, mode);

#if USE_CREAT
  int fd = creat(filename, mode);
  fprintf(stderr, "> Syscall creat() returned: %d, error: %s\n", fd, strerror(errno));
#else
  int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, mode);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));
#endif

  /* First write */
  int ret = lseek(fd, 0, SEEK_SET);
  fprintf(stderr, "> Syscall lseek() returned: %d bytes written, error: %s\n", ret, strerror(errno));

  char str[] = "Hello!";
  int cnt = write(fd, str, strlen(str) - 1);
  fprintf(stderr, "> Syscall write() returned: %d bytes written, error: %s\n", cnt, strerror(errno));

  /* Second write */
  int ret = lseek(fd, 1LL<<20, SEEK_CUR);
  fprintf(stderr, "> Syscall lseek() returned: %d bytes written, error: %s\n", ret, strerror(errno));

  char str[] = "Hello!";
  int cnt = write(fd, str, strlen(str) - 1);
  fprintf(stderr, "> Syscall write() returned: %d bytes written, error: %s\n", cnt, strerror(errno));

  /* File info */
  struct stat file_stat;
  ret = fstat(fd, &file_stat);
  fprintf(stderr, "> Syscall fstat() returned: %d, error: %s\n", ret, strerror(errno));
  
  print_stat(filename, &file_stat, stdout);

  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));

  umask(old);
}
