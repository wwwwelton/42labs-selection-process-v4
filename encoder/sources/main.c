#include "encoder.h"

int main(int argc, char **argv) {
  int fd;
  unsigned char c;
  unsigned int ascii[ASCII_HEIGHT] = {0};
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;
  char *decompressed;
  unsigned char *file_content;

  (void)argc;

  fd = open(argv[1], O_RDONLY);
  while (read(fd, &c, 1) > 0) {
    ascii[c]++;
  }
  close(fd);

  init_list(&list);
  fill_list(ascii, ASCII_HEIGHT, &list);

  root = create_huffman_tree(&list);
  columns = huffman_tree_height(root) + 1;

  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  file_content = read_file(argv[1]);
  compressed = compress_str(dictionary, file_content);
  decompressed = decompress_str(compressed, root);

  printf("\n[DICTIONARY]\n\n");
  print_dictionary(dictionary);
  printf("%s", compressed);
  printf("\n%s", decompressed);

  free_dictionary(dictionary, ASCII_HEIGHT);
  free(compressed);
  free(decompressed);
  free(file_content);
  free_tree(root);

  return (0);
}
