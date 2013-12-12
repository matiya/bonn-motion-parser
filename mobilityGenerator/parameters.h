#ifndef PARAMETERS_H
#define PARAMETERS_H

//TODO: ask for the parameters in the cmd line
//TODO: set some more realistic parameters and maybe change FIELD_SIZE_COORDS to DISTANCE_BETWEEN_NODES
//TODO: replace DOG_SPEED with a random number. It may be a good idea to draw speed from a normal distribution
//	with a mean around firefighter_speed
#define FIELD_SIZE_X 300
#define FIELD_SIZE_Y 600
#define NUM_NODES 6 //actually 6 firefighters and 6 dogs, so 12
#define SPEED_NODES 0.4 //m/s
#define STARTING_POS_X 100
#define STARTING_POS_Y 100
#define SAMPLES_NUMBER 500
#define DURATION 900
#define DELTA_X 15 //(m) this is the distance the dogs will travel before returning
#define DELTA_Y 5 // how many meters are the dogs ahead of the firefighter
#define SINE_ARC_LENGTH 7.64
#define PI 3.14159265
#define FILEPATH "../../data/scenario10.movements" //FIXME
#define FILEPATH1 "../../data/ns_scenario10.movements"
#define DOG_SPEED (SINE_ARC_LENGTH*DELTA_X*SPEED_NODES)/(16*PI)
#define STD_DEVIATION DOG_SPEED/2
#define PROB	5 //in per mil
#define SEED 0//to make it truly random set to 0

#endif//PARAMETERS.H