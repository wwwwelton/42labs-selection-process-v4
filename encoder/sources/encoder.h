#ifndef ENCODER
#define ENCODER

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>

#define DEBUG 0

#define ARRAY_SIZE(x) (x / 8 + (!!(x % 8)))
#define ASCII_HEIGHT 256

#define FILE_NAME "/tmp/7df0df64fecb85c3f0ac08f34d6.c"

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
  size_t size;
  unsigned int ascii[ASCII_HEIGHT];
  char compressed[];
} t_comp_file;

typedef struct s_decomp_file {
  int comp_bytes;
  int decomp_bytes;
  int ok;
  long time;
  char decompressed[];
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

typedef struct s_data {
  unsigned int ascii[ASCII_HEIGHT];
  t_list list;
  t_node *root;
  int columns;
  char **dictionary;
  char *compressed;
  t_decomp_segment segment;
  size_t size;
} t_data;

void init_data(t_data *data);
void clean_data(t_data *data, char *error);

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
void print_info(t_data *data);

char **alloc_dictionary(int columns);
void generate_dictionary(char **dictionary, t_node *root, char *path, int columns);

int get_string_lenght(int argc, char **argv, char **dictionary);
char *compress_str(int argc, char **argv, char **dictionary, size_t *size);

void free_dictionary(char **dictionary, int size);
void free_tree(t_node *root);

void read_files_frequency(int argc, char **argv, unsigned int *ascii);

void dup_dictionary(unsigned char (*dest)[8], char **src);

long timestamp(void);

t_decomp_segment get_decompressed_file_segment(int id);
void set_compressed_file_segment(char *compressed,
                                 unsigned int *ascii,
                                 size_t size,
                                 int id);
void set_compressed_file_segment_size(size_t size, int id);

char get_bit(char *array, int index);
void toggle_bit(char *array, int index);

void check_args(int argc, char **argv);

#endif
