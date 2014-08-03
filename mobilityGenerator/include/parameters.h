/**
 * @file node.h
 * @author Matias Siracusa
 * @version 
 * @brief Parses parameters from the cmd line and feeds them into the program
 * @date 22.02.2014
 * @bugs Set some more meaningful parameters and maybe change FIELD_SIZE_COORDS to DISTANCE_BETWEEN_NODES 
 * 
*/

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <cmath>
#include "anyoption.h"
#include "vector2D.h"

#define SINE_ARC_LENGTH 7.64
#define PI 3.14159265

Vector2D finalPosition;
unsigned long SAMPLES_NUMBER;
unsigned long DURATION;
unsigned long FIELD_SIZE_X;
unsigned long FIELD_SIZE_Y;
unsigned int AMPLITUDE;
unsigned int PROB;
unsigned int NUM_NODES;
unsigned int LOS;
double STD_DEVIATION;
double SPEED_NODES; 
double DOG_SPEED;
double INITIAL_X;
double INITIAL_Y;
double ANGLE;
int DELTA_X;
int DELTA_Y;
int SEED;
bool verbose;
std::string FILE_NAME; 
std::string OBSTACLES_FILE;


/**
 * @brief Reads the coordinates of the obstacles from a file, and stores them in a vector.
 * In addition it also checks that the coordinates are in the right format.
 * 
 * @param obstaclesFile File in which the obstacles are defined.
 * @return std::vector< float, std::allocator< void > > vector of floats with the coordinates of the obstacles.
 */
std::vector<float> readObstaclesFile(std::string obstaclesFile){
  std::ifstream f(obstaclesFile);
  std::string asd;
  std::vector<float> segment;
  
  /*Check that the obstacle file exist*/
  if(!f.good()){
    std::cout << "[E] Obstacle file does not exist. " << std::endl;
    exit(-1);
  }
  else{
    if(f.is_open()){
      while ( std::getline(f,asd, ',') )
      {
	segment.push_back(std::stof(asd.c_str()));
      }
      if( (segment.size() % 4) != 0)
      {
	std::cout << "[E] Incorrect format of obstacles." << std::endl;
	exit(-1);
      }
      std::cout << "[I] Read "<< segment.size()/4 << " obstacles. "<<std::endl;    
    }
    else{
      std::cout <<"[E] Could not open obstacle file: " << obstaclesFile << "." << std::endl;
      exit(-1);
    }
    f.close();
  }
  return segment;
}

/**
 * @brief Reads the parameters from the cmd line or from a file and feeds them into the program.
 * 
 * @param argc Argument count.
 * @param argv Argument value.
 * @return std::vector< float, std::allocator< void > > vector of floats with the coordinates of the obstacles.
 */
std::vector<float> askForParameters( int argc, char **argv){
 
    std::vector<float> segment;
    /* 1. CREATE AN OBJECT */
    AnyOption cmd;
    /* 2. SET PREFERENCES  */
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
    cmd.addUsage( " -I   --startx\t\tinitial position in x");
    cmd.addUsage( " -J   --starty\t\tinitial position in y");
    cmd.addUsage( " -i   --finalx\t\tfinal position in x");
    cmd.addUsage( " -j   --finaly\t\tfinal position in y");
    cmd.addUsage( " -r   --speed\t\tspeed of the firefighters");
    cmd.addUsage( " -X   --deltaX\t\tspacing between nodes across the X axis (in meters)");
    cmd.addUsage( " -Y   --deltaY\t\tspacing between nodes across the Y axis (in meters)");
    cmd.addUsage( " -A   --amplitude\t\tamplitude of the sine wave (in meters)");
    cmd.addUsage( " -l   --los\t\tlength of line of sight of the nodes (in meters)");
    cmd.addUsage( " -S   --seed\t\tseed, if set to 0 it will be random");
    cmd.addUsage( " -o   --obstacles\t\tobstacles file name");
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
    cmd.setOption(  "initialx", 'I' ); 
    cmd.setOption(  "initialy", 'J' ); 
    cmd.setOption(  "finalx", 'i' ); 
    cmd.setOption(  "finaly", 'j' ); 
    cmd.setOption(  "deltaX", 'X' ); 
    cmd.setOption(  "deltaY", 'Y' ); 
    cmd.setOption(  "amplitude", 'A' ); 
    cmd.setOption(  "los", 'l' ); 
    cmd.setOption(  "seed", 'S' ); 
    cmd.setOption(  "obstacles", 'o' ); 
    cmd.setFlag(  "verbose", 'v' );   /* a flag (takes no argument), supporting long and short form */ 
    
    /* for options that will be checked only on the command and line not in option/resource file */
    cmd.setCommandFlag(  "zip" , 'z'); /* a flag (takes no argument), supporting long and short form */

    /* for options that will be checked only from the option/resource file */
    cmd.setFileOption(  "title" ); /* an option (takes an argument), supporting only long form */

    /* 5. PROCESS THE COMMANDLINE AND RESOURCE FILE */

    /* read options from a  option/resource file with ':' separated opttions or flags, one per line */
    cmd.processFile( "options" );
    /* go through the command line and get the options  */
    cmd.processCommandArgs( argc, argv );
    
    if( ! cmd.hasOptions()) { /* print usage if no options */
	    cmd.printUsage();
	    exit(-1);
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
      std::cout << "[E] REQUIRED: number of samples" << std::endl;
      std::cout << endl ;
      exit(-1);
    }
    
    if( cmd.getValue( 'y' ) != NULL  || cmd.getValue( "yLimit" ) != NULL  ){
      FIELD_SIZE_Y = atoi(cmd.getValue('y')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: yLimit = 1000m" << std::endl;
      FIELD_SIZE_Y = 1000;
    }
    
    if( cmd.getValue( 'x' ) != NULL  || cmd.getValue( "xLimit" ) != NULL  ){
      FIELD_SIZE_X = atoi(cmd.getValue('x')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: xLimit = 1000m" << std::endl;
      FIELD_SIZE_X = 1000;
    }
    
    if( cmd.getValue( 'n' ) != NULL  || cmd.getValue( "nodes" ) != NULL  ){
      NUM_NODES = atoi(cmd.getValue('n')); 
    }
    else{
      std::cout << "[E] REQUIRED: number of nodes" << std::endl;
      exit(-1);
    }
   
    if( cmd.getValue( 'r' ) != NULL  || cmd.getValue( "speed" ) != NULL  ){
      SPEED_NODES = atof(cmd.getValue('r')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: speed = 1.4m/s" << std::endl;
      SPEED_NODES = 1.4;
    }
    
    if( cmd.getValue( 'd' ) != NULL  || cmd.getValue( "duration" ) != NULL  ){
      DURATION = atoi(cmd.getValue('d'));
    }
    else{
      std::cout << "[W] USING DEFAULT: duration = 1000s" << std::endl;
      DURATION = 1000;
    }
    
    if( cmd.getValue( 'p' ) != NULL  || cmd.getValue( "probability" ) != NULL  ){
      PROB = atoi(cmd.getValue('p')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: probability = 0.005" << std::endl;
      PROB = 5;
    }
    
        
    if( cmd.getValue( 'X' ) != NULL  || cmd.getValue( "deltaX" ) != NULL  ){
      DELTA_X = atoi(cmd.getValue('X')); 
    }
    else{
      DELTA_X = FIELD_SIZE_X / NUM_NODES;
      std::cout << "[W] USING DEFAULT: deltaX = " << DELTA_X << std::endl;
    }
           
    if( cmd.getValue( 'Y' ) != NULL  || cmd.getValue( "deltaY" ) != NULL  ){
      DELTA_Y = atoi(cmd.getValue('Y')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: deltaY = 0" << std::endl;
      DELTA_Y = 0;     
    }
    
    if( cmd.getValue( 'A' ) != NULL  || cmd.getValue( "amplitude" ) != NULL  ){
      AMPLITUDE = atoi(cmd.getValue('A')); 
    }
    else{
      AMPLITUDE = FIELD_SIZE_X / (NUM_NODES * 2);
      if(AMPLITUDE > 50)
	AMPLITUDE = 50;
      std::cout << "[W] USING DEFAULT: amplitude = " << AMPLITUDE << std::endl;
    }
    if( cmd.getValue( 'l' ) != NULL  || cmd.getValue( "los" ) != NULL  ){
      LOS = atoi(cmd.getValue('l')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: LoS = 80" << std::endl;
      LOS = 80;
    }

    if( cmd.getValue( 'f' ) != NULL  || cmd.getValue( "file" ) != NULL  ){
      FILE_NAME = (cmd.getValue('f')); 
    }
    else{
      std::cout << "[W] USING DEFAULT: file name \"scenario\"" << std::endl;
      FILE_NAME = "scenario" ;
    }
    
    if( cmd.getValue( 'o' ) != NULL  || cmd.getValue( "obstacles" ) != NULL  ){
      OBSTACLES_FILE = (cmd.getValue('o')); 
      segment = readObstaclesFile(OBSTACLES_FILE);
      
    }
    else{
      std::cout << "[W] no obstacles defined" << std::endl;
    }
    
    if( cmd.getValue( 'S' ) != NULL  || cmd.getValue( "seed" ) != NULL  ){
      SEED = atoi(cmd.getValue('S')); 
    }
    else{
      std::cout << "[W] no SEED specified, using random" << std::endl;
      SEED = 0;
    }

     DOG_SPEED = (SINE_ARC_LENGTH*AMPLITUDE*SPEED_NODES)/(16*PI);//FIXME: 16PI is the distance travel by the ff while cos(y/8)
     //FIXME: According to the internet an average speed would be 11 m/s, that is running. But do the dogs run or walk or gallop or?
     std::cout << "[W] Average speed of dogs: " << DOG_SPEED << "m/s. \n    Please check that this is a sane value. \n" << 
		   "    The average running speed of a dog lies between 7.8m/s and 13.6m/s." << std::endl;
     if(DOG_SPEED > 13.6){
      std::cout << "[E] Average speed of dogs surpasses the maximum of 13.6m/s" << 
		    "\n    Decrease either deltaX or the speed." << std::endl;
      exit(-1);
     }
    if( cmd.getValue( 'D' ) != NULL  || cmd.getValue( "deviation" ) != NULL  ){
      STD_DEVIATION = atof(cmd.getValue('D')); 
    }
    else{
      STD_DEVIATION = DOG_SPEED/2;
      std::cout << "[W] USING DEFAULT: deviation =" << STD_DEVIATION << std::endl;
    }
    if( cmd.getValue( 'I' ) != NULL  || cmd.getValue( "initialx" ) != NULL  ){
      INITIAL_X = atof(cmd.getValue('I')); 
    }
    if( cmd.getValue( 'J' ) != NULL  || cmd.getValue( "initialy" ) != NULL  ){
      INITIAL_Y = atof(cmd.getValue('J')); 
    }
    if( cmd.getValue( 'i' ) != NULL  || cmd.getValue( "finalx" ) != NULL  ){
      finalPosition.x = atof(cmd.getValue('i')); 
    }
    if( cmd.getValue( 'j' ) != NULL  || cmd.getValue( "finaly" ) != NULL  ){
      finalPosition.y = atof(cmd.getValue('j')); 
    }

    /* get angle from origin to final position*/ //FIXME: take into account start position
     ANGLE = std::atan2(finalPosition.y, finalPosition.x);
     if(INITIAL_X == 0 and INITIAL_Y == 0)
       std::cout << "[W] USING DEFAULT: initial position (0,0) " << std::endl;
     std::cout << "[I] The nodes will travel with an angle from the x axis of: " << 
		  ANGLE*360/(2*PI)  << "°" << std::endl;
    ANGLE -= PI/2;
    return segment;
    
}

#endif//PARAMETERS.H