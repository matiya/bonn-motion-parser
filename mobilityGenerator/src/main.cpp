/**
 * @file main.cpp
 * @author Matias Siracusa
 * @version
 * @brief Simulates the movement of a rescue team
 * @date 22.02.2014
 * @bugs: 
 *	  * Update sineScale according to ff position
 * 	  * The angle nomenclature can't be understood, please fix asap.
 *        * In the obstacle thingie, the FIELD_X and FIELD_Y size are important, check
 * @todo: * put the name of the nodes in the class constructor so that the two
 * 	    switches can be avoided
 * 	  * Make the amplitude of the sine wave variable
*/

/**
 * @mainpage mobilityModel
 *
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <random>
#include <sstream>
#include <string>
#include "node.h"
#include "obstacle.h"
#include "parameters.h"
#include "vector2D.h"

#define POS_X 1000
#define POS_Y 1000


/*temp variables*/
double angulo, lx, ly, t, px, py, velY;

/**
 * @brief returns a random integer between 0 and n
 * @param generator ...
 * @param n ...
 * @return int
 */
int random_integer(std::mt19937 &generator, int n) 
{
  std::uniform_int_distribution<int> dist_n(0, n);
  return dist_n(generator);
}

float random_float(std::mt19937 &generator, float n) 
{
  std::uniform_real_distribution<float> dist_n(0, n);
  return dist_n(generator);
}


/**
  * @brief returns a random double drawn from a normal distribution
  *
  * @param generator ${p_generator:...}
  * @param mean mean of the normal distribution
  * @param stdDeviation standar deviation of the normal distribution
  * @return double random positive double
  */
double normal_integer(std::default_random_engine &generator, double mean,
                      double stdDeviation) 
{
  std::normal_distribution<double> distribution(mean, stdDeviation);
  return std::abs(distribution(generator));
}

/**
 * @brief Calculate the distance between two points
 * 
 * @return double
 */
double distanceBetween2Points(const double x1, const double y1, const double x2, const double y2)
{
  return std::sqrt( std::pow(x1 - x2, 2) + std::pow(y1 - y2,2));
}

/**
 * @brief returns the closest vertex of an obstacle to a given node
 *
 * When a node detects that an obstacle lies ahead of it,
 * it will look for the closes edge of said obstacle in order
 * to avoid colliding with it. If next to said edge there is another
 * obstacle, it will return then the other edge of the 1st obstacle.
 * It also checks that the path it would have to take to avoid the
 * obstacle lies within the field.
 *
 * @param firefighter firefighter ahead of whom the node lies
 * @param obstacle obstacle ahead of the node
 * @param q quantity of obstacles
 * @param i index of the fiven ff-node in the node array
 * @return Obstacle::vertex
 * @bug not well tested
 * @bug change obstacles to polygons
 */
Obstacle::vertex calcClosestVertex(Node firefighter, Obstacle *obstacle,
                                   unsigned int q, unsigned int i) 
{
  Obstacle::linePointParameters distanceStruct;
  Obstacle::vertex vertex, tempVertex;
  vertex =
      (obstacle + i)->getClosestVertex(*(firefighter.getPosition().end() - 3),
                                       *(firefighter.getPosition().end() - 2));
  do {
    for (int m = 0; m < q; m++) {
      // check if there are obstacles near the calc'd vertex
      if (m != i) { // don't check against itself
        if (vertex.x > *(firefighter.getPosition().end() - 3)) {
          distanceStruct =
              (obstacle + m)->getDistanceToPoint(vertex.x + 5, vertex.y - 5);
          tempVertex = (obstacle + i)->getClosestVertex(
              *(firefighter.getPosition().end() - 3) - (FIELD_SIZE_X),
              *(firefighter.getPosition().end() - 2));
        } else {
          distanceStruct =
              (obstacle + m)->getDistanceToPoint(vertex.x - 5, vertex.y - 5);
          tempVertex = (obstacle + i)->getClosestVertex(
              *(firefighter.getPosition().end() - 3) + (FIELD_SIZE_X),
              *(firefighter.getPosition().end() - 2));
        }
        if (std::abs(distanceStruct.distance) < 20 && distanceStruct.r >= 0.0 &&
            distanceStruct.r <= 1.0) {
//           std::cout << "obstacle " << m << " it's close to obstacle " << i
//                     << " at " << vertex.x << "," << vertex.y << std::endl;
          if (vertex.x == tempVertex.x && vertex.y == tempVertex.y) {
            if (vertex.x > *(firefighter.getPosition().end() - 3)) {
              vertex = (obstacle + m)->getClosestVertex(
                  *(firefighter.getPosition().end() - 3) + FIELD_SIZE_X,
                  *(firefighter.getPosition().end() - 2));
            } else {
              vertex = (obstacle + m)->getClosestVertex(
                  *(firefighter.getPosition().end() - 3) - FIELD_SIZE_X,
                  *(firefighter.getPosition().end() - 2));
            }
          } else {
            vertex.x = tempVertex.x;
            vertex.y = tempVertex.y;
          }
        }
      }
    }
  } while (std::abs(distanceStruct.distance) > 20 &&
           distanceStruct.r < 0.00001 && distanceStruct.r > 1.0);
  return vertex;
}
  /**
   * @brief Write tests' results to a file
   * 
   * Write output to a file in the ns3 trace format and to another one that
   * will store info without formatting
   * 
   * @param ff nodes
   * @param dog nodes
   * @return void
   */
void outputToFile( Node *ff, Node *dog)
{
  int k = 0;

  /*check if file exists already, if it does, create a new one appending
   * an increasing counter to the end of the file: xx0, xx1, xx2 ...*/
  while(FILE *file = fopen((FILE_NAME + std::to_string(k)).c_str(), "r")) {
    k++;
    fclose(file);
  }
  std::ofstream resultsFile(FILE_NAME + std::to_string(k));
  std::ofstream nsFile(FILE_NAME + "_ns" + std::to_string(k)) ; /*file to feed into simulator*/

  /*writing output to a file in the ns3 trace format and to another one that
   * will store the info without format */
  for (int j = 0; j < NUM_NODES; j++) { // circle through nodes
    for (int i = 0; i < ff[j].getPosition().size();
         i++) { // circle through node's vectorPosition content
      resultsFile << ff[j].getPosition().at(i) << " ";
    }
    resultsFile << "\n";
    for (int i = 0; i < dog[j].getPosition().size();
         i++) { // circle through node's vectorPosition content
      resultsFile << dog[j].getPosition().at(i) << " ";
    }
    resultsFile << "\n";
  }
  for (int j = 0; j < NUM_NODES * 2; j++) { // circle through nodes, all of them
    for (int i = 0; i < dog[0].getPosition().size();
         i += 4) { // circle through node's vectorPosition content
      if (j < NUM_NODES) { // dogs
        if (i == 0) { // write initial position
          nsFile << "$node_(" << j << ") set X_ "
                 << dog[j].getPosition().at(i + 1) << "\n";
          nsFile << "$node_(" << j << ") set Y_ "
                 << dog[j].getPosition().at(i + 2) << "\n";
        } else {
          nsFile << "$ns_ at " << dog[j].getPosition().at(i) << " \"$node_("
                 << j << ") setdest " << dog[j].getPosition().at(i + 1) << " "
                 << dog[j].getPosition().at(i + 2) << " "
                 << dog[j].getPosition().at(i + 3) << "\"" << "\n";
        }
      } else { // firefighters
        if (i == 0) { // write initial position
          nsFile << "$node_(" << j << ") set X_ "
                 << ff[j - NUM_NODES].getPosition().at(i + 1)
                 << "\n";
          nsFile << "$node_(" << j << ") set Y_ "
                 << ff[j - NUM_NODES].getPosition().at(i + 2)
                 << "\n";
        } else {
          nsFile << "$ns_ at " << ff[j - NUM_NODES].getPosition().at(i)
                 << " \"$node_(" << j << ") setdest "
                 << ff[j - NUM_NODES].getPosition().at(i + 1) << " "
                 << ff[j - NUM_NODES].getPosition().at(i + 2) << " "
                 << ff[j - NUM_NODES].getPosition().at(i + 3) << "\""
                 << "\n";
        }
      }
    }
    
  }
  nsFile.close();
  resultsFile.close();
}

float calcSpeedDogs( );

int main(int argc, char **argv)
{
  
  /*Index appended to file name*/
  int k = 0; 					
  /*vector with the obstacles' vertices*/
  std::vector<float> segment =
      askForParameters(argc, argv);	 	
  /*number of obstacles*/
  const unsigned int obstaclesNumber =
      segment.size() / 4; 			
  /*time of the simulation*/
  double time = 0;
  /*previous position of node*/
  double prevPosYFF = 0;
  double prevPosXFF = 0;
  double prevPosXD = 0;
  double prevPosYD = 0;
  /*previous speed of dog*/
  double prevDogSpeed = 0;
  /*node random deviation along x and y axis*/
  double yDeviation = 0;
  double xDeviation = 0;
  /*speed of nodes*/
  double dogSpeed = 0;
  double ffSpeed = 0; 				
  /*min and max speed change per turn*/
  double minVarSpeed = 0;
  double maxVarSpeed = 0;
  /*watchdog*/
  unsigned int wd = 0;
  /*structure with info about the obstacles' position*/
  Obstacle::linePointParameters
  distanceToPoint; 
  /*closest vertex a node will head to avoid an obstacle*/
  Obstacle::vertex closestVertex; 
  /*each cycle the simulation time will
      be increased by this quantity*/
  const double timeStep =
      (double)DURATION / SAMPLES_NUMBER; 

  /*create the seed for the random number generator
   * if the user didn't specify a seed, then a random one
   * will be used*/
  std::random_device rd;
  int seedForGen;
  SEED == 0 ? seedForGen = rd() : seedForGen = SEED;
  std::mt19937 gen(seedForGen);
  std::default_random_engine gen2(seedForGen);

  Node dog[NUM_NODES];
  Node firefighter[NUM_NODES];
  Obstacle obstacle[obstaclesNumber];

  /*initialize obstacles*/
  if (segment.size() != 0) {
    int i = 0;
    for (int j = 0; j < obstaclesNumber; j++) {
      obstacle[j].setVertices(segment.at(i), segment.at(i + 1),
                              segment.at(i + 2), segment.at(i + 3));
      i += 4;
    }
  }
  /*set the intial positions and speed of the nodes*/
  for (int i = 0; i < NUM_NODES; i++) {
    dog[i].setPosition(0, 
		       INITIAL_X + DELTA_X * i , 
		       INITIAL_Y + DELTA_Y * i, 
		       SPEED_NODES);
    dog[i].state = SINE;
    firefighter[i].setPosition(0, 
			       INITIAL_X + DELTA_X * i, 
			       INITIAL_Y + DELTA_Y * i, 
			       SPEED_NODES); //FIXME: spread evenly along y axis when angle = 0
    firefighter[i].state = LINEAR;
  }
  std::cout << "[I] Timestep: " << timeStep << std::endl;
  /*update nodes positions*/
  while (time < DURATION) {
    /*increase time counter*/
    time += timeStep;
    /*load position at t-1 from the corresponding node's vectorPosition*/
    for (int i = 0; i < NUM_NODES; i++){
      try {
        prevPosYFF = *(firefighter[i].getPosition().end() - 2);
        prevPosXFF = *(firefighter[i].getPosition().end() - 3);
        prevPosXD = *(dog[i].getPosition().end() - 3);
        prevPosYD = *(dog[i].getPosition().end() - 2);
	prevDogSpeed = dog[i].getPosition().back();
      }
      catch (std::exception &e) {
        std::cout << " [E] Error whilst accesing previous positions" << std::endl;
      }
      
      /*output progress*/
      if( (int) (time * 100/DURATION) % 10 == 0 and i == 0 
	and (int) (time * 100/DURATION) != (int) ((time-timeStep) * 100/DURATION))
	std::cout << "[I] " << (int) time * 100/DURATION << " % done time: " << time << std::endl;
      
      /*calculate distance from node to obstacle and set status to avoid in case
       * it lies within line of sight and directly ahead*/
      if (obstaclesNumber !=0 and firefighter[i].state != AVOIDING_OBSTACLE) {
        for (int j = 0; j < obstaclesNumber; j++) {
          distanceToPoint =
              obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
          /*distance within line of sight and ahead of node*/
          if (distanceToPoint.distance < LOS && distanceToPoint.distance > 0 &&
              distanceToPoint.r > 0 && distanceToPoint.r < 1) {
            if (verbose)
              std::cout << "[I](@ " << time << "s)  Node " << i
                        << " in (" << (int)prevPosXFF << ","
                        << (int)prevPosYFF << ") will collide with obstacle "
                        << j << " in " << (int)distanceToPoint.distance
			<< "m.\n    SINE -> AVOIDING_OBSTACLE"
                        << std::endl;
            closestVertex =
                calcClosestVertex(firefighter[i], obstacle, obstaclesNumber, j);
            /*if closest vertex is on the right of node, set the direction
             * to avoid it by 20 meters to the rigth of said vertex*/
            if (closestVertex.x >= prevPosXFF) {
              firefighter[i].calcVersor(closestVertex.x + 20,
                                        closestVertex.y);
              dog[i].calcVersor(closestVertex.x + 20, closestVertex.y);
            }
            /*if closest vertex is on the left of node, set the direction
             * to avoid it by 20 meters to the left of said vertex*/
            else {
              firefighter[i].calcVersor(closestVertex.x - 20, closestVertex.y);
              dog[i].calcVersor(closestVertex.x - 20, closestVertex.y);
            }
            firefighter[i].state = AVOIDING_OBSTACLE;
            dog[i].state = AVOIDING_OBSTACLE;
            /*save current position so that later the node can return to its
             * original position*/
            firefighter[i].previousPosition.x = prevPosXFF;
            firefighter[i].previousPosition.y = prevPosYFF;
            dog[i].previousPosition.x = prevPosXD;
            dog[i].previousPosition.y = prevPosYD;
          }
        }
      }
      /*Get the maximum and minimum variation of the speed*/
      if(time > timeStep and STD_DEVIATION != 0 and dog[i].state == SINE){
	if(prevDogSpeed < DOG_SPEED - STD_DEVIATION or
	  prevDogSpeed > DOG_SPEED + STD_DEVIATION){
	  minVarSpeed = DOG_SPEED - STD_DEVIATION;
	  maxVarSpeed = DOG_SPEED + STD_DEVIATION;
	}
	else{
	  minVarSpeed = ( (time - timeStep) * prevDogSpeed) / time;
	  maxVarSpeed = (prevDogSpeed - minVarSpeed) + prevDogSpeed;
	}	  
      }
      else{
	minVarSpeed = DOG_SPEED - STD_DEVIATION;
	maxVarSpeed = DOG_SPEED + STD_DEVIATION;
      }
      /*The node's speed has been changed, they are either speeding up or
       slowing down for a fixed amount of time*/
      if (firefighter[i].powerUpTime > 0) {
        firefighter[i].powerUpTime -= timeStep;
        ffSpeed = normal_integer(gen2, firefighter[i].alteredSpeed,
                                 STD_DEVIATION / 10);
	do{
	  dogSpeed = normal_integer(gen2, dog[i].alteredSpeed, STD_DEVIATION);
	}while ( dogSpeed < minVarSpeed or 
		 dogSpeed > maxVarSpeed or
	         dogSpeed > DOG_SPEED + STD_DEVIATION  or
	         dogSpeed < DOG_SPEED - STD_DEVIATION
	       );//see note below
      } 
      else {
        /*With a probability of 5% a node will slow down or speed up
	 * by a factor of up to plus minus STD_DEVIATION during a random time 
	 * no larger than 50 seconds.
	 */
        if (random_integer(gen, 100) < 5 and time > timeStep) {
	  do{  
	  double randomSpeed = random_float(gen, 2*STD_DEVIATION) - STD_DEVIATION; 
	  firefighter[i].alteredSpeed =
              normal_integer(gen2, SPEED_NODES, STD_DEVIATION / 10) +
              randomSpeed / 4;
          dog[i].alteredSpeed =
              normal_integer(gen2, DOG_SPEED, STD_DEVIATION) + randomSpeed;
          firefighter[i].powerUpTime =
              random_integer(gen, 50);
	  }while (dog[i].alteredSpeed < minVarSpeed or 
		  dog[i].alteredSpeed > maxVarSpeed or 
		  dog[i].alteredSpeed > DOG_SPEED + STD_DEVIATION or
		  dog[i].alteredSpeed < DOG_SPEED - STD_DEVIATION
		 );//see note below
      }
        /*calculate speed of dogs, taking into account that change of speed shouldn't be
	 too abrupt nor should it exceed the given DOG_SPEED + STD_DEVIATION*/
        do {
	  dogSpeed = normal_integer(gen2, DOG_SPEED, STD_DEVIATION);
// 	  dogSpeed += (dogSpeed - prevDogSpeed) / prevDogSpeed; //FIXME: add
	  } while (dogSpeed < minVarSpeed or 
		   dogSpeed > maxVarSpeed or
	           dogSpeed > DOG_SPEED + STD_DEVIATION  or
	           dogSpeed < DOG_SPEED - STD_DEVIATION 
		  );
 	/*dog is far ahead of ff, decrease the speed so ff can catch up*/
 	if((prevPosYD-prevPosYFF) > 10){
	  if( minVarSpeed <= DOG_SPEED - STD_DEVIATION){
	    dogSpeed = maxVarSpeed;
	    dog[i].alteredSpeed = maxVarSpeed;
	  } else {
	    dogSpeed = minVarSpeed;
	    dog[i].alteredSpeed = minVarSpeed;
	  }
 	}

        /*calculate speed of ff*/
        ffSpeed = normal_integer(gen2, SPEED_NODES, STD_DEVIATION / 10);

        /*issue a warning if the speed of the firefigther is unrealistic*/
        if (ffSpeed > 1.4 * 1.2) { // 20% over average walking speed
          std::cout << "[W] (@ " << time <<"s) FF " << i << " is moving at " << ffSpeed
                    << "m/s.\n    This is higher than the average of 1.4m/s"
                    << std::endl;
        }
      }
      /*Indicate the original movement of firefighter, i.e. to the right*/
      firefighter[i].directionVector.x = ffSpeed*timeStep;
      firefighter[i].directionVector.y = 0; //FIXME: make this coherent with the rest
      /*update position of firefighters*/

      /*flags to indicate whether or not a node can return
      * to its original position when avoiding an obstacle*/
      bool canReturn = true;
      bool canReturnFF = true;

      switch (firefighter[i].state) {
      /*The firefighter advances in a straight line with a probability of PROB
       * per mil of deviating up to
       * 2.5 meters in either direction along the x axis
       */
      case LINEAR:
        if (random_integer(gen, 1000) < PROB) {
	  xDeviation = random_integer(gen, 200) / 100; 
          if (firefighter[i].xDeviation >= 0 && (-xDeviation + prevPosXFF) > 0)
            xDeviation = -1 * std::abs(xDeviation);
          else if (firefighter[i].xDeviation < 0)
            xDeviation = 1 * std::abs(xDeviation);
          firefighter[i].xDeviation = xDeviation;
        } else {
          xDeviation = 0;
        }
	firefighter[i].directionVector.Rotate(ANGLE + PI/2);
        firefighter[i].setPosition(time, 
				   prevPosXFF + firefighter[i].directionVector.x,
                                   prevPosYFF +  firefighter[i].directionVector.y, 
				   ffSpeed);

        break;

      /*When the FF is avoiding an obstacle, it heads into the vertex previously
       *calculated in the calcClosestVertex
       */
      case AVOIDING_OBSTACLE:
        /*check if current position is close to goal position*/
	if( distanceBetween2Points( firefighter[i].getNextPosition()[0], firefighter[i].getNextPosition()[1],
				    prevPosXFF, prevPosYFF) 
	  < timeStep * ffSpeed + STD_DEVIATION){
          if (verbose)
            std::cout << "[I] (@" << time << "s) FF " << i
                      << " reached goal position."
		      << "\n    Will try to return to previous position." 
		      << "\n    AVOIDING_OBSTACLE -> LINEAR_AVOIDING_OBSTACLE"
		      << std::endl;
          firefighter[i].state = LINEAR_AVOIDING_OBSTACLE;
        }
        firefighter[i].setPosition(
            time, 
	    prevPosXFF + *(firefighter[i].getVersor().begin()) * timeStep * ffSpeed,
            prevPosYFF + *(firefighter[i].getVersor().begin() + 1) * timeStep * ffSpeed,
            ffSpeed);
        break;

      /*The node advances in a straight line. In each timestep it checks if
       *there are no obstacles
       *that could interfer the way back to the original position.*/
      case LINEAR_AVOIDING_OBSTACLE:
        for (int j = 0; j < obstaclesNumber; j++) {
          distanceToPoint =
              obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
          /*goes right and there are no obstacles between the current position
           * and the position the node must return to*/
          if (((prevPosXFF < firefighter[i].previousPosition.x ) &&
               obstacle[j].getVertices().x1 > prevPosXFF &&
               obstacle[j].getVertices().x2 > prevPosXFF &&
               obstacle[j].getVertices().y1 < prevPosYFF &&
               obstacle[j].getVertices().y2 > prevPosYFF &&
               obstacle[j].getVertices().x1 <
                   firefighter[i].previousPosition.x &&
               obstacle[j].getVertices().x1 <
                   firefighter[i].previousPosition.x ) ||
              std::abs(prevPosYFF -
                       *(firefighter[i].getNextPosition().begin() + 1)) < 6) {
            canReturnFF = false;
          }
          /*goes left and there are no obstacles between the current position
             and the position the node must return to*/
          else if (((prevPosXFF > firefighter[i].previousPosition.x ) &&
                    obstacle[j].getVertices().x1 < prevPosXFF &&
                    obstacle[j].getVertices().x2 < prevPosXFF &&
                    obstacle[j].getVertices().y1 < prevPosYFF &&
                    obstacle[j].getVertices().y2 > prevPosYFF &&
                    obstacle[j].getVertices().x1 >
                        firefighter[i].previousPosition.x &&
                    obstacle[j].getVertices().x1 >
                        firefighter[i].previousPosition.x) ||
                   std::abs(
                       prevPosYFF -
                           *(firefighter[i].getNextPosition().begin() + 1) <
                       6)) {
            canReturnFF = false;
          }
        }
        if (canReturnFF) {
          if (verbose)
            std::cout << "[I] (@" << time << ") FF " << i
                      << " can return through " << prevPosYFF 
                      << "\n    LINEAR_AVOIDING_OBSTACLE -> RETURNING "
                      << std::endl;
          firefighter[i].calcVersor(firefighter[i].previousPosition.x  
				      + (prevPosYFF - firefighter[i].previousPosition.y)/ std::tan(ANGLE + PI/2),
                                    prevPosYFF);
          firefighter[i].state = RETURNING;
        }
        firefighter[i].setPosition(time, 
				   prevPosXFF,
                                   prevPosYFF + timeStep * ffSpeed, 
				   ffSpeed);
        break;

      /*When the node has reached the goal position that allows it to avoid an
       *obstacle, it will advance in a straight line along the x axis to its 
       * original x position, mantaining its current y position*/
      case RETURNING:
	if( distanceBetween2Points(firefighter[i].getNextPosition()[0], firefighter[i].getNextPosition()[1],
				   prevPosXFF, prevPosYFF)
	  < timeStep * ffSpeed + STD_DEVIATION){
          if (verbose)
            std::cout
                << "[I] (@" << time << ") FF " << i
                << " reached goal position.\n    RETURNING -> LINEAR."
                << std::endl;
          firefighter[i].state = LINEAR;
        }
        firefighter[i].setPosition(
            time, 
	    prevPosXFF + *(firefighter[i].getVersor().begin()) *
                                   timeStep * ffSpeed,
            prevPosYFF +
                *(firefighter[i].getVersor().begin() + 1) * timeStep * ffSpeed,
            ffSpeed);
        break;
      }

      switch (dog[i].state) {
      /*The dog moves following a sine wave*/
      case SINE:
        if (random_integer(gen, 1000) < PROB and
	    time - dog[i].refractoryPeriod > 10) {
          dog[i].state = ASTRAY;
          if (verbose) 
            std::cout << "[I] (@" << time << "s dog[" << i
                      << "] has gone astray"
		      << "\n    SINE -> ASTRAY"
                      << std::endl;
          /*calculate new random position, making sure it doesn't go out of
           * bounds and that the new position doesn't lie within line of sight*/
          do {
            yDeviation =
                random_float(gen, 200) - 100; 
            xDeviation = random_float(gen, 200) - 100;
          } while ( /*(xDeviation + prevPosXD)> FIELD_SIZE_X or*/ //FIXME: Put meaninful field limits
		   distanceBetween2Points (xDeviation + prevPosXD, yDeviation + prevPosYD,
					      prevPosXD, prevPosYD) < LOS);

          /*calculate the unit vector that points in the direction of the deviation*/
          dog[i].calcVersor(xDeviation + prevPosXD, yDeviation + prevPosYD);
          if (verbose) {
	    std::cout << "    (" << prevPosXD <<"," << prevPosYD << ") -> ( "
		      << xDeviation + prevPosXD << ","
                      << yDeviation + prevPosYD << ") @ (" 
		      << *(dog[i].getVersor().begin()) << ","
                      << *(dog[i].getVersor().begin() + 1) << ")"
                      << std::endl;
          }
        } else {

	  /*if dog is departed from firefighter catch up*/
	  if( distanceBetween2Points(prevPosXD, prevPosYD, prevPosXFF, prevPosYFF) > AMPLITUDE){
	    /*going up*/
	    if( (ANGLE + PI/2) > 0)
	      if(prevPosYD > (prevPosYFF + AMPLITUDE) )
		dog[i].scaleSine += 0.7*timeStep;
	      else if( prevPosYD <= (prevPosYFF + AMPLITUDE/2) )
		dog[i].scaleSine += 1.5*timeStep;
	      
	    /*going right*/
	    if( (ANGLE + PI/2) == 0)
	      if(prevPosXD > (prevPosXFF + AMPLITUDE) )
		dog[i].scaleSine += 0.7*timeStep;
	      else if(prevPosXD < (prevPosXFF + AMPLITUDE/2) )
		dog[i].scaleSine += 1.5*timeStep;
	      
	    /*going down*/
	    if( (ANGLE + PI/2) < 0)
	      /*make sure the dogs follow within a distance of AMPLITUDE/2 to AMPLITUDE*/
	      if( prevPosYD <= (prevPosYFF - AMPLITUDE))
		dog[i].scaleSine += 0.7*timeStep;
	      else if( prevPosYD > (prevPosYFF - AMPLITUDE/2))
		dog[i].scaleSine += 1.5*timeStep;
	      
	    /*going left*/
	    if(abs (ANGLE - PI/2) < 0.1)
	      if(prevPosXD > (prevPosXFF - AMPLITUDE/2) )
		dog[i].scaleSine += 1.5*timeStep;
	      else if(prevPosXD <= (prevPosXFF - AMPLITUDE) )
		dog[i].scaleSine += 0.7*timeStep;
	  } else{
	    dog[i].scaleSine += timeStep; 
	  }
	  //FIXME: This is set based on sheer experience
	  py = dog[i].scaleSine * dogSpeed * 0.2;
// 	  if(py < prevPosYD) //FIXME
// 	    std::cout << py - prevPosYD << "\t\t\t		dude!!! " << std::endl;
	  px = AMPLITUDE * std::sin(py/8);
	  
	  /*rotate the sine, and translate if it's not in the origin*/
	  lx = px * std::cos(ANGLE) - py * std::sin(ANGLE) + dog[i].getPosition()[1];
	  ly = px * std::sin(ANGLE) + py * std::cos(ANGLE) + dog[i].getPosition()[2];
	  	  
	  dog[i].calcVersor(lx, ly);
        }
	dog[i].setPosition(
            time,	
            prevPosXD + dog[i].getVersor()[0]*timeStep*dogSpeed, //TODO: replace with vector2d
            prevPosYD + dog[i].getVersor()[1]*timeStep*dogSpeed,
            dogSpeed);
        break;

      /*idem ff,but for dogs*/
      case AVOIDING_OBSTACLE:
        /*check that current position is close to goal position*/
	if( distanceBetween2Points( dog[i].getNextPosition()[0], dog[i].getNextPosition()[1],
				    prevPosXD, prevPosYD)
           < timeStep * ffSpeed + STD_DEVIATION) {
          if (verbose)
            std::cout << "[I] (@" << time << "s) Dog " << i
                      << " reached goal position.\n "
                      << "    Will try to return to previous position." 
		      << "\n    AVOIDING_OBSTACLE-> LINEAR_AVOIDING_OBSTACLE"
		      << std::endl;
          dog[i].state = LINEAR_AVOIDING_OBSTACLE;
        }
        dog[i].setPosition(
            time,
            prevPosXD + *(dog[i].getVersor().begin()) * timeStep * ffSpeed,
            prevPosYD + *(dog[i].getVersor().begin() + 1) * timeStep * ffSpeed,
            ffSpeed);

        break;

      /*ïdem ff, but for dogs*/
      case LINEAR_AVOIDING_OBSTACLE:
        for (int j = 0; j < obstaclesNumber; j++) {
          distanceToPoint =
              obstacle[j].getDistanceToPoint(prevPosXD, prevPosYD);
          /*goes right and there are no obstacles between the current position
           * and the position the node must return to*/
          if (((prevPosXD < dog[i].previousPosition.x) &&
               obstacle[j].getVertices().x1 > prevPosXD &&
               obstacle[j].getVertices().x2 > prevPosXD &&
               obstacle[j].getVertices().y1 < prevPosYD &&
               obstacle[j].getVertices().y2 > prevPosYD &&
               obstacle[j].getVertices().x1 <
                   dog[i].previousPosition.x &&
               obstacle[j].getVertices().x1 <
                   dog[i].previousPosition.x) ||
              std::abs(prevPosYD - *(dog[i].getNextPosition().begin() + 1)) <
                  6) {
            canReturn = false;
          }
          /*goes left and there are no obstacles between the current position
             and the position the node must return to*/
          else if (((prevPosXD > dog[i].previousPosition.x) &&
                    obstacle[j].getVertices().x1 < prevPosXD &&
                    obstacle[j].getVertices().x2 < prevPosXD &&
                    obstacle[j].getVertices().y1 < prevPosYD &&
                    obstacle[j].getVertices().y2 > prevPosYD &&
                    obstacle[j].getVertices().x1 >
                        dog[i].previousPosition.x &&
                    obstacle[j].getVertices().x1 >
                        dog[i].previousPosition.x) ||
                   std::abs(prevPosYD -
                                *(dog[i].getNextPosition().begin() + 1) <
                            6)) {
            canReturn = false;
          }
        }
        if (canReturn) {
          if (verbose)
            std::cout << "[I] (@" << time << ") Dog " << i
                      << " can return through " << prevPosYFF 
                      << "\n    LINEAR_AVOIDING_OBSTACLE -> RETURNING "
                      << std::endl;
          dog[i].calcVersor(dog[i].previousPosition.x 
				+ (prevPosYD - dog[i].previousPosition.y) / std::tan(ANGLE + PI/2)
			    ,prevPosYD);
          dog[i].state = RETURNING;
        }
        dog[i].setPosition(time, 
			   prevPosXD, 
			   prevPosYD + timeStep * ffSpeed,
                           ffSpeed);
        break;

      case RETURNING:
	if( distanceBetween2Points( dog[i].getNextPosition()[0], dog[i].getNextPosition()[1],
				    prevPosXD, prevPosYD) < 
	    timeStep * ffSpeed + STD_DEVIATION) {
          if (verbose)
            std::cout << "[I] (@" << time << ") Dog " << i
                      << " reached previous position.\n    RETURNING -> SINE"
                      << std::endl;
          dog[i].state = SINE; 
	  dog[i].scaleSine += 0.25*
	    (distanceBetween2Points(prevPosXD, prevPosYD, dog[i].previousPosition.x, dog[i].previousPosition.y))
	    /(timeStep*sin(ANGLE + PI/2));
        }

        dog[i].setPosition(time,
			   prevPosXD + *(dog[i].getVersor().begin()) * timeStep * ffSpeed,
			   prevPosYD + *(dog[i].getVersor().begin() + 1) * timeStep * ffSpeed,
			   ffSpeed
			  );
        break;

      /*dog has gone astray, running to a random position*/
      case ASTRAY:
	dog[i].scaleSine += timeStep; //FIXME
	if(distanceBetween2Points(dog[i].getNextPosition()[0], dog[i].getNextPosition()[1],
				  prevPosXD, prevPosYD) <
	   timeStep * DOG_SPEED + STD_DEVIATION){
        if (verbose)
            std::cout
                << "[I] (@" << time << ") dog[" << i
                << "] reached goal destination.\n    ASTRAY -> RETURNING_TO_FF"
                << std::endl;
          dog[i].state = RETURNING_TO_FF;
        }
        dog[i].setPosition(time,
			   prevPosXD + *(dog[i].getVersor().begin()) * timeStep * dogSpeed,
			   prevPosYD + *(dog[i].getVersor().begin() + 1) * timeStep * dogSpeed,
			   dogSpeed
			  );
        break;

      /*After leaving the ASTRAY state, the dog will take the shortest path to
       * return to the firefighter */
      case RETURNING_TO_FF:
	dog[i].scaleSine += timeStep; //FIXME
        dog[i].calcVersor(prevPosXFF, prevPosYFF);
        dog[i].setPosition(time,
			   prevPosXD + *(dog[i].getVersor().begin()) * timeStep * dogSpeed,
			   prevPosYD + *(dog[i].getVersor().begin() + 1) * timeStep * dogSpeed,
			   dogSpeed
			  );
	if( distanceBetween2Points(prevPosXD, prevPosYD, prevPosXFF, prevPosYFF) < 5 ){
          if (verbose)
            std::cout << "[I] (@" << time << ") dog[" << i
                      << "] has returned to firefighter\n" 
		      << "    RETURNING_TO_FF -> SINE" 
		      << std::endl;
          dog[i].state = SINE;
	  /*set the time in which the dog returned to the ff*/
	  dog[i].refractoryPeriod = time;
        }
        break;
      }
    }
  }
  outputToFile( firefighter, dog);
  return 0;
}
