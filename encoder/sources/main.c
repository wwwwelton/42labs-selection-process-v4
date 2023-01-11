#include "encoder.h"

void dump_compressed(char *file_name, char *compressed, unsigned int *ascii) {
  t_file *file;

  key_t key = ftok("shmfile", 65);
  int shmid = shmget(key, sizeof(t_file), 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  strcpy(file->file_name, file_name);
  strcpy(file->compressed, compressed);
  memcpy(file->ascii, ascii, sizeof(unsigned int) * ASCII_HEIGHT);
  shmdt(file);
}

int main(int argc, char **argv) {
  unsigned int ascii[ASCII_HEIGHT] = {0};
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;
  char *decompressed;
  unsigned char *file_content;

  (void)argc;

  read_file_frequency(argv[1], ascii);

  init_list(&list);
  fill_list(ascii, ASCII_HEIGHT, &list);

  root = create_huffman_tree(&list);
  columns = huffman_tree_height(root) + 1;

  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  file_content = read_file(argv[1]);
  compressed = compress_str(dictionary, file_content);
  decompressed = decompress_str(compressed, root);

  dump_compressed(argv[1], compressed, ascii);

  printf("\n[DICTIONARY]\n\n");
  print_dictionary(dictionary);

  printf("\n[COMPRESSED]\n");
  printf("%s", compressed);

  printf("\n\n[DECOMPRESSED]\n");
  printf("%s", decompressed);

  free_dictionary(dictionary, ASCII_HEIGHT);
  free(compressed);
  free(decompressed);
  free(file_content);
  free_tree(root);

  return (0);
}
