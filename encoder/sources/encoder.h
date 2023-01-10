#ifndef ENCODER
#define ENCODER

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_node {
  int character;
  int frequency;
  struct s_node *left, *right, *next;
} t_node;

typedef struct s_list {
  t_node *root;
  int size;
} t_list;

void sort_array(int *array, int size);

t_node *create_node(char character, int frequency);
void init_list(t_list *list);
void insert_node_list(t_list *list, t_node *node);
t_node *remove_front_node_list(t_list *list);
void fill_list(int *array, int size, t_list *list);
t_node *create_huffman_tree(t_list *list);
int huffman_tree_height(t_node *root);

void print_list(t_list *list);
void print_huffman_tree(t_node *root, int height);
void print_dictionary(char **dictionary);

char **alloc_dictionary(int columns);
void generate_dictionary(char **dictionary, t_node *root, char *path, int columns);

int get_string_lenght(char **dictionary, char *str);
char *compress_str(char **dictionary, char *str);

void free_dictionary(char **dictionary, int size);
void free_tree(t_node *root);

#endif
