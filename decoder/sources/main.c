#include "decoder.h"

int main(void) {
  t_data data;

  init_data(&data);

  data.time[0] = timestamp();
  data.segment = get_compressed_file_segment(1);

  init_list(&data.list);
  fill_list(data.segment.file->ascii, ASCII_HEIGHT, &data.list);

  data.root = create_huffman_tree(&data.list);
  data.columns = huffman_tree_height(data.root) + 1;

  data.dictionary = alloc_dictionary(data.columns);
  generate_dictionary(data.dictionary, data.root, "", data.columns);

  data.decompressed = decompress_str(data.segment.file->compressed,
                                     data.segment.file->size, data.root);
  data.time[1] = timestamp();
  set_decompressed_file_segment(data.decompressed,
                                strlen(data.segment.file->compressed),
                                strlen(data.decompressed),
                                data.time[1] - data.time[0],
                                2);

  print_info(&data);

  clean_data(NULL, "");
  return (0);
}
