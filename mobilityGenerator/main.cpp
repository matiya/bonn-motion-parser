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


int random_integer(std::mt19937& generator, int n){
    /*returns a random integer between 0 and n*/
    std::uniform_int_distribution<int> dist_n(0, n);
    return dist_n(generator);
}

double normal_integer(std::default_random_engine& generator, double mean, double stdDeviation){
  /*returns a random double drawn from a normal distribution*/
  std::normal_distribution<double> distribution(mean, stdDeviation);
  return std::abs(distribution(generator));
}

Obstacle::vertex calcClosestVertex(Node firefighter, Obstacle* obstacle, unsigned int q, unsigned int i){
  //TODO: test this better
  //TODO: change obstacles to polygons 
  Obstacle::linePointParameters distanceStruct;
  Obstacle::vertex vertex, tempVertex;
  vertex = (obstacle + i)->getClosestVertex(*(firefighter.getPosition().end()-3),*(firefighter.getPosition().end()-2));
  do{
  for(int m = 0; m < q ; m++){
    //check if there are obstacles near the calc'd vertex
    if(m!=i){//don't check against itself
//       std::cout << m << std::endl;
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
// 	    std::cout << "new vertex: " << vertex.x << "\t " << vertex.y << std::endl;
	  }
	  else{
	    vertex =  (obstacle + m)->getClosestVertex(*(firefighter.getPosition().end()-3)-FIELD_SIZE_X,*(firefighter.getPosition().end()-2));
	  }
// 	  std::cout << "obstacle: " << m << " \tnvertex: " << vertex.x<<","<< vertex.y<< " \ttempVertex: "<< tempVertex.x << "," <<tempVertex.y<< std::endl;
	}
	else{
	  vertex.x = tempVertex.x;
	  vertex.y = tempVertex.y;
	}
	//if yes, check if the lies ahead of the diversion path

      }
    }
    
    //if yes, calc the vertex of that obstacle and the other vertex if oldVertex == newVertex, that means old vertex is outside field, therefore
//       new vertex should be vertex of obstacle
    //rinse and repaea

// 	    std::cout << i << "new vertex: " << vertex.x << "\t " << vertex.y << std::endl;

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
  
  //check if file exists already
  while (f.good()) {
    k++;
    f.close();
    f.open(FILE_NAME + std::to_string(k));
  }

  std::random_device rd;
  
  int seedForGen;
  SEED == 0 ? seedForGen = rd() : seedForGen = SEED;
  std::mt19937 gen(seedForGen);
  std::default_random_engine gen2(seedForGen);
  
  Node dog[NUM_NODES]; 
  Node firefighter[NUM_NODES];
  
  Obstacle obstacle[obstaclesNumber];

//   initialize obstacles
  if(segment.size() != 0)
  {
    int i = 0;
    for ( int j = 0; j < obstaclesNumber ; j++){
      obstacle[j].setVertices(segment.at(i),segment.at(i+1),segment.at(i+2),segment.at(i+3)) ;
      i+=4; //FIXME: put in for prototype
    }
  }
  //set the intial positions and speed
  for(int i = 0; i < NUM_NODES ; i++){
    dog[i].setPosition(0, DELTA_X + i*rfDist, 0, SPEED_NODES);
    firefighter[i].setPosition(0, DELTA_X + i*rfDist, 0, SPEED_NODES);
    
  }
  std::cout << "[I] Timestep: " << timeStep << std::endl;
  
  //update nodes positions
  while(time < DURATION){
    //TODO: replace isAstray, isReturning, isAvoidingObstacle by a switch(STATUS)
    time += timeStep;
    
    //load position at t-1 from the corresponding node's vectorPosition
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

      
      /*
    std::cout <<"dog: " << prevPosXD << ";" << prevPosYD << std::endl;
    std::cout << "ff: " << prevPosXFF << ";" << prevPosYFF << std::endl;
    */
//    calculate distance from node to obstacle and set flag to avoid in case it lies within line of sight and directly ahead
      if(obstaclesNumber != 0 && !firefighter[i].isAvoidingObstacle && !firefighter[i].isReturning){
	for(int j = 0; j < obstaclesNumber ; j++){
	  distanceToPoint = obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
	  if(distanceToPoint.distance < LOS  && distanceToPoint.distance > 0 && distanceToPoint.r > 0 && distanceToPoint.r < 1){ //distance within line of sight and ahead of node
	    std::cout << "[I](@ "<<time << "s)  Node "<< i << " which is in "<< prevPosXFF << "," << prevPosYFF << " will collide with obstacle " 
		      << j << " in " << (int)distanceToPoint.distance << "m." << " r: " << distanceToPoint.r << std::endl;
	    closestVertex = calcClosestVertex(firefighter[i], obstacle, obstaclesNumber, j);
	    std::cout << "closest vertex at: "<< closestVertex.x << "," << closestVertex.y << std::endl;
	    if(closestVertex.x >= prevPosXFF){
	      firefighter[i].calcVersor( closestVertex.x+20, closestVertex.y ); //FIXME: Ask about this
	    }
	      
	    else{
// 	      if(distanceToPoint.px < prevPosXD)
// 	      std::cout << i << "prev: " <<prevPosXFF << " closest: " << closestVertex.x << ", " << closestVertex.y << std::endl;
	      firefighter[i].calcVersor( closestVertex.x-20, closestVertex.y ); //FIXME: Ask about this
	    }
	    firefighter[i].isAvoidingObstacle = true;
	    dog[i].isAvoidingObstacle = true;
	    //save current position
	    *(firefighter[i].previousPosition.begin()) = prevPosXFF;
	    *(firefighter[i].previousPosition.begin()+1) = prevPosYFF;
	  }
	}
      }
      
      //update position of firefighters
      ffSpeed = normal_integer(gen2, SPEED_NODES, STD_DEVIATION/10);
      if(ffSpeed > 1.5*1.2){ //20% over average walking speed
	std::cout << "[W] Firefighter " << i << " is moving at " << ffSpeed << "m/s.\nThis is higher than the average of 1.4m/s" << std::endl;
      }
      if(!firefighter[i].isAvoidingObstacle && !firefighter[i].isReturning){
      if(random_integer(gen, 1000) < PROB){
	do{ //calculate new random position, making sure it doesn't go out of bounds
	    xDeviation = random_integer(gen, 5) - 2.5;
	  }while((xDeviation + prevPosXFF) < 0 or (xDeviation + prevPosXFF) > FIELD_SIZE_X);
	firefighter[i].setPosition(time, prevPosXFF + xDeviation, prevPosYFF + timeStep*ffSpeed, ffSpeed);
      }
      else{
	firefighter[i].setPosition(time, prevPosXFF, prevPosYFF + timeStep*ffSpeed, ffSpeed);
      }
      }
      else if(firefighter[i].isAvoidingObstacle){
// 	std::cout << dog[i].getNextPosition().at(0) << "," << firefighter[i].getNextPosition().at(0) << std::endl;
	if(std::sqrt(std::pow(*(firefighter[i].getNextPosition().begin()) - prevPosXFF,2) + 
		std::pow(*(firefighter[i].getNextPosition().begin()+1) - prevPosYFF,2)) < timeStep * ffSpeed + STD_DEVIATION){
// 	  firefighter[i].setPosition(time, 
// 				   prevPosXFF+ *(firefighter[i].getVersor().begin())*timeStep*ffSpeed,
// 				   prevPosYFF + *(firefighter[i].getVersor().begin()+1)*timeStep*ffSpeed, 
// 				   ffSpeed);
	  std::cout << i <<" got to position at "<< time << std::endl;
	if(firefighter[i].isReturning){
	  firefighter[i].isReturning = false;
	  firefighter[i].isAvoidingObstacle = false;
	  dog[i].isAvoidingObstacle = false;
	}
	else{
	  firefighter[i].isReturning = true;
	  firefighter[i].isAvoidingObstacle = false;
	}
	
	}
	firefighter[i].setPosition(time, 
				   prevPosXFF+ *(firefighter[i].getVersor().begin())*timeStep*ffSpeed,
				   prevPosYFF + *(firefighter[i].getVersor().begin()+1)*timeStep*ffSpeed, 
				   ffSpeed);
	dog[i].setPosition(time, 
			   prevPosXFF+ *(firefighter[i].getVersor().begin())*timeStep*ffSpeed,
			   prevPosYFF + *(firefighter[i].getVersor().begin()+1)*timeStep*ffSpeed, 
			   ffSpeed);
      }
      if(firefighter[i].isReturning && !firefighter[i].isAvoidingObstacle){
	firefighter[i].setPosition(time, 
				   prevPosXFF,
				   prevPosYFF + timeStep*ffSpeed, 
				   ffSpeed);
	dog[i].setPosition(time, 
				   prevPosXFF,
				   prevPosYFF + timeStep*ffSpeed, 
				   ffSpeed);
// 	std::cout << "snode " << i << " pos: " << prevPosXFF << ", " <<  prevPosYFF << std::endl;
	bool canReturn = true;
	for(int j = 0; j < obstaclesNumber ; j++){
	  distanceToPoint = obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
// 	  if(time > 00 && time < 1000/* && (int)distanceToPoint.py == (int)prevPosYFF*/){
// 	    std::cout << "obs: " << j << "\t node: " << i <<  "\t py : " << (int)distanceToPoint.py << "\t y: " << (int)prevPosYFF << 
// 	    "\nr: " << distanceToPoint.r << "\rtime: " << time << "\t x: " << obstacle[j].getVertices().x1 <<"," << obstacle[j].getVertices().x2 << 
// 	    " px: "  << prevPosXFF << "\n sp: " << *(firefighter[i].previousPosition.begin()) << std::endl;
// 
// 	  }
	  //goes right and there are no obstacles between the current position and the position the node must return to                                 
	  if( ((prevPosXFF < *(firefighter[i].previousPosition.begin())) && obstacle[j].getVertices().x1 > prevPosXFF 
	      && obstacle[j].getVertices().x2 > prevPosXFF && obstacle[j].getVertices().y1 < prevPosYFF && obstacle[j].getVertices().y2 > prevPosYFF 
	      && obstacle[j].getVertices().x1 < *(firefighter[i].previousPosition.begin()) && obstacle[j].getVertices().x1 < *(firefighter[i].previousPosition.begin()) )
	    || std::abs(prevPosYFF - *(firefighter[i].getNextPosition().begin()+1)) < 6 ){
	    std::cout << "[I] " <<  time << " obstacle  " << j << " is on the right of node " << i << std::endl;
	    std::cout << "obstacle: " << j << "\t py : " << *(firefighter[i].previousPosition.begin()+1) << "\t y: " << (int)prevPosYFF << std::endl;
	    canReturn = false;
	  }
	  //goes left and there are no obstacles between the current position and the position the node must return to                                 
	  else if( ((prevPosXFF > *(firefighter[i].previousPosition.begin())) && obstacle[j].getVertices().x1 < prevPosXFF 
		  && obstacle[j].getVertices().x2 < prevPosXFF &&  obstacle[j].getVertices().y1 < prevPosYFF && obstacle[j].getVertices().y2 > prevPosYFF
		  && obstacle[j].getVertices().x1 > *(firefighter[i].previousPosition.begin()) && obstacle[j].getVertices().x1 > *(firefighter[i].previousPosition.begin()) )
		 || std::abs(prevPosYFF - *(firefighter[i].getNextPosition().begin()+1) < 6))
	  {
	    std::cout << time << " obstacle  " << j << " is on the left of node " << i << std::endl;
	    canReturn = false;
	  }
	}
	if(canReturn){
	  std::cout << "node " << i << " can return through " << prevPosYFF << std::endl;
	  std::cout <<"py : " << *(firefighter[i].getNextPosition().begin()+1) << "\t y: " << (int)prevPosYFF << 
	  "\tdif: "<<  std::abs(prevPosYFF - *(firefighter[i].getNextPosition().begin()+1)) << std::endl;
	  firefighter[i].calcVersor( *(firefighter[i].previousPosition.begin()) , prevPosYFF );
	  firefighter[i].isAvoidingObstacle = true;
	}
      }
      //update position of dogs
      if(!dog[i].isAstray && !dog[i].isAvoidingObstacle){
	if(random_integer(gen, 1000) < PROB){
	  dog[i].isAstray = true;
	  if(verbose){ 
	    std::cout << "@" << time << "s dog[" << i << "] has gone astray" << std::endl;
	    std::cout << "current pos: ("<< prevPosXD <<"," << prevPosYD << ")" << std::endl;
	  }
	  do{ //calculate new random position, making sure it doesn't go out of bounds
	    yDeviation = random_integer(gen, 200) - 100; //FIXME: are these limits ok?
	    xDeviation = random_integer(gen, 200) - 100;
	  }while((yDeviation + prevPosYD) < 0 or (xDeviation + prevPosXD) < 0 or
	    	 (xDeviation + prevPosXD) > FIELD_SIZE_X);
	  if((yDeviation + prevPosYD) > FIELD_SIZE_Y){ 
	    std::cout << "[W] Y limit exceeded @ " << time << "s by node " << i << std::endl;
	  }
	  
	  //calculate the versor which points in said direction
	  dog[i].calcVersor(xDeviation + prevPosXD, yDeviation + prevPosYD);
	  if(verbose){
	    std::cout << "next pos: ("<< xDeviation + prevPosXD  <<"," << yDeviation + prevPosYD << ")" << std::endl;
	    std::cout << "versor: ("<< *(dog[i].getVersor().begin()) <<"," 
		      << *(dog[i].getVersor().begin()+1)<< ")\n" << std::endl;
	  }
	}
	else{
	  dog[i].calcVersor(*(dog[i].getPosition().begin()+1)+DELTA_X*std::cos((prevPosYD + timeStep*SPEED_NODES)/8), //the eight affects the freq of the sine
			    prevPosYFF + timeStep*SPEED_NODES  + DELTA_Y); //tiempo
	}
	  
	  dog[i].setPosition(time, 
			     prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
			     prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
			     dogSpeed);

      }
    
      else if(dog[i].isAstray && !dog[i].isAvoidingObstacle){ //isAstray == true
	
	if(!dog[i].isReturning){
// 	  if(i==3){std::cout << "\tdebug: " << std::sqrt(std::pow(*(dog[i].getNextPosition().begin()) - prevPosXD,2) + 
// 		std::pow(*(dog[i].getNextPosition().begin()+1) - prevPosYD,2)) << std::endl;
// 		std::cout << "speed " << dogSpeed << std::endl;
// 		std::cout << "speed2 " << DOG_SPEED + STD_DEVIATION << "\n" << std::endl;
// 	  }

	  if(std::sqrt(std::pow(*(dog[i].getNextPosition().begin()) - prevPosXD,2) + 
		std::pow(*(dog[i].getNextPosition().begin()+1) - prevPosYD,2)) > timeStep * DOG_SPEED + STD_DEVIATION){
	    dog[i].setPosition(time, 
			       prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
			       prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
			       dogSpeed);
	  }
	  else{
	    //TODO: Add pause();	  
	    if(verbose)
	      std::cout << "@" << time << "s dog[" << i <<"] reached goal destination " << std::endl;
	    dog[i].isReturning = true;
	    dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
				  prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
				  dogSpeed);
	  }
	  
	}

	else { //isReturning == true
	  dog[i].calcVersor(prevPosXFF, prevPosYFF);
	  dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*dogSpeed,
			     prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*dogSpeed,
			     dogSpeed);
// 	  if(i==3){std::cout << "\tdebug: " << std::sqrt(std::pow(*(dog[i].getNextPosition().begin()) - prevPosXD,2) + 
// 		   std::pow(*(dog[i].getNextPosition().begin()+1) - prevPosYD,2)) << std::endl;
// 	  }

	  if(std::sqrt(std::pow(prevPosXD - prevPosXFF,2) + 
		       std::pow(prevPosYD - prevPosYFF,2)) < 5){
	    if(verbose)
	      std::cout  << "@" << time << "s dog[" << i <<"] has returned to firefighter" << std::endl;
	    dog[i].isAstray = false;
	    dog[i].isReturning = false;
	  }
	}
	
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
