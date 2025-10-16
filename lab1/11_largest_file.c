#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

ssize_t read_all(int fd, const char *buf, size_t size) {
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

int reverse(char *buf, size_t size) {
  if (!buf) {
    errno = EINVAL;
    return -1;
  }

  for (size_t i = 0; i < size; i++) {
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
