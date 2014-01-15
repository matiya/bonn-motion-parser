#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include "node.h" 
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
  
  askForParameters(argc, argv); 
  
  int k=0;
  double time;
  double prevPosYFF, prevPosXFF, prevPosXD, prevPosYD, yPos;
  double yDeviation, xDeviation, xDir, yDir;
  double dogSpeed, ffSpeed;
  const double rfDist = (double) FIELD_SIZE_X / NUM_NODES;
  const double timeStep = (double) DURATION/SAMPLES_NUMBER;  
  std::ofstream resultsFile, nsFile;
  std::ifstream f(FILE_NAME);
  
  //check if file exists already
  while (f.good()) {
    k++;
    f.close();
    f.open(FILE_NAME + std::to_string(k));
  }
  
  nsFile.open("ns_" + FILE_NAME + std::to_string(k), std::ofstream::out | std::ofstream::trunc); //to feed into ns-3
  resultsFile.open(FILE_NAME + std::to_string(k), std::ofstream::out | std::ofstream::trunc); //for debugging purposes

  std::random_device rd;
  
  int seedForGen;
  SEED == 0 ? seedForGen = rd() : seedForGen = SEED;
  std::mt19937 gen(seedForGen);
  std::default_random_engine gen2(seedForGen);
  
  Node firefighter[NUM_NODES], dog[NUM_NODES]; 
  
  //set the intial positions and speed
  for(int i = 0; i < NUM_NODES ; i++){
    firefighter[i].setPosition(0, DELTA_X + i*rfDist, 0, SPEED_NODES);
    firefighter[i].setSpeed(SPEED_NODES);
    dog[i].setPosition(0, DELTA_X + i*rfDist, 0, SPEED_NODES);
    dog[i].setSpeed(DOG_SPEED);
  }
  std::cout << "timestep: " << timeStep << std::endl;
  //update nodes positions
  while(time < DURATION){
    
    time += timeStep;/*
    std::cout << std::endl;
    std::cout << time << std::endl;*/
    for(int i = 0; i<NUM_NODES; i++){      
      //load position at t-1 from the corresponding node's vectorPosition
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
      //update position of firefighters
      ffSpeed = normal_integer(gen2, SPEED_NODES, STD_DEVIATION/10);
      if(ffSpeed > 1.5*1.2){ 
	std::cout << "[W] Firefighter " << i << " is moving at " << ffSpeed << "m/s.\nThis is higher than the average of 1.4m/s" << std::endl;
      }
      //std::cout << "ffspeed: " << ffSpeed << std::endl; //[D]
      //TODO: ff have a probability of straying away from the right path
      //then select a random direction to move into wich lies ahead an in the most possible acute angle
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
