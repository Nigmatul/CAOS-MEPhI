#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

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

void file_stat(const char *filename) {
  if (!filename) {
    fprintf(stderr, "> Error: invalid arguments\n");
  }
    
  int fd = open(filename, O_RDONLY);
  fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd, strerror(errno));

  struct stat st;
  int ret = stat(filename, &st);
  fprintf(stderr, "> Syscall fstat() returned: %d, error: %s\n", ret, strerror(errno));

  print_stat(filename, &st, stdout);

  fd = close(fd);
  fprintf(stderr, "> Syscall close() returned: %d, error: %s\n", fd, strerror(errno));
}   
