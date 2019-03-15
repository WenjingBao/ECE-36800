
#define ASCII_SIZE 256


typedef struct TreeNode {
   int label;
   long count;
   struct TreeNode *left;
   struct TreeNode *right;
} TreeNode;

typedef struct ListNode {
   TreeNode *ptr;
   struct ListNode *next;
} ListNode;

ListNode *insertNode(ListNode *head, ListNode *new);
ListNode *builNode(int a, long count);
void freeTree(TreeNode * tree);
/* void freeTable(char **table); */
ListNode *buildList(long *a);
TreeNode *buildTree(ListNode *head);
/* void printTree(TreeNode *tree, char *route, int i, FILE *outFile); */
void printPre(TreeNode *tree, int *huff, int * byte, int i, int l);
long *countLetters(FILE *fp, char *content);
char *intobits(int *huff, char *header);
void convert(int c, int *byte);
