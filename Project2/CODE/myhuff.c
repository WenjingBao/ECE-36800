#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

/* count the occurrences in a file */

long *countLetters(FILE *fp, char *content){
   long *asciiCount = (long *)malloc(sizeof(long)*ASCII_SIZE);
   if (asciiCount == NULL) {
      return NULL;
   }
   int ch;
   for (ch = 0; ch < ASCII_SIZE; ch++) {
      asciiCount[ch] = 0;
   }
   fseek(fp, 0, SEEK_SET);
   while ((ch = fgetc(fp)) != EOF) {
      asciiCount[ch] += 1;
   }
   int sz = ftell(fp);
   content = malloc(sizeof(char) * sz);
   rewind(fp);

   char c;
   for (ch = 0; ch < sz; ch++) {
      content[ch] = 0;
   }
   ch = 0;
   while ((c = fgetc(fp)) != EOF){
     content[ch] = c;
     ch++;
   }
   return asciiCount;
}

int main(int argc, char **argv){
  if (argc != 2) {
    printf("Not enough arguments");
    return EXIT_FAILURE;
  }
  FILE * inFile = fopen(argv[1], "r");
  if (inFile == NULL) {
    fprintf(stderr, "can't open the input file.  Quit.\n");
    return EXIT_FAILURE;
  }

  /* read and count the occurrences of characters and the content of it */
  char *content = NULL;
  long *asciiCount = countLetters(inFile, content);
  fclose(inFile);

  fprintf(stderr,"countcheck\n");

  if (asciiCount == NULL) {
    fprintf(stderr, "cannot allocate memory to count the characters in input file.  Quit.\n");
    return EXIT_FAILURE;
  }

  //build linked list
  ListNode *head = buildList(asciiCount);

  /* //output1 */
  /* ListNode *cur = head; */
  /* FILE *outFile = fopen(argv[2], "w"); */
  /* while(cur != NULL){ */
  /*   fprintf(outFile,"%c:%ld\n", cur -> ptr -> label, cur -> ptr -> count); */
  /*   cur = cur -> next; */
  /* } */
  /* fclose(outFile); */

  //build tree
  TreeNode *root = buildTree(head);
  fprintf(stderr,"treecheck\n");
  /* //output2 */
  /* outFile = fopen(argv[3], "w"); */
  /* char *route = malloc(sizeof(char) * 256); */
  /* printTree(root ,route, 0, outFile); */
  /* free(route); */
  /* fclose(outFile); */

  //huffman encoding
  char *out = argv[1];
  strcat(out, ".huff");

  fprintf(stderr, "%s\n", out);

  FILE * outFile = fopen(out, "w");
  int *huff = malloc(sizeof(int) * 2560); //will have 256 leaf nodes at most
  int *byte = malloc(sizeof(int) * 8);
  char *header = malloc(sizeof(char) * 320);
  printPre(root, huff, byte, 0, 0);
  intobits(huff, header);
  fprintf(outFile, "%s", header);


  fclose(outFile);
  free(content);
  free(huff);
  free(byte);
  free(out);
  freeTree(root);
  free(asciiCount);

  return EXIT_SUCCESS;
}

ListNode *buildNode(int a, long count){
  TreeNode *ptr = malloc(sizeof(TreeNode));
  ptr -> label = a;
  ptr -> count = count;
  ptr -> left = NULL;
  ptr -> right = NULL;

  ListNode *result = malloc(sizeof(ListNode));
  result -> ptr = ptr;
  result -> next = NULL;

  return result;
}

ListNode *buildList(long *a){
  int i;
  ListNode *head = NULL;
  ListNode *new;
  for(i = 0; i < 256; i++){
    if (a[i] != 0){
      new = buildNode(i, a[i]);
      head = insertNode(head, new);
    }
  }
  return head;
}

TreeNode *buildTree(ListNode *head){
  ListNode *new;
  ListNode *first;
  ListNode *second;
  TreeNode *root;

  while(head -> next != NULL){
    new = buildNode(256, head -> ptr -> count + head -> next -> ptr -> count);
    new -> ptr -> left = head -> ptr;
    new -> ptr -> right = head -> next -> ptr;
    head = insertNode(head, new);
    first = head;
    second = head -> next;
    head = head -> next -> next;
    free(first);
    free(second);
  }
  root = head -> ptr;
  free(head);

  return root;
}

ListNode *insertNode(ListNode *head, ListNode *new){
  ListNode *cmpr;
  ListNode *prev = NULL;
  cmpr = head;
  while(new -> next == NULL && cmpr != NULL){
    //check frequency 
    if (cmpr -> ptr -> count < new -> ptr -> count){
      prev = cmpr;
      cmpr = cmpr -> next;
    } else if (cmpr -> ptr -> count > new -> ptr -> count){
      if (prev == NULL){
	head = new;
	new -> next = cmpr;
      } else {
	prev -> next = new;
	new -> next = cmpr;
      }
    } else {
      //check ASCII
      if (cmpr -> ptr -> label <= new -> ptr -> label){
	prev = cmpr;
	cmpr = cmpr -> next;
      } else if (cmpr -> ptr -> label > new -> ptr -> label){
	if (prev == NULL){
	  head = new;
	  new -> next = cmpr;
  	} else {
	  prev -> next = new;
	  new -> next = cmpr;
	}
      }
    }
  }
  //if new Node should be behind the last one or the list is empty
  if (cmpr == NULL){
    if (prev == NULL){
      head = new;
    } else {
      prev -> next = new;
    }
  }
  return head;
}

/* void printTree(TreeNode *tree, char *route, int i, FILE *outFile){ */
/*   if(tree -> label != 256){ */
/*     fprintf(outFile, "%c:", tree -> label); */
/*     route[i] = '\0'; */
/*     int j = 0; */
/*     while(route[j] != '\0'){ */
/*       fprintf(outFile, "%c", route[j]); */
/*       j++; */
/*     } */
/*     fprintf(outFile, "\n"); */
/*     return; */
/*   } */

/*   route[i] = '0'; */
/*   printTree(tree -> left, route, i + 1, outFile); */
/*   route[i] = '1'; */
/*   printTree(tree -> right, route, i + 1, outFile); */

/*   return; */
/* } */

void printPre(TreeNode *tree, int *huff, int *byte, int i, int l){
  if(tree -> label != 256){
    huff[i] = 1;
    i++;
    convert(tree -> label, byte);
    for(int j = 0; j < 8; j++){
      huff[i] = byte[j];
      i++;
    }
    return;
  }
  huff[i] = 0;
  i++;
  printPre(tree -> left, huff, byte, i , l + 1);
  printPre(tree -> right, huff, byte, i, l + 1);
  if(l == 0){
    huff[i] = -1;
  }
  return;
}
  
void freeTree(TreeNode *tree){
  if(tree -> left == NULL && tree -> right == NULL){
    free(tree);
    return;
  }

  if(tree -> left != NULL){
    freeTree(tree -> left);
  }
  if(tree -> right != NULL){
    freeTree(tree -> right);
  }
  free(tree);

  return;
}

void convert(int c, int *byte){
  int a = c, i = 0;
  for(i = 0; i < 8; i++){
    a -= (2 ^ (7 - i));
    if(a >= 0){
      byte[i] = 1;
    }else{
      byte[i] = 0;
      a += (2 ^ (7 - i));
    }
  }
}

char *intobits(int *huff, char *header){
  int i = 0, cha = 0,j = 1, k = 0;
  while(j == 1){
    for(i = 0; i < 8; i++){
      if(huff[i] == -1){
	j = 0;
      }
      cha += j * huff[i] * 2^(7-i);
    }
    header[k] = (char)cha;
    k++;
    cha = 0;
  }
  return header;
}
