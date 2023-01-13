#include "decoder.h"

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
