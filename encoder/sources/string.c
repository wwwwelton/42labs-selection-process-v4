#include "encoder.h"

char **alloc_dictionary(int columns) {
  char **dictionary;

  dictionary = malloc(sizeof(char *) * ASCII_HEIGHT);

  for (int i = 0; i < ASCII_HEIGHT; i++) {
    dictionary[i] = calloc(columns, sizeof(char));
  }

  return (dictionary);
}

void generate_dictionary(char **dictionary, t_node *root, char *path, int columns) {
  char left[columns];
  char right[columns];

  if (root->left == NULL && root->right == NULL)
    strcpy(dictionary[root->character], path);
  else {
    strcpy(left, path);
    strcpy(right, path);

    strcat(left, "0");
    strcat(right, "1");

    generate_dictionary(dictionary, root->left, left, columns);
    generate_dictionary(dictionary, root->right, right, columns);
  }
}

int get_string_lenght(int argc, char **argv, char **dictionary) {
  int lenght;
  unsigned char c;

  lenght = 0;
  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);

    while (read(fd, &c, 1) > 0) {
      lenght = lenght + strlen(dictionary[c]);
    }
    close(fd);
  }

  return (lenght + 1);
}

char *compress_str(int argc, char **argv, char **dictionary, size_t *size) {
  int lenght;
  char *tmp;
  char *compressed;
  unsigned char c;

  lenght = get_string_lenght(argc, argv, dictionary);
  tmp = calloc(lenght, sizeof(char));

  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);

    while (read(fd, &c, 1) > 0) {
      strcat(tmp, dictionary[c]);
    }
    close(fd);
  }

  compressed = calloc(ARRAY_SIZE(lenght) + 1, sizeof(char));
  for (int i = 0; tmp[i]; i++) {
    if (tmp[i] == '1') {
      toggle_bit(compressed, i);
    }
  }

  *size = lenght - 1;
  free(tmp);
  return compressed;
}
