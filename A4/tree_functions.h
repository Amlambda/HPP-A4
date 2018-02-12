#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node * tl; //top left child
	struct node * tr; //top right child
	struct node * bl; //bottom left child
	struct node * br; //bottom right child

	//Top left corner of quad tree
	double xPos; 
	double yPos;
	double width;

	//struct particle * partPtr;
	//double * partPtr; //push particle address here
	int size; //nodes in inner tree

} node_t;

void add_node(struct node * node, double xPos, double yPos, double width);