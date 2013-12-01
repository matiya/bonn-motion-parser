#include <iostream>
#include <vector>
#include <fstream>
#include <cmath> 
#include <cstdlib>
 
//TODO: ask for the parameters in the cmd line
//TODO: set some more realistic parameters and maybe change FIELD_SIZE_COORDS to DISTANCE_BETWEEN_NODES
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
#define FILEPATH "../../data/scenario3.movements"
#define DOG_SPEED (SINE_ARC_LENGTH*DELTA_X*SPEED_NODES)/(16*PI)
#define concat(first, second) first second

class Node {
    std::vector<double> vectorPosition;
    double x, y;
    double speed;
    
  public:
    Node();
    void setPosition (double,double, double);
    void setSpeed(double);
    const std::vector<double> getPosition();
    const double getSpeed();
};

Node::Node ()
{
  speed = SPEED_NODES;
}


void Node::setPosition (double a, double b, double c) {
  vectorPosition.push_back(a);
  vectorPosition.push_back(b);
  vectorPosition.push_back(c);
}

void Node::setSpeed(double s){
  speed = s;
}

const double Node::getSpeed(){
  return speed;
}


const std::vector<double> Node::getPosition(){
  return vectorPosition;
}

int main(int argc, char **argv) {
    
  double time;
  double prevPosYFF, prevPosXFF, prevPosXD, prevPosYD, yPos;
  const double rfDist = (double) FIELD_SIZE_X / NUM_NODES;
  const double timeStep = (double) DURATION/SAMPLES_NUMBER;  std::cout << "timestep: " << timeStep << std::endl;
  std::ofstream resultsFile;
  resultsFile.open(FILEPATH, std::ofstream::out | std::ofstream::trunc);
  
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
      //prevPosY =  firefighter[i].getPos().at(1);
      prevPosYFF = *(firefighter[i].getPosition().end()-1);
      prevPosXFF = *(firefighter[i].getPosition().end()-2);
      prevPosXD = *(dog[i].getPosition().end()-2);
      prevPosYD = *(dog[i].getPosition().end()-1);
      firefighter[i].setPosition(time, prevPosXFF, prevPosYFF + timeStep*SPEED_NODES);
      dog[i].setPosition(time, *(dog[i].getPosition().begin()+1)+DELTA_X*cos(yPos/8), prevPosYD + timeStep*SPEED_NODES);

      //std::cout << "["<<time<<"] Firefighter " << i << " at ("<< prevPosXFF << ","<< prevPosYFF <<")" << std::endl;
      std::cout << "["<<time<<"] dog " << i << " at ("<< *(dog[i].getPosition().begin()+1)+DELTA_X*cos(yPos) << ","<< yPos <<")" << std::endl;
    }
    yPos += (timeStep*SPEED_NODES); //FIXME: there is no need for this, but I'm too tired to change it now
   
  }
  
  //writing output to a file
//     for(int j = 0; j < NUM_NODES; j++){ //circle through nodes
//       for(int i = 0; i < firefighter[j].getPosition().size(); i++){ //circle through node's vectorPosition content
// 	resultsFile << firefighter[j].getPosition().at(i) << " ";
//       }
//       resultsFile << std::endl;
//       for(int i = 0; i < dog[j].getPosition().size(); i++){ //circle through node's vectorPosition content
// 	resultsFile << dog[j].getPosition().at(i) << " ";
//       }
//       resultsFile << std::endl;
//     }


    for(int j = 0; j < NUM_NODES*2; j++){ //circle through nodes, all of them
 //FIXME: This is fugly, what's a better way to do this?
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
    //system(concat("gzip ", FILEPATH)); //FIXME: fugly 
    
    return 0;
}
