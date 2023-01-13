#ifndef DECODER
#define DECODER

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
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

typedef struct s_comp_file {
  char compressed[20000];
  unsigned int ascii[ASCII_HEIGHT];
} t_comp_file;

typedef struct s_decomp_file {
  char decompressed[20000];
  int comp_bytes;
  int decomp_bytes;
  int ok;
  long time;
} t_decomp_file;

typedef struct s_comp_segment {
  key_t key;
  int shmid;
  t_comp_file *file;
} t_comp_segment;

typedef struct s_decomp_segment {
  key_t key;
  int shmid;
  t_decomp_file *file;
} t_decomp_segment;

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

long timestamp(void);

t_comp_segment get_compressed_file_segment(int id);
void set_decompressed_file_segment(char *decompressed,
                                   int c_size,
                                   int d_size,
                                   long time,
                                   int id);

#endif
