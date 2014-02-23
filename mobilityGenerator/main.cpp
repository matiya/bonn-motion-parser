/**
 * @file main.cpp
 * @author Matias Siracusa
 * @version 
 * @brief Simulates the movemnet of a rescue team
 * @date 22.02.2014
 * @bugs: put the name of the nodes in the class constructor so that the two swithces can be avoided
*/

/**
 * @mainpage mobilityModel
 * 
 */


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



/**
 * @brief returns a random integer between 0 and n
 * @param generator ...
 * @param n ...
 * @return int
 */
int random_integer(std::mt19937& generator, int n){
    std::uniform_int_distribution<int> dist_n(0, n);
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
double normal_integer(std::default_random_engine& generator, double mean, double stdDeviation){

  std::normal_distribution<double> distribution(mean, stdDeviation);
  return std::abs(distribution(generator));
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
Obstacle::vertex calcClosestVertex(Node firefighter, Obstacle* obstacle, unsigned int q, unsigned int i){

  Obstacle::linePointParameters distanceStruct;
  Obstacle::vertex vertex, tempVertex;
  vertex = (obstacle + i)->getClosestVertex(*(firefighter.getPosition().end()-3),*(firefighter.getPosition().end()-2));
  do{
  for(int m = 0; m < q ; m++){
    //check if there are obstacles near the calc'd vertex
    if(m!=i){//don't check against itself
      if(vertex.x > *(firefighter.getPosition().end()-3) ){
	distanceStruct = (obstacle+m)->getDistanceToPoint(vertex.x+5, vertex.y-5);
	tempVertex = (obstacle + i)->getClosestVertex(*(firefighter.getPosition().end()-3)-(FIELD_SIZE_X),
						      *(firefighter.getPosition().end()-2));
      }
      else{
	distanceStruct = (obstacle+m)->getDistanceToPoint(vertex.x-5, vertex.y-5);
	tempVertex = (obstacle + i)->getClosestVertex(*(firefighter.getPosition().end()-3)+(FIELD_SIZE_X),
						      *(firefighter.getPosition().end()-2));
      }
      std::cout << "obstacle: " << i << " \tvertex: " << vertex.x<<","<< vertex.y<< " \ttempVertex: "<< tempVertex.x << "," <<tempVertex.y<< std::endl;
       std::cout << "Distance between " << i << " and " << m << " is " << distanceStruct.distance << "\tr: " << distanceStruct.r <<  std::endl;
      if(std::abs(distanceStruct.distance) < 20 && distanceStruct.r >= 0.0 && distanceStruct.r <= 1.0){
	std::cout << "obstacle " << m << " it's close to obstacle "<< i << " at " << vertex.x<<","<< vertex.y<< std::endl;
	if(vertex.x == tempVertex.x && vertex.y == tempVertex.y){
	  if(vertex.x > *(firefighter.getPosition().end()-3) ){
	    vertex =  (obstacle + m)->getClosestVertex(*(firefighter.getPosition().end()-3)+FIELD_SIZE_X,*(firefighter.getPosition().end()-2));
	  }
	  else{
	    vertex =  (obstacle + m)->getClosestVertex(*(firefighter.getPosition().end()-3)-FIELD_SIZE_X,*(firefighter.getPosition().end()-2));
	  }
	}
	else{
	  vertex.x = tempVertex.x;
	  vertex.y = tempVertex.y;
	}

      }
    }
    
  }}while( std::abs(distanceStruct.distance) > 20 && distanceStruct.r < 0.00001 && distanceStruct.r > 1.0 );
  return vertex;
  
}


int main(int argc, char **argv) {
  
  
  int k =0 ;
  std::vector<float> segment = askForParameters(argc, argv); 
  const unsigned int obstaclesNumber = segment.size()/4;
  double time = 0;
  double prevPosYFF = 0, prevPosXFF = 0, prevPosXD = 0, prevPosYD = 0;
  double yDeviation = 0, xDeviation = 0, xDir = 0, yDir = 0;
  double dogSpeed = 0, ffSpeed = 0;
  Obstacle::linePointParameters distanceToPoint;
  Obstacle::vertex closestVertex;
  const double rfDist = (double) FIELD_SIZE_X / NUM_NODES;
  const double timeStep = (double) DURATION/SAMPLES_NUMBER;  
  std::ofstream resultsFile, nsFile;
  std::ifstream f(FILE_NAME + std::to_string(k));
  
  /*check if file exists already, if it does, create a new one appending 
   * an increasing counter to the end of the file xx0, xx1, xx2 ...*/
  while (f.good()) {
    k++;
    f.close();
    f.open(FILE_NAME + std::to_string(k));
  }
  
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
  if(segment.size() != 0)
  {
    int i = 0;
    for ( int j = 0; j < obstaclesNumber ; j++){
      obstacle[j].setVertices(segment.at(i),segment.at(i+1),segment.at(i+2),segment.at(i+3)) ;
      i+=4; 
    }
  }
  /*set the intial positions and speed of the nodes*/
  for(int i = 0; i < NUM_NODES ; i++){
    dog[i].setPosition(0, DELTA_X + i*rfDist, 0, SPEED_NODES);
    dog[i].state = SINE;
    firefighter[i].setPosition(0, DELTA_X + i*rfDist, 0, SPEED_NODES);
    firefighter[i].state = LINEAR;
  }
  std::cout << "[I] Timestep: " << timeStep << std::endl;
  
  /*update nodes positions*/
  while(time < DURATION){
    /*increase time counter*/
    time += timeStep;
    
    /*load position at t-1 from the corresponding node's vectorPosition*/
    for(int i = 0; i<NUM_NODES; i++){
      int temp = 0;
      try{
	prevPosYFF = *(firefighter[i].getPosition().end()-2);
	prevPosXFF = *(firefighter[i].getPosition().end()-3);
	prevPosXD = *(dog[i].getPosition().end()-3);
	prevPosYD = *(dog[i].getPosition().end()-2);
	do{
	  dogSpeed = normal_integer(gen2, DOG_SPEED, STD_DEVIATION);
	}while(dogSpeed > DOG_SPEED + STD_DEVIATION);
      }
      catch(std::exception& e){
	std::cout << "ERROR: accesing previous positions" << std::endl;
      }

    /*calculate distance from node to obstacle and set status to avoid in case it lies within line of sight and directly ahead*/
      if(obstaclesNumber != 0/* && dog[i].state == SINE && firefighter[i].state == LINEAR*/){
	for(int j = 0; j < obstaclesNumber ; j++){
	  distanceToPoint = obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
	  /*distance within line of sight and ahead of node*/
	  if(distanceToPoint.distance < LOS  && distanceToPoint.distance > 0 && distanceToPoint.r > 0 && distanceToPoint.r < 1){ 
	    if(verbose)
	      std::cout << "[I](@ "<<time << "s)  Node "<< i << " that is in ("<< (int)prevPosXFF << "," << (int)prevPosYFF
			<< ") will collide with obstacle " << j << " in " << (int)distanceToPoint.distance << "m."; 
	    closestVertex = calcClosestVertex(firefighter[i], obstacle, obstaclesNumber, j);
 	    
	    /*if closest vertex is on the right of node, set the direction 
	     * to avoid it by 20 meters to the rigth of said vertex*/
	    if(closestVertex.x >= prevPosXFF){
	      firefighter[i].calcVersor( closestVertex.x+20, closestVertex.y ); //FIXME: Hardcoded
	      dog[i].calcVersor( closestVertex.x+20, closestVertex.y ); 
	    }
	    /*if closest vertex is on the left of node, set the direction 
	     * to avoid it by 20 meters to the left of said vertex*/  
	    else{
	      firefighter[i].calcVersor( closestVertex.x-20, closestVertex.y );
	      dog[i].calcVersor( closestVertex.x-20, closestVertex.y );
	    }
	    firefighter[i].state = AVOIDING_OBSTACLE;
	    dog[i].state = AVOIDING_OBSTACLE;
	    
	    /*save current position so that later the node can return to its original position*/
	    *(firefighter[i].previousPosition.begin()) = prevPosXFF;
	    *(firefighter[i].previousPosition.begin()+1) = prevPosYFF;
	    *(dog[i].previousPosition.begin()) = prevPosXD;
	    *(dog[i].previousPosition.begin()+1) = prevPosYD;
	  }
	}
      }
      /*update position of firefighters*/
      ffSpeed = normal_integer(gen2, SPEED_NODES, STD_DEVIATION/10);
      /*issue a warning if the speed of the firefigther is unrealistic*/
      if(ffSpeed > 1.5*1.2){ //20% over average walking speed
	std::cout << "[W] Firefighter " << i << " is moving at " << ffSpeed << 
	"m/s.\nThis is higher than the average of 1.4m/s" << std::endl;
      }
      /*flags to indicate whether or not a node can return
      * to its original position when avoiding an obstacle*/
      bool canReturn = true;
      bool canReturnFF = true;
      
      switch(firefighter[i].state){
 	/*the firefighter advances in a straight line with a probability of PROB per mil of deviating 2.5 meters in either
 	direction along the x axis*/
	case LINEAR:
	  if(random_integer(gen, 1000) < PROB){
	    do{
		xDeviation = random_integer(gen, 5) - 2.5; //FIXME: hardcoded
	    }while((xDeviation + prevPosXFF) < 0 or (xDeviation + prevPosXFF) > FIELD_SIZE_X);
	  }
	  else{
	    xDeviation = 0;
	  }
	  firefighter[i].setPosition(time, prevPosXFF + xDeviation, prevPosYFF + timeStep*ffSpeed, ffSpeed);
	  break;
	  
	/*When the FF is avoiding an obstacle, it heads into the vertex previously calculated in the calcClosestVertex*/
	case AVOIDING_OBSTACLE:
 	  /*check if current position is close to goal position*/
	  if(std::sqrt(std::pow(*(firefighter[i].getNextPosition().begin()) - prevPosXFF,2) + 
	     std::pow(*(firefighter[i].getNextPosition().begin()+1) - prevPosYFF,2)) < timeStep * ffSpeed + STD_DEVIATION){
	    if(verbose)
	      std::cout << "[I]" <<time << ": FF " << i <<" reached goal position. Will try to return to previous position." << std::endl;
	    firefighter[i].state = LINEAR_AVOIDING_OBSTACLE;
	  }
	  firefighter[i].setPosition(time, 
				     prevPosXFF+ *(firefighter[i].getVersor().begin())*timeStep*ffSpeed,
				     prevPosYFF + *(firefighter[i].getVersor().begin()+1)*timeStep*ffSpeed, 
				     ffSpeed);
	  break;
	
	/*The node advances in a straight line. In each timestep it checks if there are no obstacles
	 *that could interfer the way back to the original position.*/
	case LINEAR_AVOIDING_OBSTACLE:
	  for(int j = 0; j < obstaclesNumber ; j++){
	    distanceToPoint = obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
	    /*goes right and there are no obstacles between the current position and the position the node must return to*/
	    if( ((prevPosXFF < *(firefighter[i].previousPosition.begin())) && obstacle[j].getVertices().x1 > prevPosXFF 
		&& obstacle[j].getVertices().x2 > prevPosXFF && obstacle[j].getVertices().y1 < prevPosYFF 
		&& obstacle[j].getVertices().y2 > prevPosYFF && obstacle[j].getVertices().x1 < *(firefighter[i].previousPosition.begin()) 
		&& obstacle[j].getVertices().x1 < *(firefighter[i].previousPosition.begin()) )
		|| std::abs(prevPosYFF - *(firefighter[i].getNextPosition().begin()+1)) < 6 ){
	      if(verbose)
		std::cout << "[I] (@" <<  time << ") obstacle  " << j << " is on the right of FF " << i << std::endl;
	      canReturnFF = false;
	    }
	    /*goes left and there are no obstacles between the current position and the position the node must return to*/
	    else if( ((prevPosXFF > *(firefighter[i].previousPosition.begin())) && obstacle[j].getVertices().x1 < prevPosXFF 
		    && obstacle[j].getVertices().x2 < prevPosXFF &&  obstacle[j].getVertices().y1 < prevPosYFF 
		    && obstacle[j].getVertices().y2 > prevPosYFF && obstacle[j].getVertices().x1 > *(firefighter[i].previousPosition.begin()) 
		    && obstacle[j].getVertices().x1 > *(firefighter[i].previousPosition.begin()) )
		    || std::abs(prevPosYFF - *(firefighter[i].getNextPosition().begin()+1) < 6)){
	      if(verbose)
		std::cout << "[I] (@" <<  time << ") obstacle  " << j << " is on the left of FF " << i << std::endl;
	      canReturnFF = false;
	    }
	  }
	  if(canReturnFF){
	    if(verbose)
	      std::cout << "[I] (@" << time << ") FF " << i << " can return through " << prevPosYFF << std::endl;
	    firefighter[i].calcVersor( *(firefighter[i].previousPosition.begin()) , prevPosYFF );
	    firefighter[i].state = RETURNING;
	  }
	  firefighter[i].setPosition(time, prevPosXFF,prevPosYFF + timeStep*ffSpeed, ffSpeed);
	  break;

	/*When the node has reached the goal position that allows it to avoid an obstacle, it 
	 *will advance in a straight line along the x axis to its original x position, mantaining its current 
	 *y position*/
	case RETURNING:
	   if(std::sqrt(std::pow(*(firefighter[i].getNextPosition().begin()) - prevPosXFF,2) + 
	      std::pow(*(firefighter[i].getNextPosition().begin()+1) - prevPosYFF,2)) < timeStep * ffSpeed + STD_DEVIATION){
	     if(verbose)
	      std::cout << "[I] (@" <<time << ") FF " << i <<" reached goal position. Will resume normal movement." << std::endl;
	     firefighter[i].state = LINEAR;
	    }
	  firefighter[i].setPosition(time,
				    prevPosXFF+ *(firefighter[i].getVersor().begin())*timeStep*ffSpeed,
				    prevPosYFF + *(firefighter[i].getVersor().begin()+1)*timeStep*ffSpeed,
				    ffSpeed);
	break;
      }

      switch(dog[i].state)
      {
	/*The dog moves following a sine wave*/
	case SINE:
	  if(random_integer(gen, 1000) < PROB){
	    dog[i].state = ASTRAY;
	    if(verbose){ 
	      std::cout << "[I] (@" << time << "s dog[" << i << "] has gone astray" << "\n"
			<< "current pos: ("<< prevPosXD <<"," << prevPosYD << ")" << std::endl;
	    }
	    /*calculate new random position, making sure it doesn't go out of bounds*/
	    do{
	      yDeviation = random_integer(gen, 200) - 100; //FIXME: are these limits ok?
	      xDeviation = random_integer(gen, 200) - 100;
	    }while((yDeviation + prevPosYD) < 0 or (xDeviation + prevPosXD) < 0 or
		  (xDeviation + prevPosXD) > FIELD_SIZE_X);
	    if((yDeviation + prevPosYD) > FIELD_SIZE_Y){ 
	      std::cout << "[W] Y limit exceeded @ " << time << "s by dog " << i << std::endl;
	    }
	    
	    /*calculate the versor which points in said direction*/
	    dog[i].calcVersor(xDeviation + prevPosXD, yDeviation + prevPosYD);
	    if(verbose){
	      std::cout << "next pos: ("<< xDeviation + prevPosXD  <<"," << yDeviation + prevPosYD << ")" << std::endl;
	      std::cout << "versor: ("<< *(dog[i].getVersor().begin()) <<"," 
			<< *(dog[i].getVersor().begin()+1)<< ")\n" << std::endl;
	    }
	  }
	  else{
	    dog[i].calcVersor(*(dog[i].getPosition().begin()+1)+DELTA_X*std::cos((prevPosYD + timeStep*SPEED_NODES)/8), //FIXME: hardcoded the eight affects the freq of the sine
			      prevPosYFF + timeStep*SPEED_NODES  + DELTA_Y); 
	  }
	  dog[i].setPosition(time, 
			    prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
			    prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
			    dogSpeed);
	break;
	  
	/*idem ff*/
	case AVOIDING_OBSTACLE:
	  /*check that current position is close to goal position*/
	  if(std::sqrt(std::pow(*(dog[i].getNextPosition().begin()) - prevPosXD,2) + 
	     std::pow(*(dog[i].getNextPosition().begin()+1) - prevPosYD,2)) < timeStep * ffSpeed + STD_DEVIATION){
	    std::cout << "[I]" <<time << ": Dog " << i <<" reached goal position. Will try to return to previous position." << std::endl;
	    dog[i].state = LINEAR_AVOIDING_OBSTACLE;
	  }
	  dog[i].setPosition(time, 
			     prevPosXD + *(dog[i].getVersor().begin())*timeStep*ffSpeed,
			     prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*ffSpeed, 
			     ffSpeed);

	  break;
	
	/*ïdem ff*/
	case LINEAR_AVOIDING_OBSTACLE:
	  
	  for(int j = 0; j < obstaclesNumber ; j++){
	    distanceToPoint = obstacle[j].getDistanceToPoint(prevPosXD, prevPosYD);
	    /*goes right and there are no obstacles between the current position and the position the node must return to*/
	    if( ((prevPosXD < *(dog[i].previousPosition.begin())) && obstacle[j].getVertices().x1 > prevPosXD 
		&& obstacle[j].getVertices().x2 > prevPosXD && obstacle[j].getVertices().y1 < prevPosYD 
		&& obstacle[j].getVertices().y2 > prevPosYD && obstacle[j].getVertices().x1 < *(dog[i].previousPosition.begin()) 
		&& obstacle[j].getVertices().x1 < *(dog[i].previousPosition.begin()) )
	        || std::abs(prevPosYD - *(dog[i].getNextPosition().begin()+1)) < 6 ){
	      canReturn = false;
	    }
	    /*goes left and there are no obstacles between the current position and the position the node must return to*/
	    else if( ((prevPosXD > *(dog[i].previousPosition.begin())) && obstacle[j].getVertices().x1 < prevPosXD
		    && obstacle[j].getVertices().x2 < prevPosXD &&  obstacle[j].getVertices().y1 < prevPosYD 
		    && obstacle[j].getVertices().y2 > prevPosYD && obstacle[j].getVertices().x1 > *(dog[i].previousPosition.begin()) 
		    && obstacle[j].getVertices().x1 > *(dog[i].previousPosition.begin()) )
		    || std::abs(prevPosYD - *(dog[i].getNextPosition().begin()+1) < 6)){
	      canReturn = false;
	    }
	  }
	  if(canReturn){
	    if(verbose)
	      std::cout << "[I] (@" << time << ") dog " << i << " can return through " << prevPosYFF << std::endl;
	    dog[i].calcVersor( *(dog[i].previousPosition.begin()) , prevPosYD );
	    dog[i].state = RETURNING;
	  }
	  dog[i].setPosition(time, prevPosXD, prevPosYD + timeStep*ffSpeed, ffSpeed);
	break;
	  
	case RETURNING:
	  if(std::sqrt(std::pow(*(dog[i].getNextPosition().begin()) - prevPosXD,2) + 
	     std::pow(*(dog[i].getNextPosition().begin()+1) - prevPosYD,2)) < timeStep * ffSpeed + STD_DEVIATION){
	    if(verbose)
	      std::cout << "[I] (@" <<time << ") Dog " << i <<" reached goal position. Will resume normal movement." << std::endl;
	    dog[i].state = SINE;
	  }

	  dog[i].setPosition(time, 
			    prevPosXD+ *(dog[i].getVersor().begin())*timeStep*ffSpeed,
			    prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*ffSpeed, 
			    ffSpeed);
	break;
	
	/*dog has gone astray, running to a random position*/
	case ASTRAY:
	  if(std::sqrt(std::pow(*(dog[i].getNextPosition().begin()) - prevPosXD,2) + 
		std::pow(*(dog[i].getNextPosition().begin()+1) - prevPosYD,2)) < timeStep * DOG_SPEED + STD_DEVIATION){
	    if(verbose)
	      std::cout << "[I] (@" << time << ") dog[" << i <<"] reached goal destination. Will return to firefighter." << std::endl;
	    dog[i].state = RETURNING_TO_FF;
	  }
	  dog[i].setPosition(time, 
			     prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
			     prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
			     dogSpeed);
	break;
	
	/*After leaving the ASTRAY state, the dog will take the shortest path to return to the firefighter */
	case RETURNING_TO_FF:
	  dog[i].calcVersor(prevPosXFF, prevPosYFF);
	  dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
			     prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
			     dogSpeed);

	  if(std::sqrt(std::pow(prevPosXD - prevPosXFF,2) + 
		       std::pow(prevPosYD - prevPosYFF,2)) < 5){
	  if(verbose)
	    std::cout  << "[I] (@" << time << ") dog[" << i <<"] has returned to firefighter." << std::endl;
	  dog[i].state = SINE;
	  }
	  
	break;
	
      }
      
    }
  }
  
  //writing output to a file
  nsFile.open("ns_" + FILE_NAME + std::to_string(k), std::ofstream::out | std::ofstream::trunc); //to feed into ns-3
  resultsFile.open(FILE_NAME + std::to_string(k), std::ofstream::out | std::ofstream::trunc); //for debugging purposes
    for(int j = 0; j < NUM_NODES; j++){ //circle through nodes
      for(int i = 0; i < firefighter[j].getPosition().size(); i++){ //circle through node's vectorPosition content
	resultsFile << firefighter[j].getPosition().at(i) << " ";
      }
      resultsFile << std::endl;
      for(int i = 0; i < dog[j].getPosition().size(); i++){ //circle through node's vectorPosition content
	resultsFile << dog[j].getPosition().at(i) << " ";
      }
      resultsFile << std::endl;
    }

    for(int j = 0; j < NUM_NODES*2; j++){ //circle through nodes, all of them
      for(int i = 0; i < dog[0].getPosition().size(); i+=4){ //circle through node's vectorPosition content
	if(j < NUM_NODES){ //dogs
	  if(i == 0){ //write initial position
	    nsFile << "$node_(" << j << ") set X_ " << dog[j].getPosition().at(i+1) << std::endl;
	    nsFile << "$node_(" << j << ") set Y_ " << dog[j].getPosition().at(i+2) << std::endl;
	  }
	  else{
	    nsFile << "$ns_ at " << dog[j].getPosition().at(i) << " \"$node_(" << j << ") setdest " <<
	      dog[j].getPosition().at(i+1) << " " << dog[j].getPosition().at(i+2) << " " << dog[j].getPosition().at(i+3) << "\"" <<std::endl;
	  }
	}
	else{ //firefighters
	  if(i == 0){ //write initial position
	    nsFile << "$node_(" << j << ") set X_ " << firefighter[j-NUM_NODES].getPosition().at(i+1) << std::endl;
	    nsFile << "$node_(" << j << ") set Y_ " << firefighter[j-NUM_NODES].getPosition().at(i+2) << std::endl;
	  }
	  else{
	    nsFile << "$ns_ at " << firefighter[j-NUM_NODES].getPosition().at(i) << " \"$node_(" << j << ") setdest " <<
	      firefighter[j-NUM_NODES].getPosition().at(i+1) << " " << firefighter[j-NUM_NODES].getPosition().at(i+2) << " " << 
	      firefighter[j-NUM_NODES].getPosition().at(i+3) << "\"" <<std::endl;
	  }
	}
      }
    }
	
    resultsFile.close();
    nsFile.close();
    
    return 0;
}
