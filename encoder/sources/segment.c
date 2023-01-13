#include "encoder.h"

t_decomp_segment get_decompressed_file_segment(int id) {
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

void set_compressed_file_segment(char *compressed,
                                 unsigned int *ascii,
                                 size_t size,
                                 int id) {
  t_comp_file *file;

  key_t key = ftok("shmfile", id);
  int shmid = shmget(key, sizeof(t_comp_file), 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  strcpy(file->compressed, compressed);
  memcpy(file->ascii, ascii, sizeof(unsigned int) * ASCII_HEIGHT);
  file->size = size;
  shmdt(file);
}
