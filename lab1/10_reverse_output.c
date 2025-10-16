#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

ssize_t read_all(int fd, char *buf, size_t size) {
  if (!buf) {
    errno = EINVAL;
    return -1;
  }

  if (size == 0) {
    return 0;
  }

  ssize_t off = 0;

  while (off < (ssize_t)size) {
    ssize_t n = read(fd, buf + off, size - off);
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

int reverse(char *buf, ssize_t size) {
  if (!buf) {
    errno = EINVAL;
    return -1;
  }

  for (ssize_t i = 0; i < size / 2; i++) {
    char tmp = buf[i];
    buf[i] = buf[(size - 1) - i];
    buf[(size - 1) - i] = tmp;
  }

  return 0;
}

int process(const char *filename) {
  if (!filename) {
    errno = EINVAL;
    return -1;
  }

  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("> reverse()");
    return -1;
  }

  char buf[4096];
  ssize_t r, w;

  // Чтение из stdin и запись в stdout
  while ((r = read(fd, buf, sizeof(buf))) > 0) {
    reverse(buf, r);
    w = write_all(STDOUT_FILENO, buf, r);
    if (w == -1) {
      fprintf(stderr, "> Syscall write() returned: %s\n", strerror(errno));
      return -1;
    }
  }

  if (r == -1) {
    fprintf(stderr, "> Syscall read() returned: %s\n", strerror(errno));
    return -1;
  }

  close(fd);

  return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stdout, "Usage: %s [filename]\n", argv[0]);
    }

    process(argv[1]);

    return 0;
}
