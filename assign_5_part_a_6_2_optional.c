#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 11
#define INFI 9999

struct City
{
  int vertex;
  int distance;
  int root;
};

int findMinQ(struct City cityQ[MAX_VERTEX]);
int printQ(struct City cityQ[MAX_VERTEX]);
void printPath(int rootPath[], int dest);

int findMinQ(struct City cityQ[MAX_VERTEX])
{
  int i = 0;
  int min = INFI;
  int retCity = INFI;

  for (i = 0; i < MAX_VERTEX * MAX_VERTEX; i++)
  {
    if (cityQ[i].distance != INFI)
    {
      // printf("min: %d & Dist- %d\n", min, cityQ[i].distance);
      if (min > cityQ[i].distance)
      {
        min = cityQ[i].distance;
        retCity = i;
      }
    }
  }

  printf("Returning retCity from Q=%d\n", retCity);
  return retCity;
}

int printQ(struct City cityQ[MAX_VERTEX])
{
  printf("City Q: [");
  for (int i = 0; i < MAX_VERTEX * MAX_VERTEX; i++)
  {
    if (cityQ[i].distance != INFI)
    {
      printf("(index=%d, vertex=%d, root=%d, dist=%d), ", i, cityQ[i].vertex, cityQ[i].root, cityQ[i].distance);
    }
  }
  printf("]\n");

  return 0;
}

void printPath(int rootPath[], int dest)
{
  if (rootPath[dest] == 0)
  {
    printf("%d->", dest);
    return;
  }

  printPath(rootPath, rootPath[dest]);

  printf("%d->", dest);
}

int main (int argc, char *argv[])
{
  int graph[MAX_VERTEX][MAX_VERTEX] = {\
                                      {0, 5, INFI, INFI, 12, INFI, 21, INFI, 15, 1, INFI},
                                      {5, 0, 9, INFI, INFI, INFI, 18, INFI, INFI, 20, INFI},\
                                      {INFI, 9, 0, 16, INFI, INFI, 17, INFI, INFI, INFI, 8},\
                                      {INFI, INFI, 16, 0, INFI, 7, 11, 14, INFI, INFI, INFI},\
                                      {12, INFI, INFI, INFI, 0, 6, 2, INFI, 10, INFI, INFI},\
                                      {INFI, INFI, INFI, 7, 6, 0, INFI, 4, INFI, 19, 13},\
                                      {21, 18, 17, 11, 2, INFI, 0, 3, INFI, INFI, INFI},\
                                      {INFI, INFI, INFI, 14, INFI, 4, 3, 0, INFI, INFI, INFI},\
                                      {15, INFI, INFI, INFI, 10, INFI, INFI, INFI, 0, INFI, INFI},\
                                      {1, 20, INFI, INFI, INFI, 19, INFI, INFI, INFI, 0, INFI},\
                                      {INFI, INFI, 8, INFI, INFI, 13, INFI, INFI, INFI, INFI, 0}};

  int D[MAX_VERTEX] = {INFI};         // Resulting shortest dist a to each city, default all to infinite
  struct City cityQ[MAX_VERTEX * MAX_VERTEX];
  int i = 0, j = 0, k = 0, z = 0;
  int qCity=0;
  int rootPath[MAX_VERTEX] = {INFI};
  int finalizedCity[MAX_VERTEX];      // 0 means pending and 1 means finalized distance

  for (i = 0; i < MAX_VERTEX * MAX_VERTEX; i++)   // Defult everything to INFI
  {
    cityQ[i].vertex = INFI;
    cityQ[i].distance = INFI;
    cityQ[i].root = INFI;
  }

  for (i = 0; i < MAX_VERTEX; i++)   // Defult everything to INFI
  {
    D[i] = INFI;
    finalizedCity[i] = 0;
  }

  D[0] = 0;       // City a's distance to itself is 0
  finalizedCity[0] = 1; // City a is finalized already
  rootPath[0] = 0;
  k = 0;          // Our Q

  i = 0;
  while (1)           // Untill we add all the vertex
  {
    // find adjecent
    for (j = 0; j < MAX_VERTEX; j++)
    {
      // printf("Adding: finalizedCity[%d]=%d\n", j, finalizedCity[j]);
      if ((graph[i][j] != INFI) && (i != j) && (j != 0) && (finalizedCity[j] != 1))
      {
        cityQ[k].vertex = j;
        cityQ[k].distance = graph[i][j];
        cityQ[k++].root = i;
        printf("\n--->Added to Q Root=%d Vertex=%d Dis=%d\n", i, j, graph[i][j]);
      }
    }
    printQ(&cityQ[0]);  // Print current Q

    qCity = findMinQ(&cityQ[0]);      // Vertex who has min distance in adjecent

    if (qCity == INFI)  // PQ empty now
    {
      break;
    }

    printf("Working on Vertex: %d\n", cityQ[qCity].vertex);
    printf("D[%d]=> Current Dist: %d, Possible distance:%d\n", cityQ[qCity].vertex, D[cityQ[qCity].vertex], (D[cityQ[qCity].root] + graph[cityQ[qCity].root][cityQ[qCity].vertex]));
    if (D[cityQ[qCity].vertex] > (D[cityQ[qCity].root] + graph[cityQ[qCity].root][cityQ[qCity].vertex]))
    {
      D[cityQ[qCity].vertex] = D[cityQ[qCity].root] + graph[cityQ[qCity].root][cityQ[qCity].vertex];
      rootPath[cityQ[qCity].vertex] = cityQ[qCity].root;
      finalizedCity[cityQ[qCity].vertex] = 1;   // City's shortest distance finalized
    }

    i = cityQ[qCity].vertex;

    printf("DONE: i=%d D[i]=%d rootPath[i]=%d\n", cityQ[qCity].vertex, D[cityQ[qCity].vertex], rootPath[cityQ[qCity].vertex]);
    cityQ[qCity].vertex = INFI;       // Reset the one we know is done
    cityQ[qCity].distance = INFI;
    cityQ[qCity].root = INFI;
    z++;
  }

  for (i = 0; i < MAX_VERTEX; i++)
  {
    printf("*************************************\n");
    printf("Min distance- %d - %d\n", i, D[i]);
    printf("Path of %d: \n", i);
    // printPath(rootPath, i);
    printf("\n");
  }

  printf("PATH:\n");
  for (i = 0; i < MAX_VERTEX; i++)
  {
    printf("(%d = %d): ", i, rootPath[i]);
  }
  printf("\n");
  return 0;
}
