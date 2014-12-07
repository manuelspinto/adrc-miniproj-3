/* connectivity.h */

typedef struct node{
	int num;
	struct node *next;

} Node;

typedef struct pair_node{
	int s;
	int t;
	struct pair_node *next;

} Pair;


typedef struct k_node{
	int k;
	struct k_node *next;
	struct pair_node *nodes;

} K;




