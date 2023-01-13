#include "encoder.h"

int main(int argc, char **argv) {
  unsigned int ascii[ASCII_HEIGHT] = {0};
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;
  t_decomp_segment segment;
  size_t size;

  check_args(argc, argv);

  read_files_frequency(argc, argv, ascii);

  init_list(&list);
  fill_list(ascii, ASCII_HEIGHT, &list);

  root = create_huffman_tree(&list);
  columns = huffman_tree_height(root) + 1;

  dictionary = alloc_dictionary(columns);
  generate_dictionary(dictionary, root, "", columns);

  size = 0;
  compressed = compress_str(argc, argv, dictionary, &size);

  set_compressed_file_segment(compressed, ascii, size, 1);
  segment = get_decompressed_file_segment(2);

  printf("\n[DICTIONARY]\n\n");
  print_dictionary(dictionary);

  printf("\n[COMPRESSED]\n");
  printf("%s", compressed);

  printf("\n\n[DECOMPRESSED]\n");
  printf("Compressed size:   %d bytes\n", segment.file->comp_bytes);
  printf("Decompressed size: %d bytes\n", segment.file->decomp_bytes);
  printf("Time to decode:    %ld ms\n\n", segment.file->time);
  printf("Data content: \n%s\n", segment.file->decompressed);

  shmdt(segment.file);
  shmctl(segment.shmid, IPC_RMID, NULL);

  free_dictionary(dictionary, ASCII_HEIGHT);
  free(compressed);
  free_tree(root);

  return (0);
}
