//Function descriptions
#include <stdio.h>
#include <stdlib.h>

struct particle {		
	double xPos;
	double yPos;
	double mass;
	double xVel;
	double yVel;
};
typedef struct particle particle_t;

double get_pos_1D(double pos, double vel, double delta_t);
double get_vel_1D(double acc, double vel, double delta_t);
double get_part_dist_1D(double pos1, double pos2, double absDist);
double get_abs_dist(double posX1, double posY1, double posX2, double posY2);
double get_force_1D(double partDist, double absDist, double mass2);
