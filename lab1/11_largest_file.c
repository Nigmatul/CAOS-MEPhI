#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

off_t get_size(const char *filename) {
  if (!filename) {
    errno = EINVAL;
    return -1;
  }

  struct stat st;
  int ret = stat(filename, &st);
  if (ret == -1) {
    fprintf(stderr, "> Syscall fstat() returned: %d, error: %s\n", ret, strerror(errno));
  }
  return st.st_size;
}

void largest_file(int argc, char **argv) {
  char *max_file = NULL;
  off_t max_size = 0;

  for (int i = 1; i < argc; i++) {
    off_t size = get_size(argv[i]);
    if (size > max_size) {
      max_size = size;
      max_file = argv[i];
    }
  }

  fprintf(stdout, "Largest file: %s, size = %lld\n", max_file, (long long)max_size);
}

int main(int argc, char **argv) {
  
  largest_file(argc, argv);

  return 0;
}