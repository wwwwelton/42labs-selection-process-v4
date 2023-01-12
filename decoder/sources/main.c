#include "decoder.h"

static t_comp_segment get_compressed_file_segment(int id) {
  t_comp_segment segment;

  segment.key = ftok("shmfile", id);
  segment.shmid = shmget(segment.key, sizeof(t_comp_file), 0666 | IPC_CREAT);
  segment.file = shmat(segment.shmid, (void *)0, 0);
  if (!strlen(segment.file->file_name)) {
    dprintf(2, "The file to be decoded was not found\n");
    exit(1);
  }
  return (segment);
}

static void set_decompressed_file_segment(char *decompressed,
                                          int c_size,
                                          int d_size,
                                          long time,
                                          int id) {
  t_decomp_file *file;

  key_t key = ftok("shmfile", id);
  int shmid = shmget(key, sizeof(t_decomp_file), 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  strcpy(file->decompressed, decompressed);
  file->comp_bytes = c_size;
  file->decomp_bytes = d_size;
  file->time = time;
  file->ok = 1;
  shmdt(file);
}

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
