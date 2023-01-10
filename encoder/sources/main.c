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
  printf("[LIST]\n\n");
  print_list(&list);
  root = create_huffman_tree(&list);

  printf("\n[TREE]\n\n");
  print_huffman_tree(root, 0);

  columns = huffman_tree_height(root) + 1;
  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  printf("\n[DICTIONARY]\n\n");

  print_dictionary(dictionary);

  compressed = compress_str(dictionary, "Vamos aprender a programa");
  printf("%s", compressed);

  //   for (int i = 0; i < 128; i++) {
  //     if (ascii[i] > 0) {
  //       printf("%c: %d\n", i, ascii[i]);
  //     }
  //   }
  return (0);
}
