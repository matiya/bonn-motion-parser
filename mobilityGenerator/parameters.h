#ifndef PARAMETERS_H
#define PARAMETERS_H

//TODO: ask for the parameters in the cmd line
//TODO: set some more realistic parameters and maybe change FIELD_SIZE_COORDS to DISTANCE_BETWEEN_NODES
//TODO: replace DOG_SPEED with a random number. It may be a good idea to draw speed from a normal distribution
//	with a mean around firefighter_speed
//#define FIELD_SIZE_X 300
//#define FIELD_SIZE_Y 600
//#define NUM_NODES 6 //actually 6 firefighters and 6 dogs, so 12
//#define SPEED_NODES 0.4 //m/s
// #define STARTING_POS_X 100
// #define STARTING_POS_Y 100
//#define SAMPLES_NUMBER 500
//#define DURATION 900
//#define DELTA_X 15 //(m) this is the distance the dogs will travel before returning
//#define DELTA_Y 5 // how many meters are the dogs ahead of the firefighter
#define SINE_ARC_LENGTH 7.64
#define PI 3.14159265
#define FILEPATH "../../data/scenario11.movements" //FIXME
#define FILEPATH1 "../../data/ns_scenario11.movements"
//#define DOG_SPEED (SINE_ARC_LENGTH*DELTA_X*SPEED_NODES)/(16*PI)
//#define STD_DEVIATION DOG_SPEED/2
//#define PROB	5 //in per mil
//#define SEED 0//to make it truly random set to 0
#include "anyoption.cpp" //change to .h

unsigned long SAMPLES_NUMBER, DELTA_X, DELTA_Y, DURATION, SEED, FIELD_SIZE_X, FIELD_SIZE_Y;
unsigned int PROB, NUM_NODES;
double STD_DEVIATION, SPEED_NODES, DOG_SPEED;
bool verbose;
std::string FILE_NAME;

void askForParameters( int argc, char **argv){
 
    /* 1. CREATE AN OBJECT */
    //AnyOption *opt = new AnyOption();
    AnyOption cmd;
    /* 2. SET PREFERENCES  */
    //cmd.noPOSIX(); /* do not check for POSIX style character options */
    cmd.setVerbose(); /* print warnings about unknown options */
    cmd.autoUsagePrint(true); /* print usage for bad options */

    /* 3. SET THE USAGE/HELP   */
    cmd.addUsage( "" );
    cmd.addUsage( "Usage: " );
    cmd.addUsage( "" );
    cmd.addUsage( " -h   --help\t\tprints this help ");
    cmd.addUsage( " -f   --file\t\tfile name");
    cmd.addUsage( " -n   --nodes\t\tnumber of nodes");
    cmd.addUsage( " -s   --samples\t\tnumbers of samples to be taken per node ");
    cmd.addUsage( " -d   --duration\thow long the simulation runs (in seconds)");
    cmd.addUsage( " -x   --xLimit\t\tlimit of the y coordinate (in meters) ");
    cmd.addUsage( " -y   --yLimit\t\tlimit of the y coordinate (in meters) ");
    cmd.addUsage( " -p   --probability\tprobability of a node of deviating (in per mil)");
    cmd.addUsage( " -D   --deviation\tstandard deviation of the probability distribution which controls the nodes' speed");
    cmd.addUsage( " -r   --speed\t\tspeed of the firefighters");
    cmd.addUsage( " -X   --deltaX\t\tamplitude of the sine wave in meters (in meters)");
    cmd.addUsage( " -Y   --deltaY\t\toffset between the firefighters and the dogs (in meters)");
    cmd.addUsage( " -S   --seed\t\tseed, if set to 0 it will be random");
    cmd.addUsage( " -v   --verbose\t\tdisplays debug info");
    cmd.addUsage( "" );

    /* 4. SET THE OPTION STRINGS/CHARACTERS */

    /* by default all  options  will be checked on the command line and from option/resource file */
    cmd.setFlag(  "help", 'h' );   /* a flag (takes no argument), supporting long and short form */ 
    cmd.setOption(  "file", 'f' ); 
    cmd.setOption(  "nodes", 'n' ); 
    cmd.setOption(  "samples", 's' ); 
    cmd.setOption(  "yLimit", 'y' ); 
    cmd.setOption(  "xLimit", 'x' );
    cmd.setOption(  "duration", 'd' ); 
    cmd.setOption(  "probability", 'p' );
    cmd.setOption(  "deviation", 'D' ); 
    cmd.setOption(  "deltaX", 'X' ); 
    cmd.setOption(  "deltaY", 'Y' ); 
    cmd.setOption(  "seed", 'S' ); 
    cmd.setFlag(  "verbose", 'v' );   /* a flag (takes no argument), supporting long and short form */ 
    
    /* for options that will be checked only on the command and line not in option/resource file */
    cmd.setCommandFlag(  "zip" , 'z'); /* a flag (takes no argument), supporting long and short form */

    /* for options that will be checked only from the option/resource file */
    cmd.setFileOption(  "title" ); /* an option (takes an argument), supporting only long form */

    /* 5. PROCESS THE COMMANDLINE AND RESOURCE FILE */

    /* read options from a  option/resource file with ':' separated opttions or flags, one per line */
    cmd.processFile( "/home/user/.options" );  
    /* go through the command line and get the options  */
    cmd.processCommandArgs( argc, argv );

    if( ! cmd.hasOptions()) { /* print usage if no options */
	    cmd.printUsage();
	    return;
    }

    /* 6. GET THE VALUES */
    if( cmd.getFlag( "help" ) || cmd.getFlag( 'h' ) ) 
	cmd.printUsage();
    
    if( cmd.getFlag( "verbose" ) || cmd.getFlag( 'v' ) )
	verbose = true;
    
    if( cmd.getValue( 's' ) != NULL  || cmd.getValue( "samples" ) != NULL  ){
      SAMPLES_NUMBER = atoi(cmd.getValue('s')); 
    }
    else{
      std::cout << "REQUIRED: number of samples" << std::endl;
      std::cout << endl ;
      std::terminate();   
    }
    
    if( cmd.getValue( 'y' ) != NULL  || cmd.getValue( "yLimit" ) != NULL  ){
      NUM_NODES = atoi(cmd.getValue('y')); 
    }
    else{
      std::cout << "USING DEFAULT: yLimit" << std::endl;
      FIELD_SIZE_Y = 1000;
    }
    
    if( cmd.getValue( 'x' ) != NULL  || cmd.getValue( "xLimit" ) != NULL  ){
      SAMPLES_NUMBER = atoi(cmd.getValue('x')); 
    }
    else{
      std::cout << "USING DEFAULT: xLimit" << std::endl;
      FIELD_SIZE_X = 1000;
    }
    
    if( cmd.getValue( 'n' ) != NULL  || cmd.getValue( "nodes" ) != NULL  ){
      NUM_NODES = atoi(cmd.getValue('n')); 
    }
    else{
      std::cout << "REQUIRED: number of nodes" << std::endl;
      std::cout << endl ;
      std::terminate();
    }
   
    if( cmd.getValue( 'r' ) != NULL  || cmd.getValue( "speed" ) != NULL  ){
      NUM_NODES = atof(cmd.getValue('r')); 
    }
    else{
      std::cout << "USING DEFAULT: speed" << std::endl;
      SPEED_NODES = 0.4;      
    }
    
    if( cmd.getValue( 'd' ) != NULL  || cmd.getValue( "duration" ) != NULL  ){
      DURATION = atoi(cmd.getValue('d')); 
    }
    else{
      std::cout << "USING DEFAULT: duration" << std::endl;
      DURATION = 1000;     
    }
    
    if( cmd.getValue( 'p' ) != NULL  || cmd.getValue( "probability" ) != NULL  ){
      DURATION = atoi(cmd.getValue('p')); 
    }
    else{
      std::cout << "USING DEFAULT: probability" << std::endl;
      PROB = 5;     
    }
    
        
    if( cmd.getValue( 'X' ) != NULL  || cmd.getValue( "deltaX" ) != NULL  ){
      DURATION = atoi(cmd.getValue('X')); 
    }
    else{
      std::cout << "USING DEFAULT: deltaX" << std::endl;
      DELTA_X = FIELD_SIZE_X / (NUM_NODES * 2);     
    }
           
    if( cmd.getValue( 'Y' ) != NULL  || cmd.getValue( "deltaY" ) != NULL  ){
      DURATION = atoi(cmd.getValue('Y')); 
    }
    else{
      std::cout << "USING DEFAULT: deltaY" << std::endl;
      DELTA_Y = 5;     
    }
    
    if( cmd.getValue( 'f' ) != NULL  || cmd.getValue( "file" ) != NULL  ){
      FILE_NAME = (cmd.getValue('f')); 
    }
    else{
      std::cout << "USING DEFAULT: file name" << std::endl;
      FILE_NAME = "scenario" ;     
    }
    
    if( cmd.getValue( 'S' ) != NULL  || cmd.getValue( "seed" ) != NULL  ){
      SEED = atoi(cmd.getValue('S')); 
    }
    else{
      std::cout << "no SEED specified, using random" << std::endl;
      SEED = 0;     
    }

     DOG_SPEED = (SINE_ARC_LENGTH*DELTA_X*SPEED_NODES)/(16*PI);//16PI is the distance travel by the ff while cos(y/8)
     //TODO: DOG_SPEED*timeStep is the minumm sensibility for the return to firefighter
     //According to the internet an average speed would be 11 m/s, that is running. But do the dogs run or walk or gallop or?
     std::cout << "Average speed of dogs: " << DOG_SPEED << "m/s. Please check that this is a sane value." << endl;
     
    if( cmd.getValue( 'D' ) != NULL  || cmd.getValue( "deviation" ) != NULL  ){
      STD_DEVIATION = atof(cmd.getValue('D')); 
    }
    else{
      std::cout << "USING DEFAULT: deviation" << std::endl;
      STD_DEVIATION = DOG_SPEED/2;     
    }
     
     std::cout << endl ;

    /* 8. DONE */
    //delete opt;
 
}


#endif//PARAMETERS.H