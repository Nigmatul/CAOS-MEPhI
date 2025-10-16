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

int copy() {
  char buf[4096];
  ssize_t r, w;

  // Чтение из stdin и запись в stdout
  while ((r = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    w = write_all(STDOUT_FILENO, buf, r);
    if (w == -1) {
      fprintf(stderr, "> Error writing to destination\n");
      return -1;
    }
  }

  if (r == -1) {
    fprintf(stderr, "> Error reading from source\n");
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  if (argc == 3) {
    // Открываем файлы, на которые перенаправим stdin и stdout
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
      perror("Failed to open input file");
      return 1;
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
      perror("Failed to open output file");
      close(fd_in);
      return 1;
    }

    if (dup2(fd_in, STDIN_FILENO) == -1) {
      perror("Failed to redirect stdin");
      close(fd_in);
      close(fd_out);
      return 1;
    }

    if (dup2(fd_out, STDOUT_FILENO) == -1) {
      perror("Failed to redirect stdout");
      close(fd_in);
      close(fd_out);
      return 1;
    }


    close(fd_in);
    close(fd_out);

    // Копируем данные
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
