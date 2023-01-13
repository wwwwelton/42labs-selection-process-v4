#include "decoder.h"

t_comp_segment get_compressed_file_segment(int id) {
  t_comp_segment segment;

  segment.key = ftok("shmfile", id);
  segment.shmid = shmget(segment.key, sizeof(t_comp_file), 0666 | IPC_CREAT);
  segment.file = shmat(segment.shmid, (void *)0, 0);
  if (!strlen(segment.file->compressed)) {
    dprintf(2, "The file to be decoded was not found\n");
    exit(1);
  }
  return (segment);
}

void set_decompressed_file_segment(char *decompressed,
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
