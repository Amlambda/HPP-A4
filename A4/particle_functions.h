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
double get_part_dist_1D(double posTarget, double posOther);
double get_abs_dist(double xPosTarget, double yPosTarget, double xPosOther, double yPosOther);
double get_force_1D(double partDist, double absDist, double mass2);
