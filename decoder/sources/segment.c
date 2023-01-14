#include "decoder.h"

t_comp_segment get_compressed_file_segment(int id) {
  t_comp_segment segment;

  segment.key = ftok(FILE_NAME, id);
  segment.shmid = shmget(segment.key, 0, 0);
  if (segment.shmid == -1) {
    dprintf(2, "The data to be decoded was not found\n");
    exit(1);
  }
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
  shmdt(file);
  dprintf(2, "Decode process complete!\n");
}
