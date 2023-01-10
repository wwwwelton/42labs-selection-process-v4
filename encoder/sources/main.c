#include "encoder.h"

char *read_file(char *file) {
  int fd;
  char tmp[2] = {0};
  char *str;

  str = calloc(1000, sizeof(char *));
  fd = open(file, O_RDONLY);
  while (read(fd, tmp, 1) > 0) {
    strcat(str, tmp);
  }
  return str;
}

int main(int argc, char **argv) {
  int fd;
  char c;
  int ascii[128] = {0};
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;
  char *file_content;

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

  file_content = read_file(argv[1]);
  compressed = compress_str(dictionary, file_content);

  printf("\n[DICTIONARY]\n\n");
  print_dictionary(dictionary);
  printf("%s", compressed);

  free_dictionary(dictionary, 128);
  free(compressed);
  free(file_content);
  free_tree(root);

  return (0);
}
