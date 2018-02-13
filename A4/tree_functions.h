#include "particle_functions.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct cm {
	double mass;
	double xPos;
	double yPos;
} cm_t;

typedef struct node {
	struct node * tl; //top left child
	struct node * tr; //top right child
	struct node * bl; //bottom left child
	struct node * br; //bottom right child

	// Top left corner of quad
	double xPos; 
	double yPos;

	// Width of quad
	double width;

	// Particle in quad (if leaf. else, it is  NULL)
	particle_t * particle;

	struct cm * nodeCm;
	//Center of mass of node, mass and coord
	double cmMass;
	double cmXPos;
	double cmYPos;


} node_t;


void descent(node_t *node);
void ascent(node_t *node);
cm_t * calc_cm(node_t *root);
void walk(node_t *root, void (*descent)(node_t *node), void (*ascent)(node_t *node));
// Creates new empty node with specified top left corner coordinates and width 
node_t * new_node(double xPos, double yPos, double width); 

// Checks if node is empty	( = having no children and no particle stored)
int isempty(node_t * node);

// Checks if node is a leaf ( = having a particle stored)
int isleaf(node_t * node);

// Checks if node is pointer ( = having children but no particle stores)
int ispointer(node_t *node);

int split_node(node_t *node);

// Inserts particle into quadtree
int insert(node_t *root, particle_t * particle);

// Gets quadrant which to place particle in
node_t * get_quadrant(node_t * root, particle_t * particle);

// Checks if particles coordinates matches those of quadrant
int match_coords(node_t * quadrant, particle_t * particle);

