#include "encoder.h"

int main(int argc, char **argv) {
  unsigned int ascii[ASCII_HEIGHT] = {0};
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;
  char *decompressed;
  unsigned char *file_content;
  t_decomp_segment segment;

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

  set_compressed_file_segment(compressed, ascii, 1);
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
  free(decompressed);
  free(file_content);
  free_tree(root);

  return (0);
}
