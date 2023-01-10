#include "encoder.h"

void sort_array(int *array, int size) {
  int swapped = 1;
  int tmp;

  while (swapped) {
    swapped = 0;

    for (int i = 0; i < (size - 1); i++) {
      if (array[i] > array[i + 1]) {
        tmp = array[i];
        array[i] = array[i + 1];
        array[i + 1] = tmp;
        swapped = 1;
      }
    }
  }
}
