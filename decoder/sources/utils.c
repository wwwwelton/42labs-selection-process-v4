#include "decoder.h"

void read_file_frequency(char *file_name, unsigned int *ascii) {
  int fd;
  unsigned char c;

  fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    dprintf(2, "Cannot open input file\n");
    exit(1);
  }
  while (read(fd, &c, 1) > 0) {
    ascii[c]++;
  }
  close(fd);
}

unsigned char *read_file(char *file_name) {
  int fd;
  char tmp[2] = {0};
  char *str;

  str = calloc(5000, sizeof(char));
  fd = open(file_name, O_RDONLY);
  while (read(fd, tmp, 1) > 0) {
    strcat(str, tmp);
  }
  close(fd);
  return ((unsigned char *)str);
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

long	timestamp(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
