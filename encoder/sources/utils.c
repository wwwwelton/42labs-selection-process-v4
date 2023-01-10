#include "encoder.h"

void sort_array(int *array, int size) {
  int swapped = 1;
  int tmp;

  while (swapped) {
    swapped = 0;

    for (int i = 0; i < (size - 1); i++) {
      if (array[i] > array[i + 1]) {
        tmp = array[i];
        array[i] = array[i + 1];
        array[i + 1] = tmp;
        swapped = 1;
      }
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
