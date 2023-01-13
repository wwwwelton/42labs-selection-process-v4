#include "decoder.h"

int main(void) {
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *decompressed;
  t_comp_segment segment;
  long time[2];

  time[0] = timestamp();
  segment = get_compressed_file_segment(1);

  init_list(&list);
  fill_list(segment.file->ascii, ASCII_HEIGHT, &list);

  root = create_huffman_tree(&list);
  columns = huffman_tree_height(root) + 1;

  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  decompressed = decompress_str(segment.file->compressed, root);
  time[1] = timestamp();
  printf("%s", decompressed);
  set_decompressed_file_segment(decompressed,
                                strlen(segment.file->compressed),
                                strlen(decompressed),
                                time[1] - time[0],
                                2);

  shmdt(segment.file);
  shmctl(segment.shmid, IPC_RMID, NULL);

  free_dictionary(dictionary, ASCII_HEIGHT);
  free(decompressed);
  free_tree(root);
  return (0);
}
