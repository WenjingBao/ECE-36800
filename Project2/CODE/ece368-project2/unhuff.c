#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "compress.h"

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
	if (argc == 1)
		return 0;

	FILE* file = fopen(argv[1], "r");
	if (file == NULL)
	{
		fprintf(stderr, "can not open compressed file\n");
		return 0;
	}

	// create root node for huffman tree.
	h_node* root = (h_node*)malloc(sizeof(h_node));
	root->left = NULL;
	root->right = NULL;
	root->parent = NULL;

	rebuild_tree(root, file);

	decompress(root, argv[1], file);

	fclose(file);

	prefree(root);

	return 0;
}

