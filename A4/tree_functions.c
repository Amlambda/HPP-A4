#include "tree_functions.h"
#include <math.h>

/* Node functions */
void descent(node_t *node){
  if(node != NULL)
    printf("{ tl.x:%f, tl.y:%f, br.x:%f, br.y:%f }: ", node->xPos,
      node->yPos, node->xPos + node->width, node->yPos + node->width);
}

void ascent(node_t *node){
  printf("\n");
}

void walk(node_t *root, void (*descent)(node_t *node), void (*ascent)(node_t *node)) {
  (*descent)(root);
  if(root->tl != NULL) walk(root->tl, descent, ascent);
  if(root->tr != NULL) walk(root->tr, descent, ascent);
  if(root->bl != NULL) walk(root->bl, descent, ascent);
  if(root->br != NULL) walk(root->br, descent, ascent);
  (*ascent)(root);
}


cm_t * calc_cm(node_t *root) {
  //Initiate center of mass-structs for each quadrant in the node
  cm_t * tlCm = (cm_t*)malloc(sizeof(cm_t)); //contains mass, x-position, y-position
  cm_t * trCm = (cm_t*)malloc(sizeof(cm_t));
  cm_t * blCm = (cm_t*)malloc(sizeof(cm_t));
  cm_t * brCm = (cm_t*)malloc(sizeof(cm_t));
  cm_t * newCm = (cm_t*)malloc(sizeof(cm_t));

  //Recursively calculate CoM for each quadrant
  if(root->tl != NULL){ 
    if(isleaf(root->tl)){
      tlCm = root->tl->nodeCm;
    }else if(ispointer(root->tl)){
      tlCm = calc_cm(root->tl);
    }else{
      (tlCm)->mass = 0;
      (tlCm)->xPos = 0;
      (tlCm)->yPos = 0;
    }
  }
  if(root->tr != NULL){ 
    if(isleaf(root->tr)){   
      trCm = root->tr->nodeCm;
    }else if(ispointer(root->tr)){
      trCm = calc_cm(root->tr);
    }else{
      (trCm)->mass = 0;
      (trCm)->xPos = 0;
      (trCm)->yPos = 0;
    }
  }
  if(root->bl != NULL){ 
    if(isleaf(root->bl)){
      blCm = root->bl->nodeCm;
    }else if(ispointer(root->bl)){
      blCm = calc_cm(root->bl);
    }else{
      (blCm)->mass = 0;
      (blCm)->xPos = 0;
      (blCm)->yPos = 0;
    }
  }
  if(root->br != NULL){ 
    if(isleaf(root->br)){
      brCm = root->br->nodeCm;
    }else if(ispointer(root->br)){
      brCm = calc_cm(root->br);
    }else{
      (brCm)->mass = 0;
      (brCm)->xPos = 0;
      (brCm)->yPos = 0;
    }
  }


  /* TRYING TO WEIGH TOGETHER CM HERE BUT IT FAILS!! */
  double totMass = tlCm->mass + trCm->mass + blCm->mass + brCm->mass;
  newCm->mass = totMass;
  newCm->xPos = ((tlCm->mass)*((tlCm->xPos)-(root->tl->width)) + (trCm->mass)*((trCm->xPos)-(root->tr->width)) + (blCm->mass)*((blCm->xPos)-(root->bl->width)) + (brCm->mass)*((brCm->xPos)-(root->br->width)))/totMass;
  newCm->yPos = ((tlCm->mass)*((tlCm->yPos)-(root->tl->width)) + (trCm->mass)*((trCm->yPos)-(root->tr->width)) + (blCm->mass)*((blCm->yPos)-(root->bl->width)) + (brCm->mass)*((brCm->yPos)-(root->br->width)))/totMass;
  
  free(tlCm);
  free(trCm);
  free(blCm);
  free(brCm);

  printf("New center of mass, mass: %f, xpos: %f, ypos: %f.\n",newCm->mass,newCm->xPos,newCm->yPos);
  root->nodeCm = newCm;
  printf("Confirm new center of mass, mass: %f, xpos: %f, ypos: %f.\n",root->nodeCm->mass,root->nodeCm->xPos,root->nodeCm->yPos);

  return newCm;
}

// void update_cm(cm_t * newCm,){

// }

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
  	node->particle = 0;
    node->nodeCm->mass  = 0;
    node->nodeCm->xPos  = 0;
    node->nodeCm->xPos  = 0;

	return insert(node, old);
}

// Inserts particle into quadtree
int insert(node_t * root, particle_t * particle) {
	if(isempty(root)){
		root->particle = particle;
    root->nodeCm = (cm_t*)malloc(sizeof(cm_t));
    root->nodeCm->mass  = particle->mass;
    root->nodeCm->xPos  = particle->xPos;
    root->nodeCm->yPos  = particle->yPos;
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
      && ((quadrant->yPos) <= (particle->yPos))						// Check upper border	
      && ((quadrant->yPos + quadrant->width) >= (particle->yPos));	// Check lower border
}


