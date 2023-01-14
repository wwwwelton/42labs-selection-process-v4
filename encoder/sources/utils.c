#include "encoder.h"

void init_data(t_data *data) {
  int fd = open(FILE_NAME, O_RDWR);
  if (fd == -1)
    fd = open(FILE_NAME, O_CREAT | O_TRUNC, 0777);
  close(fd);
  clean_data(data, "");
  memset(data->ascii, 0, sizeof(data->ascii));
  data->root = NULL;
  data->columns = 0;
  data->dictionary = NULL;
  data->compressed = NULL;
  data->size = 0;
  data->segment.file = NULL;
  data->segment.key = 0;
  data->segment.shmid = 0;
}

void read_files_frequency(int argc, char **argv, unsigned int *ascii) {
  unsigned char c;
  long size = 0;

  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      dprintf(2, "Failed to open %s!\n", argv[i]);
      exit(1);
    }
    while (read(fd, &c, 1) > 0) {
      ascii[c]++;
      size++;
    }
    close(fd);
  }
  if (size < 2) {
    dprintf(2, "The data to be encoded has %ld bytes and cannot be compressed!\n", size);
    exit(1);
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

void clean_data(t_data *data, char *error) {
  static t_data *ptr = NULL;

  if (data) {
    ptr = data;
    shmctl(shmget(ftok(FILE_NAME, 1), 0, 0), IPC_RMID, NULL);
    shmctl(shmget(ftok(FILE_NAME, 2), 0, 0), IPC_RMID, NULL);
  } else {
    if (*error) {
      dprintf(2, "%s\n", error);
      exit(1);
    }
    shmdt(ptr->segment.file);
    shmctl(ptr->segment.shmid, IPC_RMID, NULL);

    free_dictionary(ptr->dictionary, ASCII_HEIGHT);
    free(ptr->compressed);
    free_tree(ptr->root);
  }
}
