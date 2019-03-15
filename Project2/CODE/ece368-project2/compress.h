#ifndef _COMPRESS_H_
#define _COMPRESS_H_

// 256 ascii character + 1 pseudo-EOF
#define MAX_LEAF 257

#include <stdbool.h>
#include <stdio.h>

// leaf node.
typedef struct h_node
{
	int id; // ASCII code. pseudo-EOF is 256
	int count; // times of character 
	char code[30]; // Huffman code
	struct h_node* parent;
	struct h_node* left;
	struct h_node* right;
}h_node;

// some global information.
typedef struct info
{
	h_node* leaf_node[MAX_LEAF];
	int size;
	h_node* root;
}info;

// map table, from character ascii code to huffman code.
typedef struct table
{
	int id;
	char* code;
}table;

// compression.

// open input file.
FILE* open_file(int argc, char* path);

// count the number of times every character occurs.
int count_times(FILE* file, int* count);

// deal with empty file.
void empty_file(char* arg);

// use character and its times to create leaf node.
void create_leaf(info* base, int* count);

// use leaf nodes to create Huffman tree.
void create_tree(info* base);

// create map table.
void create_table(h_node* node, bool left, table* tb);

// get character's huffman code.
void get_huffman_code(info* base, table* tb);

// compress file from comand line and output compressed file
void compress(FILE* file, char* path, table* tb, info* base);

// find huffman code from map table by ascii code.
char* find(int ch, int size, table* tb);

// internal functions :

int empty(h_node** arr, int size);

int top(h_node** arr, int size);

void pop(h_node** arr, int index);

void push(h_node* node, h_node** arr, int size);


// decompression

// rebuild huffman tree from compressed file.
void rebuild_tree(h_node* root, FILE* file);

// decompression from compressed file use huffman tree.
void decompress(h_node* root, char* path, FILE* file);

#endif // _COMPRESS_H_
