#include <stdio.h>
#include <stdlib.h>

#define SOURCE_CITY 3             // NOTE: Modify this to have new source city
#define DESTINATION_CITY 8        // NOTE: Modify this to have new destination city

#define MAX_VERTEX 11
#define INFI 9999
#define ENABLE_DEBUG 0  // 0 to disable debugs, 1 to enable debugs

struct City             // Queue logic
{
  int vertex;
  int distance;
  int root;
};

struct CityName         // To store alpha name of city
{
  char name;
};

int findMinQ(struct City cityQ[MAX_VERTEX]);          // Finds current minimum distance from Q
int printQ(struct City cityQ[MAX_VERTEX]);            // Print current Q
void printCityPath(struct CityName cityname[MAX_VERTEX], int rootPath[], int dest, int invert);   // invert = 0 means Src to 0 and invert = 1 means 0 to src

int findMinQ(struct City cityQ[MAX_VERTEX])
{
  int i = 0;
  int min = INFI;
  int retCity = INFI;

  for (i = 0; i < MAX_VERTEX * MAX_VERTEX; i++)       // Check whole Q and ignore find min
  {
    if (cityQ[i].distance != INFI)
    {
      if (min > cityQ[i].distance)
      {
        min = cityQ[i].distance;
        retCity = i;
      }
    }
  }

  return retCity;
}

int printQ(struct City cityQ[MAX_VERTEX])
{
  printf("City Q: [");
  for (int i = 0; i < MAX_VERTEX * MAX_VERTEX; i++)
  {
    if (cityQ[i].distance != INFI)                      // Print only valid entries
    {
      printf("(index=%d, vertex=%d, root=%d, dist=%d), ", i, cityQ[i].vertex, cityQ[i].root, cityQ[i].distance);
    }
  }
  printf("]\n");

  return 0;
}

void printCityPath(struct CityName cityname[MAX_VERTEX], int rootPath[], int dest, int invert)
{
  if (invert == 0)    // Prints From Src to 0
  {
    printf("%c->", cityname[dest].name);
    if (rootPath[dest] == 0)
    {
      // printf("%c", cityname[rootPath[dest]].name);                           // Uncomment to have last node as capitol included
      return;
    }

    printCityPath(&cityname[0], rootPath, rootPath[dest], invert);
  }
  else                // Prints From 0 to Src
  {
    if (rootPath[dest] == 0)
    {
      printf("%c->%c", cityname[rootPath[dest]].name, cityname[dest].name);
      return;
    }

    printCityPath(&cityname[0], rootPath, rootPath[dest], invert);
    printf("->%c", cityname[dest].name);
  }
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

  struct CityName cityname[MAX_VERTEX];

  int D[MAX_VERTEX] = {INFI};         // Resulting shortest dist a to each city, default all to infinite
  struct City cityQ[MAX_VERTEX * MAX_VERTEX]; // Q
  int i = 0, j = 0, k = 0, z = 0;
  int qCity=0;
  int rootPath[MAX_VERTEX] = {INFI};  // stores Path of parent
  int finalizedCity[MAX_VERTEX];      // 0 means pending and 1 means finalized distance

  for (i = 0; i < MAX_VERTEX * MAX_VERTEX; i++)   // Defult everything to INFI
  {
    cityQ[i].vertex = INFI;
    cityQ[i].distance = INFI;
    cityQ[i].root = INFI;
  }

  for (i = 0; i < MAX_VERTEX; i++)   // Defulting
  {
    D[i] = INFI;
    finalizedCity[i] = 0;
    cityname[i].name = (char)(0x61 + (char)i);      // Load alpha names of city
  }

  D[0] = 0;               // City a's distance to itself is 0
  finalizedCity[0] = 1;   // City a is finalized already
  rootPath[0] = 0;        // City a's parent is itself
  k = 0;                  // Our Q

  i = 0;
  while (1)           // Untill Q is empty
  {
    // find adjecent and add them to Q
    for (j = 0; j < MAX_VERTEX; j++)
    {
      if ((graph[i][j] != INFI) && (i != j) && (j != 0) && (finalizedCity[j] != 1))
      {
        cityQ[k].vertex = j;
        cityQ[k].distance = graph[i][j];
        cityQ[k++].root = i;
      }
    }
#if ENABLE_DEBUG
    printQ(&cityQ[0]);                // Print current Q
#endif

    qCity = findMinQ(&cityQ[0]);      // Vertex who has min distance in adjecent

    if (qCity == INFI)                // Q empty now
    {
      break;
    }

    if (D[cityQ[qCity].vertex] > (D[cityQ[qCity].root] + graph[cityQ[qCity].root][cityQ[qCity].vertex]))    // If new distance is smaller, update it
    {
      D[cityQ[qCity].vertex] = D[cityQ[qCity].root] + graph[cityQ[qCity].root][cityQ[qCity].vertex];
      rootPath[cityQ[qCity].vertex] = cityQ[qCity].root;
      finalizedCity[cityQ[qCity].vertex] = 1;   // City's shortest distance finalized
    }

    i = cityQ[qCity].vertex;          // Store it and go for next adjecent

    cityQ[qCity].vertex = INFI;       // Reset the one we know is done in Q
    cityQ[qCity].distance = INFI;
    cityQ[qCity].root = INFI;
    z++;
  }

#if ENABLE_DEBUG
  printf("PATH:\n");
  for (i = 0; i < MAX_VERTEX; i++)
  {
    printf("(%d = %d): ", i, rootPath[i]);
  }
  printf("\n");

  for (i = 0; i < MAX_VERTEX; i++)
  {
    printf("*************************************\n");
    printf("Min distance- %d - %d\n", i, D[i]);
    printf("Path of %d: \n", i);
    printCityPath(&cityname[0], rootPath, i, 1);
    printf("\n");
  }
#endif

  printf("Shortest Path from d to i via a is: ");               // Print result path
  printCityPath(&cityname[0], rootPath, SOURCE_CITY, 0);                  // NOTE: Modify source city in macros to check other city
  printCityPath(&cityname[0], rootPath, DESTINATION_CITY, 1);             // NOTE: Modify destination city in macros to check other city
  printf("\n");

  return 0;
}
