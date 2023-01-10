#ifndef ENCODER
#define ENCODER

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_frequency_table {
  int character;
  int frequency;
} t_frequency_table;

void sort_array(t_frequency_table *array, int size);

#endif
