#include "encoder.h"

void read_files_frequency(int argc, char **argv, unsigned int *ascii) {
  unsigned char c;

  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      dprintf(2, "Failed to open %s!\n", argv[i]);
      exit(1);
    }
    while (read(fd, &c, 1) > 0) {
      ascii[c]++;
    }
    close(fd);
  }
}

void dup_dictionary(unsigned char (*dest)[8], char **src) {
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 8; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

void free_dictionary(char **dictionary, int size) {
  for (int i = 0; i < size; i++) {
    free(dictionary[i]);
    dictionary[i] = NULL;
  }
  free(dictionary);
  dictionary = NULL;
}

void free_tree(t_node *root) {
  if (root == NULL)
    return;

  free_tree(root->left);
  free_tree(root->right);

  free(root);
  root = NULL;
}

long timestamp(void) {
  struct timeval time;

  gettimeofday(&time, NULL);
  return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void check_args(int argc, char **argv) {
  if (argc == 1) {
    dprintf(2, "At least one file must be specified!\n");
    exit(1);
  }
  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      dprintf(2, "Failed to open %s!\n", argv[i]);
      exit(1);
    }
    close(fd);
  }
}
