#include "decoder.h"

void print_frequency(unsigned int *array) {
  for (int i = 0; i < ASCII_HEIGHT; i++) {
    if (array[i] > 0) {
      printf("%d: %u\n", i, array[i]);
    }
  }
}

void print_list(t_list *list) {
  t_node *tmp;

  tmp = list->root;
  while (tmp) {
    printf("%c: %d\n", tmp->character, tmp->frequency);
    tmp = tmp->next;
  }
}

void print_huffman_tree(t_node *root, int height) {
  if (root->left == NULL && root->right == NULL) {
    printf("%c: f:%d h:%d\n", root->character, root->frequency, height);
  } else {
    print_huffman_tree(root->left, height + 1);
    print_huffman_tree(root->right, height + 1);
  }
}

void print_dictionary(char **dictionary) {
  for (int i = 0; i < ASCII_HEIGHT; i++) {
    if (strlen(dictionary[i])) {
      dprintf(2, "%c: %s\n", i, dictionary[i]);
    }
  }
}

void print_info(t_data *data) {
  if (DEBUG) {
    dprintf(2, "\n[DICTIONARY]\n\n");
    print_dictionary(data->dictionary);

    dprintf(2, "\n[COMPRESSED]\n");
    dprintf(2, "%s", data->segment.file->compressed);

    dprintf(2, "\n\n[DECOMPRESSED]\n");
    dprintf(2, "Compressed size:   %ld bytes\n", strlen(data->segment.file->compressed));
    dprintf(2, "Decompressed size: %ld bytes\n", strlen(data->decompressed));
    dprintf(2, "Time to decode:    %ld ms\n\n", data->time[1] - data->time[0]);
    dprintf(2, "Data content: \n");
    printf("%s", data->decompressed);
    dprintf(2, "\n");
  }
}
