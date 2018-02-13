#include "tree_functions.h"
#include <math.h>

/* Node functions */

// Creates new empty node with specified top left corner coordinates and width 
node_t * new_node(double xPos, double yPos, double width) {
	node_t * node;
	if(!(node = malloc(sizeof(*node))))
    	return NULL;
	node->tl = NULL;
  	node->tr = NULL;
  	node->bl = NULL;
  	node->br = NULL;
  	node->xPos = xPos;
  	node->yPos = yPos;
  	node->width = width;
  	node->particle = NULL;
	return node;
}

// Checks if node is empty	( = having no children and no particle stored)
int isempty(node_t * node){
  return node->tl == NULL
      && node->tr == NULL
      && node->bl == NULL
      && node->br == NULL
      && !isleaf(node);
}

// Checks if node is a leaf ( = having a particle stored)
int isleaf(node_t * node){
  return node->particle != NULL;
}

// Checks if node is pointer ( = having children but no particle stores)
int ispointer(node_t *node){
  return node->tl != NULL
      && node->tr != NULL
      && node->bl != NULL
      && node->br != NULL
      && !isleaf(node);
}

int split_node(node_t *node){
	// Declare children
	node_t * tl;
	node_t * tr;
	node_t * bl;
	node_t * br;

	// Get top left corner coordinates of quadrant
  	double x  = node->xPos;
  	double y  = node->yPos;

  	// Compute width of children quadrants
  	double w = node->width / 2;

  	// Create children nodes
  	if(!(tl = new_node(x, y, w))) return 0;
  	if(!(tr = new_node(x + w, y, w))) return 0;
  	if(!(bl = new_node(x, y + w, w))) return 0;
  	if(!(br = new_node(x + w, y + w, w))) return 0;

  	node->tl = tl;
  	node->tr = tr;
  	node->bl = bl;
  	node->br = br;

  	// Save pointer to particle which was stored in node before split
  	particle_t * old = node->particle;

  	// Set particle attribute to null
  	node->particle = NULL;

	return insert(node, old);
}

// Inserts particle into quadtree
int insert(node_t * root, particle_t * particle) {
	if(isempty(root)){
		root->particle = particle;
		return 1; /* normal insertion flag */
	} else if(isleaf(root)) {
  		if(root->particle->xPos == particle->xPos && root->particle->yPos == particle->yPos){
      		printf("Error: Two particles are on the same position.\n");
      		exit(1); /* Break programr un if two different particles have the same postions */
    	} else {
    		if(!split_node(root)){	
        		return 0; /* failed insertion flag */
      		}
     		insert(root, particle);
     	}
    } else if(ispointer(root)) {
    	node_t * quadrant = get_quadrant(root, particle);		// Get quadrant which to place particle in
    	return quadrant == NULL ? 0 : insert(quadrant, particle);
    }
    return 0; /* failed insertion flag */
}

// Gets quadrant which to place particle in
node_t * get_quadrant(node_t * root, particle_t * particle) {		// DENNA MATCHAR INTE
	if (match_coords(root->tl, particle)) return root->tl;
	if (match_coords(root->tr, particle)) return root->tr;
	if (match_coords(root->bl, particle)) return root->bl;
	if (match_coords(root->br, particle)) return root->br;
	return NULL; 
}

// Checks if particles coordinates matches those of quadrant
int match_coords(node_t * quadrant, particle_t * particle) {	// Kollat att quadrant inte är noll! Det stämmer!
	// printf("Partikelns x: %f \n", particle->xPos);
	// printf("Partikelns y: %f \n", particle->yPos);
	// printf("%f \n", quadrant->xPos);
	// printf("%f \n", quadrant->yPos);
	// printf("%f \n", quadrant->width);
	// printf("%d , %d , %d , %d \n", quadrant->xPos <= particle->xPos,
	// 	quadrant->xPos + quadrant->width >= particle->xPos,
	// 	quadrant->yPos <= particle->yPos,
	// 	quadrant->yPos + quadrant->width >= particle->yPos);
      return ((quadrant->xPos) <= (particle->xPos))					// Check left border
      && ((quadrant->xPos + quadrant->width) >= (particle->xPos))		// Check right border
      && ((quadrant->yPos) <= (particle->yPos))						// Check upper border		BLIR FEL HÄR!
      && ((quadrant->yPos + quadrant->width) >= (particle->yPos));	// Check lower border
}

/*void add_node(node_t * node, double xPos, double yPos, double width){
	node->xPos = xPos; 
	node->yPos = yPos;
	node->width = width;
}*/
