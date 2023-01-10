#include "encoder.h"

void huffman_codes(t_frequency_table *ref, int size)
{
 for (int i = 0; i < size; i++) {
    printf("%c: %d\n", ref[i].character, ref[i].frequency);
  }
}

int main(int argc, char **argv) {
  int fd;
  char c;
  int size = 0;
  t_frequency_table ascii[128];
  t_frequency_table ref[128];

  (void)argc;
  fd = open(argv[1], O_RDONLY);

  for (int i = 0; i < 128; i++) {
    ascii[i].character = i;
    ascii[i].frequency = 0;
  }

  while (read(fd, &c, 1) > 0) {
    ascii[(int)c].frequency++;
  }

  sort_array(ascii, 128);

  for (int i = 0; i < 128; i++)
  {
	if (ascii[i].frequency > 0)
	{
		ref[size] = ascii[i];
		size++;
	}
  }

  huffman_codes(ref, size);

//   for (int i = 0; i < size; i++) {
//     printf("%c: %d\n", ref[i].character, ref[i].frequency);
//   }
  return (0);
}
