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

int get_string_lenght(char **dictionary, unsigned char *str) {
  int i;
  int lenght;

  lenght = 0;
  i = 0;
  while (str[i]) {
    lenght = lenght + strlen(dictionary[(int)str[i]]);
    i++;
  }

  return (lenght + 1);
}

char *compress_str(char **dictionary, unsigned char *str) {
  int i;
  int lenght;
  char *compressed;

  lenght = get_string_lenght(dictionary, str);
  compressed = calloc(lenght, sizeof(char));

  i = 0;
  while (str[i]) {
    strcat(compressed, dictionary[(int)str[i]]);
    i++;
  }
  return compressed;
}
