#include "encoder.h"

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
