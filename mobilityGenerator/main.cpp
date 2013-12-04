#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include "node.h" //here's the definition of a node
#include "parameters.h"
//FIXME: make sure that the sine wave doesn't start in 0 so that there are no neg values in x axis

int random_integer(std::mt19937& generator, int n){
    /*returns a random integer between 0 and n*/
    std::uniform_int_distribution<int> dist_n(0, n);
    return dist_n(generator);
}

int main(int argc, char **argv) {
    
  double time;
  double prevPosYFF, prevPosXFF, prevPosXD, prevPosYD, yPos;
  double yDeviation, xDeviation, xDir, yDir;
  const double rfDist = (double) FIELD_SIZE_X / NUM_NODES;
  const double timeStep = (double) DURATION/SAMPLES_NUMBER;  std::cout << "timestep: " << timeStep << std::endl;
  std::ofstream resultsFile, nsFile;
  nsFile.open(FILEPATH, std::ofstream::out | std::ofstream::trunc); //to feed into ns-3
  resultsFile.open(FILEPATH1, std::ofstream::out | std::ofstream::trunc); //for debugging purposes

  std::random_device rd;
  int seedForGen;
  SEED == 0 ? seedForGen = rd() : seedForGen = SEED;
  std::mt19937 gen(seedForGen);
  
  Node firefighter[NUM_NODES], dog[NUM_NODES]; 
  
  //set the intial positions and speed
  //TODO: allow the user to enter positions through a txt file
  for(int i = 0; i < NUM_NODES ; i++){
    firefighter[i].setPosition(0, 0 + i*rfDist, 0);
    firefighter[i].setSpeed(SPEED_NODES);
    dog[i].setPosition(0, 0 + i*rfDist, 0);
    dog[i].setSpeed(DOG_SPEED);
  }
  
  //update nodes positions
  while(time < DURATION){
    
    //TODO: throw exceptions when surpassing field limits
    //TODO: randomize speed and x movement
    time += timeStep;
    for(int i = 0; i<NUM_NODES; i++){
      
      //load position at t-1 from the corresponding node's vectorPosition
      prevPosYFF = *(firefighter[i].getPosition().end()-1);
      prevPosXFF = *(firefighter[i].getPosition().end()-2);
      prevPosXD = *(dog[i].getPosition().end()-2);
      prevPosYD = *(dog[i].getPosition().end()-1);
      
      //update position of firefighters
      firefighter[i].setPosition(time, prevPosXFF, prevPosYFF + timeStep*SPEED_NODES);
      
      //update position of dogs
      if(!dog[i].isAstray){
	if(random_integer(gen, 1000) < PROB){
	  dog[i].isAstray = true;
	  std::cout << "dog[" << i << "] has gone astray" << std::endl;
	  std::cout << "current pos: ("<< prevPosXD <<"," << prevPosYD << ")" << std::endl;
	  
	  do{ //calculate new random position, making sure it doesn't go out of bounds
	    yDeviation = random_integer(gen, 100) - 50;
	    xDeviation = random_integer(gen, 100) - 50;
	    std::cout << i << " : dev : " << yDeviation + prevPosYD << " , " << xDeviation + prevPosXD << std::endl;
	  }while((yDeviation + prevPosYD) < 0 and (xDeviation + prevPosXD) < 0);
	  //calculate the versor which points in said direction
	  dog[i].calcVersor(xDeviation + prevPosXD, yDeviation + prevPosYD);	  
	  std::cout << "next pos: ("<< xDeviation + prevPosXD  <<"," << yDeviation + prevPosYD << ")" << std::endl;
	  std::cout << "versor: ("<< *(dog[i].getVersor().begin()) <<"," 
		    << *(dog[i].getVersor().begin()+1)<< ")" << std::endl;
	}
	
	dog[i].setPosition(time, *(dog[i].getPosition().begin()+1)+DELTA_X*cos(prevPosYD/8), prevPosYD + 
	  timeStep*SPEED_NODES);
      }
      else{ //isAstray == true
	
	if(!dog[i].isReturning){
	  
	  std::cout << i << " : " << *(dog[i].getNextPosition().begin()) - prevPosXD << std::endl;
	  
	  if( std::abs(*(dog[i].getNextPosition().begin()) - prevPosXD) > 2){
	    dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*DOG_SPEED,
				prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*DOG_SPEED);
	  }
	  else{
	  //TODO: Add pause();	  
	  std::cout << i <<" dog reached destination " << std::endl;
	  dog[i].isReturning = true;
	  dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*DOG_SPEED,
				prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*DOG_SPEED);
	  }
	  
	}
	
	else { //isReturning == true
	  //locate corresponding firefighter (has the same i?)
	  //calculate versor
	  //speed up 
	  dog[i].calcVersor(prevPosXFF, prevPosYFF);
	  dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*DOG_SPEED,
				  prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*DOG_SPEED);
	  
	  if( std::abs(*(dog[i].getNextPosition().begin()) - prevPosXFF) > 1 ){
	    std::cout << i << " returning : " << *(dog[i].getNextPosition().begin()) - prevPosXD << std::endl;
	    
	  }
	  else{
	    std::cout << "dog has returned to firefighter" << std::endl;
	    //dog[i].isAstray = false;
// 	    dog[i].setPosition(time, prevPosXD + *(dog[i].getVersor().begin())*timeStep*DOG_SPEED,
// 				  prevPosYD + *(dog[i].getVersor().begin()+1)*timeStep*DOG_SPEED);
	  }
	}
	
      }
    }
  }
  
  //writing output to a file
    for(int j = 0; j < NUM_NODES; j++){ //circle through nodes
      for(int i = 0; i < firefighter[j].getPosition().size(); i++){ //circle through node's vectorPosition content
	nsFile << firefighter[j].getPosition().at(i) << " ";
      }
      nsFile << std::endl;
      for(int i = 0; i < dog[j].getPosition().size(); i++){ //circle through node's vectorPosition content
	nsFile << dog[j].getPosition().at(i) << " ";
      }
      nsFile << std::endl;
    }


    for(int j = 0; j < NUM_NODES*2; j++){ //circle through nodes, all of them
      for(int i = 0; i < firefighter[j].getPosition().size(); i+=3){ //circle through node's vectorPosition content
	if(j<NUM_NODES){ //dogs
	  if(i == 0){ //write initial position
	    resultsFile << "$node_(" << j << ") set X_ " << dog[j].getPosition().at(i+1) << std::endl;
	    resultsFile << "$node_(" << j << ") set Y_ " << dog[j].getPosition().at(i+2) << std::endl;
	  }
	  else{
	    resultsFile << "$ns_ at " << dog[j].getPosition().at(i) << " \"$node_(" << j << ") setdest " <<
	      dog[j].getPosition().at(i+1) << " " << dog[j].getPosition().at(i+2) << " " << dog[j].getSpeed() << "\"" <<std::endl;
	  }
	}
	else{ //firefighters
	  if(i == 0){ //write initial position
	    resultsFile << "$node_(" << j << ") set X_ " << firefighter[j-NUM_NODES].getPosition().at(i+1) << std::endl;
	    resultsFile << "$node_(" << j << ") set Y_ " << firefighter[j-NUM_NODES].getPosition().at(i+2) << std::endl;
	  }
	  else{
	    resultsFile << "$ns_ at " << firefighter[j-NUM_NODES].getPosition().at(i) << " \"$node_(" << j << ") setdest " <<
	      firefighter[j-NUM_NODES].getPosition().at(i+1) << " " << firefighter[j-NUM_NODES].getPosition().at(i+2) << " " << 
	      firefighter[j-NUM_NODES].getSpeed() << "\"" <<std::endl;
	  }
	}
      }
    }
	
    resultsFile.close();
    nsFile.close();
    
    return 0;
}
