#ifndef PARAMETERS.H
#define PARAMETERS.H

//TODO: ask for the parameters in the cmd line
//TODO: set some more realistic parameters and maybe change FIELD_SIZE_COORDS to DISTANCE_BETWEEN_NODES
//TODO: replace DOG_SPEED with a random number. It may be a good idea to draw speed from a normal distribution
//	with a mean around firefighter_speed
#define FIELD_SIZE_X 200
#define FIELD_SIZE_Y 200
#define NUM_NODES 6 //actually 6 firefighters and 6 dogs, so 12
#define SPEED_NODES 0.4 //m/s
#define STARTING_POS_X 100
#define STARTING_POS_Y 100
#define SAMPLES_NUMBER 500
#define DURATION 900
#define DELTA_X 15 //(m) this is the distance the dogs will travel before returning
#define SINE_ARC_LENGTH 7.64
#define PI 3.14159265
#define FILEPATH "../../data/scenario5.movements" //FIXME
#define FILEPATH1 "../../data/ns_scenario5.movements"
#define DOG_SPEED (SINE_ARC_LENGTH*DELTA_X*SPEED_NODES)/(16*PI)
#define concat(first, second) first second //remove?
#define PROB	2 //in per mil
#define SEED 102 //to make it truly random set to 0

#endif//PARAMETERS.H