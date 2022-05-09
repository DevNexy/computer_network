#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 10
#define INF 1000000
#define TRUE 1
#define FALSE 0

typedef struct GraphType{
	int n;
	int cost[MAX_NODES][MAX_NODES];
}GraphType;
int distance[MAX_NODES];
int found[MAX_NODES];
void print_status(GraphType* g);
int choose(int distance[], int n, int found[]);
void shortest_path(GraphType* g, int start_node);
int main(void)
{
	GraphType G = { 6,
		{{0,5,3,7,INF,INF},
		{5,0,4,INF,7,9},
		{3,4,0,3,8,INF},
		{7,INF,3,0,4,INF},
		{INF,7,8,4,0,2},
		{INF,9,INF,INF,2,0}}
	};
	shortest_path(&G,0);
	return 0;

}
void print_status(GraphType* g)
{
	static int step = 1;
	printf("STEP %d: ", step++);
	printf("distance: ");
	for(int i = 0; i<g->n;i++){
		if(distance[i]==INF)
			printf(" * ");
		else
			printf("%2d ", distance[i]);
	}
	printf("\n");
	printf("	found:    ");
	for(int i =0;i<g->n;i++)
		printf("%2d ",found[i]);
	printf("\n\n");
}
int choose(int distance[], int n, int found[])
{
	int i, min, minpos;

	min = INT_MAX;
	minpos = -1;
	for(i =0 ; i <n;i++)
		if(distance[i] < min && !found[i]){
			min = distance[i];
			minpos = i;
		}
	return minpos;
	
}
void shortest_path(GraphType * g, int start_node)
{
	int i, u, w;
	for(i = 0; i <g->n; i++){
		distance[i] = g->cost[start_node][i];
		found[i] = FALSE;
	}
	found[start_node] = TRUE;
	distance[start_node] = 0;
	for(i=0;i<g->n-1;i++){
		print_status(g);
		u=choose(distance, g->n, found);
		found[u] = TRUE;
		for(w = 0; w<g->n;w++)
			if(!found[w])
				if(distance[u] + g->cost[u][w]<distance[w])
					distance[w] = distance[u] + g->cost[u][w];
	}
}

