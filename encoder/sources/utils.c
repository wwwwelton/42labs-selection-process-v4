#include "encoder.h"

void sort_array(t_frequency_table *array, int size) {
  int swapped = 1;
  t_frequency_table tmp;

  while (swapped) {
    swapped = 0;

    for (int i = 0; i < (size - 1); i++) {
      if (array[i].frequency > array[i + 1].frequency) {
        tmp = array[i];
        array[i] = array[i + 1];
        array[i + 1] = tmp;
        swapped = 1;
      }
    }
  }
}
