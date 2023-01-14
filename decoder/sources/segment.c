#include "decoder.h"

size_t get_compressed_file_segment_size(int id) {
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

void set_decompressed_file_segment_size(size_t size, int id) {
  int *ptr;

  key_t key = ftok(FILE_NAME, id);
  int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
  ptr = shmat(shmid, (void *)0, 0);
  *ptr = size;
  shmdt(ptr);
}

t_comp_segment get_compressed_file_segment(int id) {
  t_comp_segment segment;
  size_t size;

  size = get_compressed_file_segment_size(3);
  if (!size) {
    dprintf(2, "The file to be decoded was not found\n");
    exit(1);
  }
  segment.key = ftok(FILE_NAME, id);
  segment.shmid = shmget(segment.key, sizeof(t_comp_file) + ARRAY_SIZE(size) + 1, 0666 | IPC_CREAT);
  segment.file = shmat(segment.shmid, (void *)0, 0);
  return (segment);
}

void set_decompressed_file_segment(char *decompressed,
                                   int c_size,
                                   int d_size,
                                   long time,
                                   int id) {
  t_decomp_file *file;

  key_t key = ftok(FILE_NAME, id);
  int shmid = shmget(key, sizeof(t_decomp_file) + sizeof(char) * d_size + 1, 0666 | IPC_CREAT);
  file = shmat(shmid, (void *)0, 0);
  strcpy(file->decompressed, decompressed);
  file->comp_bytes = c_size;
  file->decomp_bytes = d_size;
  file->time = time;
  file->ok = 1;
  set_decompressed_file_segment_size(d_size, 4);
  shmdt(file);
  dprintf(2, "Decode process complete!\n");
}
