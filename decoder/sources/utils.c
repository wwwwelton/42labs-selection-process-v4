#include "decoder.h"

void init_data(t_data *data) {
  clean_data(data, "");
  data->root = NULL;
  data->columns = 0;
  data->dictionary = NULL;
  data->decompressed = NULL;
  data->segment.file = NULL;
  data->segment.key = 0;
  data->segment.shmid = 0;
  data->time[0] = 0;
  data->time[1] = 0;
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

void clean_data(t_data *data, char *error) {
  static t_data *ptr = NULL;

  if (data) {
    ptr = data;
  } else {
    if (*error) {
      dprintf(2, "%s\n", error);
      exit(1);
    }
    shmdt(ptr->segment.file);
    shmctl(ptr->segment.shmid, IPC_RMID, NULL);

    free_dictionary(ptr->dictionary, ASCII_HEIGHT);
    free(ptr->decompressed);
    free_tree(ptr->root);
  }
}
