#ifndef ENCODER
#define ENCODER

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define ASCII_HEIGHT 256

typedef struct s_node {
  unsigned char character;
  int frequency;
  struct s_node *left, *right, *next;
} t_node;

typedef struct s_list {
  t_node *root;
  int size;
} t_list;

typedef struct s_file {
  char file_name[1024];
  char compressed[20000];
  unsigned int ascii[ASCII_HEIGHT];
} t_file;

t_node *create_node(char character, int frequency);
void init_list(t_list *list);
void insert_node_list(t_list *list, t_node *node);
t_node *remove_front_node_list(t_list *list);
void fill_list(unsigned int *array, int size, t_list *list);
t_node *create_huffman_tree(t_list *list);
int huffman_tree_height(t_node *root);

void print_frequency(unsigned int *array);
void print_list(t_list *list);
void print_huffman_tree(t_node *root, int height);
void print_dictionary(char **dictionary);

char **alloc_dictionary(int columns);
void generate_dictionary(char **dictionary, t_node *root, char *path, int columns);

int get_string_lenght(char **dictionary, unsigned char *str);
char *compress_str(char **dictionary, unsigned char *str);
char *decompress_str(char *compress, t_node *root);

void free_dictionary(char **dictionary, int size);
void free_tree(t_node *root);

void read_file_frequency(char *file_name, unsigned int *ascii);
unsigned char *read_file(char *file_name);

void dup_dictionary(unsigned char (*dest)[8], char **src);

#endif
