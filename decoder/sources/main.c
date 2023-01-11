#include "decoder.h"

int main(void) {
  t_file *file;
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *decompressed;

  key_t key = ftok("shmfile", 65);
  int shmid = shmget(key, sizeof(t_file), 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  if (!strlen(file->file_name)) {
    dprintf(2, "The file to be decoded was not found\n");
    exit(1);
  }

  init_list(&list);
  fill_list(file->ascii, ASCII_HEIGHT, &list);

  root = create_huffman_tree(&list);
  columns = huffman_tree_height(root) + 1;

  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  decompressed = decompress_str(file->compressed, root);
  printf("%s", decompressed);

  shmdt(file);
  shmctl(shmid, IPC_RMID, NULL);

  free_dictionary(dictionary, ASCII_HEIGHT);
  free(decompressed);
  free_tree(root);
  return (0);
}
