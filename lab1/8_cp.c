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

ssize_t write_all(int fd, const char *buf, size_t size) {
  if (!buf) {
    errno = EINVAL;
    return -1;
  }

  if (size == 0) {
    return 0;
  }

  ssize_t off = 0;

  while (off < (ssize_t)size) {
    ssize_t n = write(fd, buf + off, size - off);
    if (n > 0) {
      off += n;
    } else if (n == 0) {
      break;
    } else {
      if (errno == EINTR) continue;

      return -1;
    }
  }

  return off;
}

int copy(const char *filename1, const char *filename2) {
  int fd1, fd2;

  if (!filename1 || !filename2) {
    fd1 = STDIN_FILENO;
    fd2 = STDOUT_FILENO;
  } else {
    fd1 = open(filename1, O_RDONLY);
    if (fd1 == -1) {
      fprintf(stderr, "> Error opening source file: %s\n", strerror(errno));
      return -1;
    }
    fprintf(stderr, "> Syscall open() returned: %d, error: %s\n", fd1, strerror(errno));

    fd2 = creat(filename2, 0644);
    if (fd2 == -1) {
      fprintf(stderr, "> Error creating destination file: %s\n", strerror(errno));
      close(fd1);
      return -1;
    }
    fprintf(stderr, "> Syscall creat() returned: %d, error: %s\n", fd2, strerror(errno));
  }

  char buf[4096];
  ssize_t r, w;

  while ((r = read(fd1, buf, sizeof(buf))) > 0) {
    w = write_all(fd2, buf, r);
    if (w == -1) {
      fprintf(stderr, "> Error writing to destination file: %s\n", strerror(errno));
      close(fd1);
      close(fd2);
      return -1;
    }
  }

  if (r == -1) {
    fprintf(stderr, "> Error reading from source file: %s\n", strerror(errno));
  }

  close(fd1);
  close(fd2);
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stdout, "Usage: %s [filename1] [filename2]\n", argv[0]);
  }

  if (argc == 3) {
    if (copy(argv[1], argv[2]) == -1) {
      fprintf(stderr, "> Error during file copy\n");
      return 1;
    }
  } else {
    fprintf(stderr, "> Invalid arguments, copying from stdin to stdout\n");
    copy(NULL, NULL);
  }

  return 0;
}