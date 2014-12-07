#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connectivity.h"

void remove_link(int head, int tail, Node **graph){
	Node *aux;
	Node *rem;

	if(graph[head]->num == tail){
		aux = graph[head];
		graph[head] = graph[head]->next;
		free(aux);	
		return;
	}

	for(aux=graph[head]; aux->next->num != tail; aux = aux->next);
	rem = aux->next;
	aux->next = aux->next->next;
	free(rem);

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


/*retorna um ponteiro para uma copia do grafo que descreve a rede*/
Node ** copy_graph(Node ** network, int n){

	int i;
	Node * aux;
	Node ** copy = (Node**) malloc((n+1)*sizeof(Node*));

	for(i = 0; i <= n; i++) copy[i] = NULL;

	for(i = 0; i <= n; i++)
		for(aux = network[i]; aux != NULL; aux = aux->next)
			new_link(i, aux->num, copy);

	return copy;

}

/*apaga um grafo da memoria*/
void delete_graph(Node ** graph, int n){

	Node * aux;
	int i;

	for(i = 0; i <= n; i++)
		while(graph[i] != NULL){
			aux = graph[i];
			graph[i] = graph[i]->next;
			free(aux);
		}

	return;
}

void BFS(Node **graph, int n, int s, int t, int *parents){
	int visited[n+1]; /*Estão a chatear aqui.. se calhar temos de usar malloc*/
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
		/*print_graph(graph, n);*/
	}

	return count;
}

K * new_k(int k){
	
	K * new = (K*) malloc (sizeof(K));
	new->k = k;
	new->next = NULL;
	new->nodes = NULL;

	return new;
}

Pair * new_pair(int s, int t){

	Pair * new = (Pair*) malloc (sizeof(Pair));
	new->s = s;
	new->t = t;
	new->next = NULL;

	return new;
}

/*retorna um apontador para a k_list com o k dado*/
K * return_k_list(K ** k_lists, int k){
	K * aux_b, * aux_a, * new;
	
	/*Ver se existe uma k_list com o k dado*/
	/*se nao existir nenhuma k_list*/
	if(*k_lists == NULL){
		*k_lists = new_k(k);
		return *k_lists;
	}

	/*ver se vamos ter de inserir um no K na primeira posicao de k_lists*/
	if((*k_lists)->k > k){
		new = new_k(k);
		new->next = (*k_lists);
		*k_lists = new;
		return *k_lists;
	}

	/*aux_b vai ficar a apontar para a k_list que queremos ou para o sitio onde a queremos criar*/
	for(aux_a = (*k_lists)->next, aux_b = *k_lists; aux_a != NULL && aux_a->k <= k; aux_a = aux_a->next, aux_b = aux_b->next);

	if(aux_b->k == k){
		return aux_b;
	}

	new = new_k(k);
	new->next = aux_b->next;
	aux_b->next = new;
	return aux_b->next;
}

/*adicionar a lista k, o par de nos (s,t)*/
void add_pair(K ** k_lists, int s, int t, int k){

	Pair * new = new_pair(s,t);
	K * k_list = return_k_list(k_lists, k);

	new->next = k_list->nodes;
	k_list->nodes = new;
}

void print_k_lists(K * k_lists){
	K * aux_k;
	Pair * aux_p;

	for(aux_k = k_lists; aux_k != NULL; aux_k = aux_k->next){
		printf("k = %d: ", aux_k->k);
		for(aux_p = aux_k->nodes; aux_p != NULL; aux_p = aux_p->next){
			printf(" (%d,%d)", aux_p->s,aux_p->t);
		}
		printf("\n");
	}
	
	return;
}


void print_k_counts(K * k_lists){

	K * aux_k;
	Pair * aux_p;
	int max_k, pairs_num = 0, i;
	int * k_counts;

	for(aux_k = k_lists; aux_k != NULL; aux_k = aux_k->next)
		max_k = aux_k->k;

	k_counts = (int*) malloc((max_k+1)*sizeof(int));

	for(i = 0; i <= max_k; i++)
		k_counts[i] = 0;

	for(aux_k = k_lists; aux_k != NULL; aux_k = aux_k->next){
		for(aux_p = aux_k->nodes; aux_p != NULL; aux_p = aux_p->next){
			k_counts[aux_k->k]++;
			pairs_num++;
		}
	}

	printf("Printing the complementary cumulative distribution of the minimum number of links that separates one node from another...\n");

	for(i = 0; i <= max_k; i++)
		printf("k = %d: %.2f %% of the pairs of nodes\n", i, (((float) k_counts[i] / pairs_num))*100);

	printf("\n");

	free(k_counts);

	return;
}

int main(int argc, char *argv[]){
	FILE *fp;
	char buff[128];
	int n, i, j, k, head, tail, option, source, dest;
	Node **network, **res_net;
	K * k_lists = NULL;	


	fp = fopen(argv[1],"r");
	if(fp == NULL){
	  printf("ERROR: Impossivel abrir o ficheiro '%s'\n",argv[1]);
	  exit(1);
	}

	fgets(buff, sizeof(buff), fp);
	sscanf(buff,"%d",&n);

	network = (Node**) malloc((n+1)*sizeof(Node*));
	
	for(i = 0; i < n+1 ; i++)
		network[i] = NULL;

  
	while (fgets(buff, sizeof(buff), fp) != NULL ){
	  sscanf(buff,"%d %d",&head,&tail);
	  new_link(head, tail, network);
	}

	print_graph(network, n);

	while(1){
		printf("[1] Compute the minimum number of links that separates a source node from a destination node\n");
		printf("[2] Compute the complementary cumulative distribution of the minimum number of links that separates one node from another\n");
		printf("[3] Compute the link-connectivity of this network\n");
		printf("[4] Quit\n");
		printf("Choose an option: ");
		scanf("%d", &option);

		if(option == 1){
			printf("\nChoose a source node:");
			scanf("%d", &source);
			if(source > n || source < 1) printf("\nInvalid source node!\n");

			else{
				printf("\nChoose a destination node:");
				scanf("%d", &dest);
				if(dest > n || dest < 1) printf("\nInvalid destination node!\n");

				else{
					res_net = copy_graph(network,n);
					printf("\nNumber of Link disjoint paths from source node %d to destination node %d = %d\n\n", source, dest, disjoint_paths(res_net,n,source,dest));
					delete_graph(res_net,n);					
				}
			}
		}
		else if(option == 2){ /*LIBERTAR MEMORIA*/

			for(i = 1; i <= n; i++){
				for(j = 1; j <= n; j++){
					if(i != j){
						res_net = copy_graph(network,n);
						k = disjoint_paths(res_net,n,i,j);
						printf("from s = %d to t = %d, k = %d\n", i, j, k);
						add_pair(&k_lists, i, j, k);
						delete_graph(res_net,n);
						print_k_lists(k_lists);
						printf("\n\n");
					}
				}
			}

			print_k_counts(k_lists);
		}
		else if(option == 3){


		}
		else if(option == 4) break;
		else printf("Invalid option!\n");

			
	}
	

	


	return 0;
}

