#include "encoder.h"

int main(int argc, char **argv) {
  t_data data;

  check_args(argc, argv);

  init_data(&data);

  read_files_frequency(argc, argv, data.ascii);

  init_list(&data.list);
  fill_list(data.ascii, ASCII_HEIGHT, &data.list);

  data.root = create_huffman_tree(&data.list);
  data.columns = huffman_tree_height(data.root) + 1;

  data.dictionary = alloc_dictionary(data.columns);
  generate_dictionary(data.dictionary, data.root, "", data.columns);

  data.size = 0;
  data.compressed = compress_str(argc, argv, data.dictionary, &data.size);

  set_compressed_file_segment(data.compressed, data.ascii, data.size, 1);
  data.segment = get_decompressed_file_segment(2);

  printf("\n[DICTIONARY]\n\n");
  print_dictionary(data.dictionary);

  printf("\n[COMPRESSED]\n");
  printf("%s", data.compressed);

  printf("\n\n[DECOMPRESSED]\n");
  printf("Compressed size:   %d bytes\n", data.segment.file->comp_bytes);
  printf("Decompressed size: %d bytes\n", data.segment.file->decomp_bytes);
  printf("Time to decode:    %ld ms\n\n", data.segment.file->time);
  printf("Data content: \n%s\n", data.segment.file->decompressed);

  clean_data(NULL, "");

  return (0);
}
