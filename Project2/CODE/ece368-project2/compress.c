#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "compress.h"

FILE* open_file(int argc, char* path)
{
	if (argc == 1) // no command line argument.
		return NULL;

	FILE* file = fopen(path, "r"); // open input file.

	if (file == NULL) // can not open.
		return NULL;

	return file;
}

int count_times(FILE* file, int* count)
{
	int bytes = 0;
	char rd;

	while (1)
	{
		if (EOF == fscanf(file, "%1c", &rd))
			break ;

		bytes++;
		count[(unsigned)rd]++;
	}
	return bytes;
}

void empty_file(char* arg)
{
	char path[20];
	strcpy(path, "");
	strcat(path, arg);
	strcat(path, ".huff");

	fopen(path, "w");
}

void create_leaf(info* base, int* count)
{
	int i = 0;
	for (i = 0; i < 256; i++)
	{
		if (count[i] == 0)
			continue ;

		// allocate one leaf node and initialize it, then put it to leaf node array.
		h_node* node = (h_node*)malloc(sizeof(h_node));
		node->id = i;
		node->count = count[i];
		strcpy(node->code, "");
		node->parent = NULL;
		node->left = NULL;
		node->right = NULL;
		base->leaf_node[base->size++] = node;
	}

	// put pseudo-EOF to leaf node array.
	h_node* node = (h_node*)malloc(sizeof(h_node));
	node->id = 256; // pseudo-EOF character
	node->count = 1;
	strcpy(node->code, "");
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	base->leaf_node[base->size++] = node;
}

void create_tree(info* base)
{
	while (!empty(base->leaf_node, base->size))
	{
		// first leaf node
		int index = top(base->leaf_node, base->size);
		h_node* first = base->leaf_node[index];
		pop(base->leaf_node, index);

		// only one leaf node => root node
		if (empty(base->leaf_node, base->size))
		{
			base->root = first;
			break;
		}

		// second leaf node
		index = top(base->leaf_node, base->size);
		h_node* second = base->leaf_node[index];
		pop(base->leaf_node, index);

		// new internal node
		h_node* inter_node = (h_node*)malloc(sizeof(h_node));
		inter_node->count = first->count + second->count;
		inter_node->left = inter_node->right = inter_node->parent = NULL;
		strcpy(inter_node->code, "");

		// set internal node's left child node and right child node.
		if (first->count <= second->count)
		{
			inter_node->left = first;
			inter_node->right = second;
		}
		else
		{
			inter_node->left = second;
			inter_node->right = first;
		}
		first->parent = second->parent = inter_node;

		// push new node to node array.
		push(inter_node, base->leaf_node, base->size);
	}
}

void create_table(h_node* node, bool left, table* tb)
{
	if (left)
	{
		char tmp[20];
		strcpy(tmp, "");
		strcat(tmp, node->parent->code);
		strcat(tmp, "0");
		strcat(node->code, tmp);
	}
	else
	{
		char tmp[20];
		strcpy(tmp, "");
		strcat(tmp, node->parent->code);
		strcat(tmp, "1");
		strcat(node->code, tmp);
	}

	static int count = 0;
	if (node->left == NULL && node->right == NULL)
	{
		tb[count].code = node->code;
		tb[count].id = node->id;
		count++;
	}
	else
	{
		if (node->left != NULL)
			create_table(node->left, true, tb);
		if (node->right != NULL)
			create_table(node->right, false, tb);
	}
}

void get_huffman_code(info* base, table* tb)
{
	if (base->root->left != NULL)
		create_table(base->root->left, true, tb);

	if (base->root->right != NULL)
		create_table(base->root->right, false, tb);
}

void compress(FILE* file, char* path, table* tb, info* base)
{
	// compressed file name.
	char outpath[20];
	strcpy(outpath, "");
	strcat(outpath, path);
	strcat(outpath, ".huff");

	// create new compressed file.
	FILE* file2 = fopen(outpath, "w");

	// output the number of leaf nodes to compressed file.
	char str[33];
	sprintf(str, "%d", base->size);
	strcat(str, "\n");
	fprintf(file2, str);

	// output character's ascii code and huffman code to compressed file.
	int i = 0; 
	for (i = 0; i < base->size; i++)
	{
		char str[4];
		sprintf(str, "%d", tb[i].id);

		fprintf(file2, str);

		char tmp[30];
		strcpy(tmp, " ");
		strcat(tmp, tb[i].code);
		strcat(tmp, "\n");

		fprintf(file2, tmp);
	}

	fseek(file, 0, SEEK_SET);
	char buf[60];
	strcpy(buf, "");

	// read one character from input file every time.
	while (1) 
	{
		char rc;
		if (EOF == fscanf(file, "%1c", &rc))
			break;

		char* str = NULL;
		str = find((unsigned char)rc, base->size, tb);
		if (str == NULL)
			fprintf(stderr, "error in find character in table\n");

		strcat(buf, str);

		// output compressed content to compressed file every 8 bit.
		if (strlen(buf) >= 8)
		{
			int j = 0;
			for (j = 0; j + 7 < strlen(buf); j += 8)
			{
				unsigned char out = 0;

				int k = 0;
				for (k = 0; k < 8; k++)
				{
					out = out << 1;
					if (buf[j + k] == '0')
						out += 0;
					else
						out += 1;
				}
				if (1 != fwrite(&out, 1, 1, file2))
					fprintf(stderr, "error to output compressed content to compressed file\n");
			}

			// the last bits.
			char tmp[10];
			strcpy(tmp, "");
			strncat(tmp, buf + j, strlen(buf) - j);
			strcpy(buf, tmp);
		}
	}

	// output last bits and pseudo-EOF to compressed file.
	char* s = find(256, base->size, tb);
	strcat(buf, s);

	int length = strlen(buf);
	unsigned char out = 0;

	int m = 0;
	for (m = 0; m < length; m++)
	{
		out = out << 1;
		if (buf[m] == '0')
			out += 0;
		else
			out += 1;
		//if (((i + 1) % 8 == 0) || (i == length - 1))
		if ((m + 1) % 8 == 0) 
		{
			fwrite(&out, 1, 1, file2);
			out = 0;
		}

		if (m == length - 1)
			out = out << (8 - (m + 1));
	}
	fclose(file2);
}

char* find(int ch, int size, table* tb)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		if (tb[i].id == ch)
			return tb[i].code;
	}
	return NULL;
}

int empty(h_node** arr, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		if (arr[i] != NULL)
			return 0;
	}
	return 1;
}

int top(h_node** arr, int size)
{
	int min;
	bool flag = true;

	int i = 0;
	for (i = 0; i < size; i++)
	{
		if (flag && arr[i] != NULL)
		{
			min = i;
			flag = false;
		}

		if (arr[i] != NULL)
		{
			if (arr[i]->count < arr[min]->count)
				min = i;
		}
	}
	return min;
}

void pop(h_node** arr, int index)
{
	arr[index] = NULL;
}

void push(h_node* node, h_node** arr, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		if (arr[i] == NULL)
		{
			arr[i] = node;
			break;
		}
	}
}

void rebuild_tree(h_node* root, FILE* file)
{
	// get the num of leaf nodes.
	char num[10];
	fgets(num, 10, file);
	int size = atoi(num);

	h_node* node;
	h_node* tmp_node;

	int i = 0;
	for (i = 0; i < size; i++)
	{
		// get ascii code and huffman code.
		char buf[40];
		fgets(buf, 40, file);

		char id[6];
		char code[30];
		strcpy(id, "");
		strcpy(code, "");

		int j = 0;
		for (j = 0; j < strlen(buf); j++)
		{
			if (buf[j] == ' ')
				break ;
		}
		strncpy(id, buf, j);
		id[j] = '\0';
		strncpy(code, buf + j + 1, strlen(buf) - (j + 2));
		code[strlen(buf) - (j + 2)] = '\0';

		int node_id = atoi(id);

		// put leaf node to its postion in huffman tree by its huffman code
		int length = strlen(code);
		node = root;

		int k = 0;
		for (k = 0; k < length; k++)
		{
			if (code[k] == '0')
				tmp_node = node->left;
			else if (code[k] == '1')
				tmp_node = node->right;
			else
				fprintf(stderr, "rebuild tree error\n");

			// empty, create a new node
			if (tmp_node == NULL)
			{
				h_node* new_node = (h_node*)malloc(sizeof(h_node));
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->parent = node;

				// arrive to leaf node
				if (k == length - 1)
				{
					new_node->id = node_id;
					strcpy(new_node->code, code);
				}

				if (code[k] == '0')
					node->left = new_node;
				else
					node->right = new_node;

				tmp_node = new_node;
			}

			node = tmp_node;
		}
	}
}

void decompress(h_node* root, char* path, FILE* file)
{
	char out[20];
	strcpy(out, "");
	strcat(out, path);
	strcat(out, ".unhuff");

	FILE* file2 = fopen(out, "w");

	h_node* node = root;
	char c;
	int eof = 1;

	while (eof && (fread(&c, 1, 1, file) == 1))
	{
		unsigned char uc = (unsigned char)c;
		unsigned flag = 0x80;

		int i = 0;
		for (i = 0; i < 8; i++)
		{
			if (uc & flag)
				node = node->right;
			else
				node = node->left;

			// arrive at leaf node
			if (node->left == NULL && node->right == NULL)
			{
				if (node->id == 256) // arrive at pseudo-EOF
				{
					eof = 0;
					break;
				}
				else
				{
					fprintf(file2, "%c", node->id);
					node = root;
				}
			}

			flag = flag >> 1;
		}
	}
	fclose(file2);
}
