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


int main(int argc, char **argv) {
  
  
  int k =0 ;
  std::vector<float> segment = askForParameters(argc, argv); 
  int obstaclesNumber = segment.size()/4;
  double time = 0;
  double prevPosYFF = 0, prevPosXFF = 0, prevPosXD = 0, prevPosYD = 0;
  double yDeviation = 0, xDeviation = 0, xDir = 0, yDir = 0;
  double dogSpeed = 0, ffSpeed = 0;
  Obstacle::linePointParameters distanceToPoint;
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
      i+=4;
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
    time += timeStep;
    
    //load position at t-1 from the corresponding node's vectorPosition
    for(int i = 0; i<NUM_NODES; i++){      
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
      if(obstaclesNumber != 0)
      {
	for(int j = 0; j < obstaclesNumber ; j++){
	  distanceToPoint = obstacle[j].getDistanceToPoint(prevPosXFF, prevPosYFF);
	  if(distanceToPoint.distance < LOS  && distanceToPoint.distance > 0 && distanceToPoint.r > 0 && distanceToPoint.r < 1){ //distance within line of sight and ahead of node
	    std::cout << "[W](@ "<<time << "s)  Node "<< i << " wich is in "<< (int)prevPosXFF << "," << (int)prevPosYFF << " will collide with obstacle " 
		      << j << " in " << (int)distanceToPoint.px << "," 
		      << (int)distanceToPoint.py << " given distance: " << distanceToPoint.distance <<  std::endl;
	  }
	}
      }
      
      //update position of firefighters
      ffSpeed = normal_integer(gen2, SPEED_NODES, STD_DEVIATION/10);
      if(ffSpeed > 1.5*1.2){ //20% over average walking speed
	std::cout << "[W] Firefighter " << i << " is moving at " << ffSpeed << "m/s.\nThis is higher than the average of 1.4m/s" << std::endl;
      }
      if(random_integer(gen, 1000) < PROB){
	do{ //calculate new random position, making sure it doesn't go out of bounds
	    xDeviation = random_integer(gen, 5) - 2.5;
	  }while((xDeviation + prevPosXFF) < 0 or (xDeviation + prevPosXFF) > FIELD_SIZE_X);
	firefighter[i].setPosition(time, prevPosXFF + xDeviation, prevPosYFF + timeStep*ffSpeed*0.8, ffSpeed);
      }
      else{
	firefighter[i].setPosition(time, prevPosXFF, prevPosYFF + timeStep*ffSpeed, ffSpeed);
      }
      
      //update position of dogs
      if(!dog[i].isAstray){
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
    
      else{ //isAstray == true
	
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
