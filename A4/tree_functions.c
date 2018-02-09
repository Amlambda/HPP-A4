#include "tree_functions.h"
#include <math.h>

void add_node(struct node * node, double xPos, double yPos, double width){
	node->xPos = xPos; 
	node->yPos = yPos;
	node->width = width;
}