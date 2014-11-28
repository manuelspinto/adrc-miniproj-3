#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connectivity.h"

void remove_link(int head, int tail, Node **graph){
	Node *aux;


	if(graph[head]->num == tail){
		aux = graph[head];
		graph[head] = graph[head]->next;
		free(aux);	
		return;
	}

	for(aux=graph[head]; aux->next->num != tail; aux = aux->next);
	aux = aux->next->next;
	free(aux->next);

	return;
}


Node *new_node(int num, Node* next){
	Node *new = (Node*) malloc(sizeof(Node));
	new->num = num;
	new->next = next;
	return new;
}

void new_link(int head, int tail, Node **graph){
	graph[head] = new_node(tail,graph[head]);
}

void print_graph(Node**graph, int n){
	int i;
	Node *aux;

	for(i = 0; i<=n ; i++){
		printf("[%d]: ",i);
		for(aux = graph[i]; aux != NULL ; aux = aux->next)
			printf("%d ",aux->num);
		printf("\n");
	}

}

void BFS(Node **graph, int n, int s, int t, int *parents){
	int visited[n+1];
	int i;
	Node *L = NULL;
	Node *last = NULL;
	Node *aux;

	for(i = 0; i<=n ; i++)
		visited[i] = 0;

	visited[s] = 1;

	L = new_node(s, NULL);
	last = L;

	while(L != NULL){ /* Até não existirem nós para visitar */
		for(aux = graph[L->num];aux != NULL; aux = aux->next){ 
		/* Percorremos vizinhos do primeiro nó na lista */
			if(visited[aux->num] == 0){ /* Se vizinho não visitado */
				visited[aux->num] = 1; /* Marcar como visitado */
				parents[aux->num] = L->num; /* Pai do vizinho é o primeiro nó de L */
				if(aux->num == t){ /* Se o vizinho for o destino, já encontrámos o caminho mais curto de s->t */
					while(L != NULL){ /* Apagar a lista L */
						aux = L;
						L = L->next;
						free(aux);
					}
					return;
				}
				/* Se o vizinho não for o destino, colocar no final de L */
				last->next = new_node(aux->num, NULL);
				last = last->next;
			}
		}
		/* Não existem mais vizinhos a visitar, remover primeiro nó da lista */
		aux = L;
		L = L->next;
		free(aux);
	}

	return;
}

int disjoint_paths(Node **graph,int n, int s, int t){
	int *parents;
	int i, count = 0;

	parents = (int*) malloc((n+1)*sizeof(int));
	for(i = 0; i < n+1 ; i++)
		parents[i] = -1;

	BFS(graph,n,s,t,parents);
	while(parents[t] != -1){
		count++;
		i = t;
		while(parents[i] != -1){
			new_link(i, parents[i], graph);
			remove_link(parents[i], i, graph);
			i = parents[i];
		}
		for(i = 0; i < n+1 ; i++)
		parents[i] = -1;
		BFS(graph,n,s,t,parents);
		printf("\n");
		print_graph(graph, n);
	}

	return count;
}


int main(int argc, char *argv[]){
	FILE *fp;
	char buff[128];
	int n, i, head, tail;
	Node **network, **res_net;
	


	fp = fopen(argv[1],"r");
	if(fp == NULL){
	  printf("ERROR: Impossivel abrir o ficheiro '%s'\n",argv[1]);
	  exit(1);
	}

	fgets(buff, sizeof(buff), fp);
	sscanf(buff,"%d",&n);

	network = (Node**) malloc((n+1)*sizeof(Node*));
	res_net = (Node**) malloc((n+1)*sizeof(Node*));
	
	for(i = 0; i < n+1 ; i++){
		network[i] = NULL;
		res_net[i] = NULL;
	}

  
	while (fgets(buff, sizeof(buff), fp) != NULL ){
	  sscanf(buff,"%d %d",&head,&tail);
	  new_link(head, tail, network);
	  new_link(head, tail, res_net);
	}

	print_graph(network, n);

	/*BFS(res_net,n,1,6,parents);
	for(i = 0; i<= 6; i++)
		printf("%d tem pai %d\n",i,parents[i]);*/

	printf("Number of Link disjoint paths = %d\n",disjoint_paths(res_net,n,1,6));
	

	


	return 0;
}

