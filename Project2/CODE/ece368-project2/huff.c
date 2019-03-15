#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "compress.h"

#define MAX_LEAF 257

void prefree(h_node* root)
{
	if (root == NULL)
		return ;
	prefree(root->left);
	prefree(root->right);
	free(root);
}

int main(int argc, char* argv[])
{
	// open input file from command line.
	FILE* file = open_file(argc, argv[1]);
	if (file == NULL) // can not open input file.
	{
		fprintf(stderr, "there is not a input file or can not open input file\n");
		return 0;
	}

	// store the number of times every character occurs.
	int count[256]; 
	memset(count, 0, 256 * sizeof(int));

	int total = count_times(file, count);
	if (total == 0)
	{
		empty_file(argv[1]);
		return 0;
	}

	// create leaf nodes use character and its times.
	info base;
	base.size = 0;
	base.root = NULL;

	create_leaf(&base, count);

	// create Huffman tree
	create_tree(&base);

	table* tb = (table*)malloc(base.size * sizeof(table));

	// get Huffman code
	get_huffman_code(&base, tb);

	// compress file
	compress(file, argv[1], tb, &base);

	int i = 0;
	for (i = 0; i < base.size; i++)
		free(base.leaf_node[i]);

	prefree(base.root);

	free(tb);

	fclose(file);

   	return 0; 
}
