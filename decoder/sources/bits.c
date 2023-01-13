#include "decoder.h"

void toggle_bit(char *array, int index) {
  array[index / 8] ^= 1 << (index % 8);
}

char get_bit(char *array, int index) {
  return 1 & (array[index / 8] >> (index % 8));
}
