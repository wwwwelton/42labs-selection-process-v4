#include "decoder.h"

t_node *create_node(char character, int frequency) {
  t_node *new;

  new = (t_node *)malloc(sizeof(t_node));
  new->character = character;
  new->frequency = frequency;
  new->right = NULL;
  new->left = NULL;
  new->next = NULL;

  return (new);
}

void init_list(t_list *list) {
  list->root = NULL;
  list->size = 0;
}

void insert_node_list(t_list *list, t_node *node) {
  t_node *tmp;

  if (list->root == NULL) {
    list->root = node;
  } else if (node->frequency < list->root->frequency) {
    node->next = list->root;
    list->root = node;
  } else {
    tmp = list->root;
    while (tmp->next && tmp->next->frequency <= node->frequency)
      tmp = tmp->next;
    node->next = tmp->next;
    tmp->next = node;
  }
  list->size++;
}

t_node *remove_front_node_list(t_list *list) {
  t_node *tmp;

  tmp = NULL;
  if (list->root) {
    tmp = list->root;
    list->root = tmp->next;
    tmp->next = NULL;
    list->size--;
  }

  return (tmp);
}

void fill_list(unsigned int *array, int size, t_list *list) {
  t_node *new_node;

  for (int i = 0; i < size; i++) {
    if (array[i] > 0) {
      new_node = create_node(i, array[i]);
      insert_node_list(list, new_node);
    }
  }
}

t_node *create_huffman_tree(t_list *list) {
  t_node *first;
  t_node *second;
  t_node *new;

  while (list->size > 1) {
    first = remove_front_node_list(list);
    second = remove_front_node_list(list);
    new = create_node(0, first->frequency + second->frequency);
    new->left = first;
    new->right = second;

    insert_node_list(list, new);
  }

  return (list->root);
}

int huffman_tree_height(t_node *root) {
  int left;
  int right;

  if (root == NULL) {
    return -1;
  } else {
    left = huffman_tree_height(root->left) + 1;
    right = huffman_tree_height(root->right) + 1;

    if (left > right) {
      return left;
    } else {
      return right;
    }
  }
}

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

char *decompress_str(char *compressed, t_node *root) {
  int i;
  t_node *tmp;
  char tmp_str[2] = {0};
  char *decompressed;

  tmp = root;
  decompressed = calloc(strlen(compressed), sizeof(char));
  i = 0;
  while (compressed[i]) {
    if (compressed[i] == '0')
      tmp = tmp->left;
    else
      tmp = tmp->right;

    if (tmp->left == NULL && tmp->right == NULL) {
      tmp_str[0] = tmp->character;
      strcat(decompressed, tmp_str);
      tmp = root;
    }
    i++;
  }
  return decompressed;
}

void print_frequency(unsigned int *array) {
  for (int i = 0; i < ASCII_HEIGHT; i++) {
    if (array[i] > 0) {
      printf("%d: %u\n", i, array[i]);
    }
  }
}

void print_list(t_list *list) {
  t_node *tmp;

  tmp = list->root;
  while (tmp) {
    printf("%c: %d\n", tmp->character, tmp->frequency);
    tmp = tmp->next;
  }
}

void print_huffman_tree(t_node *root, int height) {
  if (root->left == NULL && root->right == NULL) {
    printf("%c: f:%d h:%d\n", root->character, root->frequency, height);
  } else {
    print_huffman_tree(root->left, height + 1);
    print_huffman_tree(root->right, height + 1);
  }
}

void print_dictionary(char **dictionary) {
  for (int i = 0; i < ASCII_HEIGHT; i++) {
    if (strlen(dictionary[i])) {
      printf("%c: %s\n", i, dictionary[i]);
    }
  }
}
