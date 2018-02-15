#include "tree_functions.h"
#include <math.h>
#include <assert.h>

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

void set_cm(cm_t * cm, double mass, double x, double y){
  cm->mass = mass;
  cm->xPos = x;
  cm->yPos = y;
}

void update_cm(cm_t * newCm, cm_t * tlCm, cm_t * trCm, cm_t * blCm, cm_t * brCm, double width){
  cm_t * cmVec[4]; 
  cmVec[0]= tlCm; //Top left node center of mass
  cmVec[1]= trCm;
  cmVec[2]= blCm;
  cmVec[3]= brCm; 
  double totMass = 0;
  double xAvg = 0; 
  double yAvg = 0;

  for(int i=0; i<4; i++){
    if(cmVec[i] != NULL){
      printf("Mass added: %f\n", cmVec[i]->mass);
      totMass += cmVec[i]->mass;
    }
  }

  assert(totMass>0);
  printf("Total mass is: %f\n", totMass);

  for(int i=0; i<4; i++){
    if(cmVec[i] != NULL){
      // Weight with cm's distance to mother nodes center.
      printf("index %d: x value added: %f\n", i,(cmVec[i]->xPos));
      printf("index %d: y value added: %f\n", i,(cmVec[i]->yPos));
      xAvg += (cmVec[i]->xPos)*(cmVec[i]->mass); 
      yAvg += (cmVec[i]->yPos)*(cmVec[i]->mass);
    }
  }
  xAvg /= totMass;
  yAvg /= totMass;

  assert(xAvg>=0);
  assert(xAvg<=1);
  assert(yAvg>=0);
  assert(yAvg<=1);

  newCm->mass = totMass;
  newCm->xPos = xAvg;
  newCm->yPos = yAvg; 
  free(cmVec);
}


cm_t * calc_cm(node_t *root) {

  if(isempty(root)){
    return NULL;
  }else if (isleaf(root)){
    root->nodeCm = (cm_t*)malloc(sizeof(cm_t));
    set_cm(root->nodeCm, root->particle->mass, root->particle->xPos, root->particle->yPos);
    return root->nodeCm;
  }else{
  
    //Initiate temp center of mass-struct
    double data[3]; 
    cm_t * newCm = (cm_t*)malloc(sizeof(cm_t));
    
    //Recursively calculate CoM for each quadrant
    if(!isempty(root->tl)){ 
      root->tl->nodeCm = calc_cm(root->tl);
        // printf("TOP LEFT\n");
        // printf("CM mass: %f\n", root->tl->nodeCm->mass);
        // printf("CM xPos: %f\n", root->tl->nodeCm->xPos);
        // printf("CM yPos: %f\n", root->tl->nodeCm->yPos);
    }
    if(!isempty(root->tr)){  
      root->tr->nodeCm  = calc_cm(root->tr);
        // printf("TOP RIGHT\n");
        // printf("CM mass: %f\n", root->tr->nodeCm->mass);
        // printf("CM xPos: %f\n", root->tr->nodeCm->xPos);
        // printf("CM yPos: %f\n", root->tr->nodeCm->yPos);
    }
    if(!isempty(root->bl)){ 
      root->bl->nodeCm = calc_cm(root->bl);
        // printf("BOTTOM LEFT\n");
        // printf("CM mass: %f\n", root->bl->nodeCm->mass);
        // printf("CM xPos: %f\n", root->bl->nodeCm->xPos);
        // printf("CM yPos: %f\n", root->bl->nodeCm->yPos);
    }
    if(!isempty(root->br)){ 
      root->br->nodeCm = calc_cm(root->br);
        // printf("BOTTOM RIGHT\n");
        // printf("CM mass: %f\n", root->br->nodeCm->mass);
        // printf("CM xPos: %f\n", root->br->nodeCm->xPos);
        // printf("CM yPos: %f\n", root->br->nodeCm->yPos);    
    }

    //Calculate the node's new center of mass from the childrens center of mass
    update_cm(newCm, root->tl->nodeCm, root->tr->nodeCm, 
      root->bl->nodeCm,root->br->nodeCm, root->width);

    printf("New center of mass, mass: %f, xpos: %f, ypos: %f.\n",
      newCm->mass,newCm->xPos,newCm->yPos);
    
    data[0] = newCm->mass;
    data[1] = newCm->xPos;
    data[2] = newCm->yPos;
   
    free(newCm);

    root->nodeCm = (cm_t*)malloc(sizeof(cm_t));
    root->nodeCm->mass = data[0];
    root->nodeCm->xPos = data[1];
    root->nodeCm->yPos = data[2];

    return root->nodeCm;
  }
}



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
  cm_t * nodeCm = NULL;
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
    // node->nodeCm->mass  = 0;
    // node->nodeCm->xPos  = 0;
    // node->nodeCm->xPos  = 0;

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
      && ((quadrant->yPos) <= (particle->yPos))						// Check upper border	
      && ((quadrant->yPos + quadrant->width) >= (particle->yPos));	// Check lower border
}


