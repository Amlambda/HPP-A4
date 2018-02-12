#include "particle_functions.h"
#include <math.h>
						
const double eps0 = 0.001;

//Calculated position in 1D
double get_pos_1D(double pos, double vel, double delta_t) {
	return pos + delta_t*vel;
}

//Calculates velocity in 1D
double get_vel_1D(double acc, double vel, double delta_t){
	return vel + (delta_t*acc);
}

//Calculates the distance between two particles in 1D
double get_part_dist_1D(double pos1, double pos2, double absDist){
	return pos1-pos2;
}

//Calculates the absolute distance between two particles in 2D
double get_abs_dist(double posX1, double posY1, double posX2, double posY2){
	return sqrt((posX1-posX2)*(posX1-posX2) + (posY1-posY2)*(posY1-posY2));
}

// Calculates the contributioin to the force sum in 1D on target particle from one other particle 
double get_force_1D(double partDist, double absDist, double mass2) {
	// Plummer sphere modification, r<<1 
	return (mass2*partDist)/(pow((absDist + eps0),3));
}

