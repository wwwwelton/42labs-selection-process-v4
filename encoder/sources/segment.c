#include "encoder.h"

t_decomp_segment get_decompressed_file_segment(int id) {
  t_decomp_segment segment;

  segment.key = ftok(FILE_NAME, id);
  segment.shmid = shmget(segment.key, sizeof(t_decomp_file), 0666 | IPC_CREAT);
  segment.file = shmat(segment.shmid, (void *)0, 0);
  dprintf(2, "Wating for decoded data...\n");
  while (!segment.file->ok) {
    sleep(1);
  }
  return (segment);
}

void set_compressed_file_segment(char *compressed,
                                 unsigned int *ascii,
                                 size_t size,
                                 int id) {
  t_comp_file *file;

  key_t key = ftok(FILE_NAME, id);
  int shmid = shmget(key, sizeof(t_comp_file), 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  memcpy(file->compressed, compressed, ARRAY_SIZE(size));
  memcpy(file->ascii, ascii, sizeof(unsigned int) * ASCII_HEIGHT);
  file->size = size;
  shmdt(file);
}
