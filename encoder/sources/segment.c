#include "encoder.h"

void set_compressed_file_segment_size(size_t size, int id) {
  int *ptr;

  key_t key = ftok(FILE_NAME, id);
  int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
  ptr = shmat(shmid, (void *)0, 0);
  *ptr = size;
  shmdt(ptr);
}

size_t get_decompressed_file_segment_size(int id) {
  int *ptr;
  size_t size;

  key_t key = ftok(FILE_NAME, id);
  int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
  ptr = shmat(shmid, (void *)0, 0);
  size = *ptr;
  shmdt(ptr);
  shmctl(shmid, IPC_RMID, NULL);

  return (size);
}

t_decomp_segment get_decompressed_file_segment(int id) {
  t_decomp_segment segment;
  size_t size = 0;

  size = get_decompressed_file_segment_size(4);
  while (!size) {
    sleep(1);
    size = get_decompressed_file_segment_size(4);
  }
  segment.key = ftok(FILE_NAME, id);
  segment.shmid = shmget(segment.key, sizeof(t_decomp_file) + ARRAY_SIZE(size) + 1, 0666 | IPC_CREAT);
  segment.file = shmat(segment.shmid, (void *)0, 0);
  dprintf(2, "Wating for decoded data...\n");
  return (segment);
}

void set_compressed_file_segment(char *compressed,
                                 unsigned int *ascii,
                                 size_t size,
                                 int id) {
  t_comp_file *file;

  key_t key = ftok(FILE_NAME, id);
  int shmid = shmget(key, sizeof(t_comp_file) + ARRAY_SIZE(size) + 1, 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  memcpy(file->compressed, compressed, ARRAY_SIZE(size));
  memcpy(file->ascii, ascii, sizeof(unsigned int) * ASCII_HEIGHT);
  file->size = size;
  set_compressed_file_segment_size(size, 3);
  shmdt(file);
}
