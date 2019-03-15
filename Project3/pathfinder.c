#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "pathfinder.h"

int main(int argc, char **argv){
//check inputs
  if (argc != 3) {
    printf("Not enough arguments");
    return EXIT_FAILURE;
  }

//readmap
  int j = 0, m, n, vsize, esize, i;
  FILE * inFile = fopen(argv[1], "r");
//check open
  if (inFile == NULL){
    fprintf(stderr, "can't open the input file.\n");
    return EXIT_FAILURE;
  }
//read size
  int test = fscanf(inFile, "%d %d\n", &vsize, &esize);
  if (test != 2){
    fprintf(stderr, "Fail to read size.\n");
    return EXIT_FAILURE;
  }

//malloc
  vert *vertices = malloc(sizeof(vert) * vsize);

  int **edges = malloc(sizeof(int*) * vsize); 
  for(i = 0; i < vsize; i++){
    edges[i] = malloc(sizeof(int) * vsize);
  }
  
//reset
  for(i = 0; i < vsize; i++){
    vertices[i].x = 0;
    vertices[i].y = 0;
    vertices[i].dist = -1;
    vertices[i].from = -1;
    vertices[i].out = 0;
  }

  for(i = 0; i < vsize; i++){
    for(j = 0; j < vsize; j++){
      edges[i][j] = 0;
    }
  }

//scan number
  for(i = 0; i < vsize; i++){
    fscanf(inFile, "%d %d %d\n", &j, &vertices[i].x, &vertices[i].y);
    if(i != j){
      fprintf(stderr, "index error when reading vertices\n");
      return EXIT_FAILURE;
    }
  }

  int diff_x, diff_y;
  for(i = 0;i < esize; i++){
    fscanf(inFile, "%d %d\n", &m, &n);

    diff_x = abs(vertices[m].x - vertices[n].x);
    diff_y = abs(vertices[m].y - vertices[n].y);
    edges[m][n] = (int)sqrt(pow(diff_x,2) + pow(diff_y,2));
    edges[n][m] = edges[m][n];
  }

//close file
  fclose(inFile);

  //fprintf(stderr,"vsize:%d\nesize:%d\n", vsize, esize);
  fprintf(stderr,"x:%d\ny:%d\nedge 0 to 1: %d\n",vertices[17].x,vertices[17].y,edges[17][18]);
  //fprintf(stderr,"%s\n",argv[2]);
//readque
  readque(argv[2], edges, vertices, vsize, esize);

  free(vertices);
  for(i = 0; i < vsize; i++){
    free(edges[i]);
  }
  free(edges);

  return 0;
}


//void readmap(char *Filename, int **edges, vert * vertices, int *vsize, int *esize){

//  return;
//}

void ms2(int *edges[3], int vsize, int esize){
  int i = 0, j = 0, k = 0;
  for(i = 0; i < vsize; i++){
    printf("%d: ",i);
    for(j = 0; j < esize; j++){
      for(k = 0; k < 2; k++){
	if(edges[k][j] == i){
	  printf("%d ", edges[k][j]);
	}
      }
    }
    printf("\n");
  }
}

void readque(char *Filename, int **edges, vert * vertices, int vsize, int esize){
  int i = 0, j = 0, qsize;
  FILE * inFile = fopen(Filename, "r");
//check open
  if (inFile == NULL){
    fprintf(stderr, "can't open the input file.\n");
    return;
  }
//read size
  int test = fscanf(inFile, "%d\n", &qsize);
  if (test != 1){
    fprintf(stderr, "Fail to read size.\n");
    return;
  }

//malloc
  int **que = malloc(sizeof(int *) * 2);
  for(i = 0; i < 2; i++){
    que[i] = malloc(sizeof(int) * qsize);
  }

  for(i = 0; i < qsize; i++){
    test = fscanf(inFile, "%d %d\n", &que[0][i],&que[1][i]);
    if (test != 2){
      fprintf(stderr, "Fail to read query.\n");
      return;
    }
  }

  //fprintf(stderr, "%d,%d\n",que[0][0],que[1][0]);

  for(i = 0; i < qsize; i++){
    dijk(edges, vertices, que[0][i], que[1][i], vsize, esize);
    for(j = 0; j < vsize; j++){
      vertices[j].dist = -1;
      vertices[j].out = 0;
      vertices[j].from = -1;
    }
  }

  free(que[0]);
  free(que[1]);
  free(que);
  fclose(inFile);

  return;
}

void dijk(int **edges, vert * vertices, int start, int end, int vsize, int esize){
  int i, j, min;
  int *path = malloc(sizeof(int) * esize);

  vertices[start].dist = 0;
  for(i = 0; i < vsize - 1; i++){
    min  = mindist(vertices, vsize);

    //fprintf(stderr, "min : %d\n", min);

    for(j = 0; j < vsize; j++){
      if(vertices[j].out == 0 && edges[min][j] && (vertices[j].dist == -1 || vertices[min].dist + edges[min][j] < vertices[j].dist)){
	vertices[j].dist = vertices[min].dist + edges[min][j];
	vertices[j].from = min; 
      }
    }

    vertices[min].out = 1;

    if(vertices[end].out == 1){
      break;
      }
  }

  i = end;
  j = 0;
  while(i != start){
    path[j] = i;
    i = vertices[i].from;
    j++;
  }

  int temp;
  int a = 0;
  int b = j;
    while (a < b)
    {
        temp = path[a];   
        path[a] = path[b];
        path[b] = temp;
        a++;
        b--;
    }

  printf("%d\n\n", vertices[end].dist);
  for(i = 0; i < j+1; i++){
    printf("%d ", path[i]);
  }
  printf("\n\n");

  free(path);

  return;
}

int mindist(vert *vertices, int vsize){
  int i;
  int min = -1;

  for(i = 0; i < vsize; i++){
    if(vertices[i].out != 1 && vertices[i].dist != -1 && (min == -1 || vertices[min].dist > vertices[i].dist)){
      min  = i;
    }
  }

  return min;
}