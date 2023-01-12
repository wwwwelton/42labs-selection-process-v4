#include "encoder.h"

static t_decomp_segment get_decompressed_file_segment(int id) {
  t_decomp_segment segment;

  segment.key = ftok("shmfile", id);
  segment.shmid = shmget(segment.key, sizeof(t_decomp_file), 0666 | IPC_CREAT);
  segment.file = shmat(segment.shmid, (void *)0, 0);
  printf("Wating for decoded file\n");
  while (!segment.file->ok) {
    sleep(1);
  }
  return (segment);
}

static void set_compressed_file_segment(char *compressed,
                                        unsigned int *ascii,
                                        int id) {
  t_comp_file *file;

  key_t key = ftok("shmfile", id);
  int shmid = shmget(key, sizeof(t_comp_file), 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
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
