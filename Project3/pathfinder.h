#define ERROR            ( -1 ) // Generic error

#include <stdlib.h>
#include <stdio.h>

// structures
typedef struct vert
{
  int x; // x coordination
  int y; // y coordination
  int dist; // current shortest distance when Dijkstra
  int from;
  int out; //whether the vertex is checked
}vert;

/* Function Prototypes */
void readmap(char *Filename, int **edges, vert * vertices, int *vsize, int *esize);
void readque(char *Filename, int **edges, vert * vertices, int vsize, int esize);
void ms2(int **edges, int vsize, int esize);
void dijk(int **edges, vert * vertices, int start, int end, int vsize, int esize);
int mindist(vert *vertices, int vsize);
