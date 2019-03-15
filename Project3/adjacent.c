#include <stdio.h>
#include <stdlib.h>

#define ERROR -1 // Generic error

// structures
typedef struct vert
{
  int x; // x coordination
  int y; // y coordination
  int dist; // current shortest distance when Dijkstra
  struct vert *from; // traceback
}vert;

int main (int Argc, char **Argv){
  if(Argc != 2){
    printf("wrong number of arguements");
    return ERROR;
  }
  int i = 0, j = 0, a, b;
  int vsize = 0;
  int esize = 0;
  char * Filename = Argv[1];
  FILE * inFile = fopen(Filename, "r");
//check open
  if (inFile == NULL){
    fprintf(stderr, "can't open the input file.\n");
    return ERROR;
  }
//read size
  int test = fscanf(inFile, "%d %d\n", &vsize, &esize);
  if (test != 2){
    fprintf(stderr, "Fail to read size.\n");
    return ERROR;
  }
  //fprintf(stderr, "%d %d\n", vsize, esize);

//malloc
  vert * vertices = malloc(sizeof(vert) * vsize);
  int ** edges = malloc(sizeof(int *) * vsize);
  for(i = 0; i <  vsize; i++){
    edges[i] = malloc(sizeof(int) * vsize);
  }
  //fprintf(stderr, "malloced\n");

//reset
  for(i = 0; i < vsize; i++){
    vertices[i].x = 0;
    vertices[i].y = 0;
    vertices[i].dist = -1;
    vertices[i].from = NULL;
  }

  for(i = 0; i < vsize; i++){
    for(j = 0; j < vsize; j++){
      edges[i][j] = 0;
    }
  }
  //fprintf(stderr, "reset done\n");
//scan number
  for(i = 0; i < vsize; i++){
    fscanf(inFile, "%d %d %d\n", &j, &vertices[i].x, &vertices[i].y);
    //fprintf(stderr, "%d %d\n", vertices[i].x, vertices[i].y);
    if(i != j){
      fprintf(stderr, "index error when reading vertices\n");
      return ERROR;
    }
  }
  //fprintf(stderr, "vertices read\n");

  for(i = 0;i < esize; i++){
    fscanf(inFile, "%d %d\n", &a,&b);
    edges[a][b] = 1;
    edges[b][a] = 1;
    //fprintf(stderr, "%d %d\n",a, b);
  }
  //fprintf(stderr, "edges read\n");
//close file
  fclose(inFile);

  for(i = 0; i < vsize; i++){
    fprintf(stderr, "%d:", i);
    for(j = 0; j < vsize; j++){
      if(edges[i][j] == 1){
	fprintf(stderr, " %d", j);
      }
    }
    fprintf(stderr, "\n");
  }

  //fprintf(stderr, "printed\n");

  free(vertices);
  for(i = 0; i < vsize; i++){
    free(edges[i]);
  }
  free(edges);

  return 0;
}
