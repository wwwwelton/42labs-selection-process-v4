#include "encoder.h"

int main(int argc, char **argv) {
  int fd;
  char c;
  int ascii[128] = {0};
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;

  (void)argc;

  fd = open(argv[1], O_RDONLY);
  while (read(fd, &c, 1) > 0) {
    ascii[(int)c]++;
  }

  init_list(&list);
  fill_list(ascii, 128, &list);

  root = create_huffman_tree(&list);
  columns = huffman_tree_height(root) + 1;

  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  compressed = compress_str(dictionary, "bora fazer um test");

  printf("\n[DICTIONARY]\n\n");
  print_dictionary(dictionary);
  printf("%s", compressed);

  free_dictionary(dictionary, 128);
  free(compressed);
  free_tree(root);

  return (0);
}
