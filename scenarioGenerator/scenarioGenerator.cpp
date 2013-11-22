#include <iostream>
#include <vector>
#include <fstream> 

 
//TODO: ask for the parameters in the cmd line

#define FIELD_SIZE_X 200
#define FIELD_SIZE_Y 200
#define NUM_NODES 3
#define SPEED_NODES 0.4 //m/s
#define STARTING_POS_X 100
#define STARTING_POS_Y 100
#define SAMPLES_NUMBER 500
#define DURATION 900


class Node {
    std::vector<double> vectorPos = std::vector<double>(2);
    std::vector<double> vectorPosition;
    double x, y;
    double speed;
    
  public:
    Node();
    void setPosition (double,double, double);
    void setSpeed(double);
    std::vector<double> getPosition();
    double getSpeed();
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

double Node::getSpeed(){
  return speed;
}


std::vector<double> Node::getPosition(){
  return vectorPosition;
}

int main(int argc, char **argv) {
    
  double time, prevPosY, prevPosX;
  double rfDist = (double) FIELD_SIZE_X / NUM_NODES;
  double timeStep = (double) DURATION/SAMPLES_NUMBER;  std::cout << "timestep: " << timeStep << std::endl;
  std::ofstream resultsFile;
  resultsFile.open("scenario.movements", std::ofstream::out | std::ofstream::trunc);
  
  Node firefighter[NUM_NODES], dog[NUM_NODES]; 
  
  //set the intial positions
  //TODO: allow the user to enter positions through a txt file

  
  for(int i = 0; i < NUM_NODES ; i++){
    firefighter[i].setPosition(0, 0 + i*rfDist, 0);
    dog[i].setPosition(0, 0 + i*rfDist, 0);
  }
  
  //std::cout << firefighter[2].getPos().at(0) << std::endl;
  
  
  while(time < DURATION){
    //update nodes positions
    //TODO: throw exceptions when surpassing field limits
    //TODO: randomize speed and x movement
    time += timeStep;
    for(int i = 0; i<NUM_NODES; i++){
      //prevPosY =  firefighter[i].getPos().at(1);
      prevPosY = *(firefighter[i].getPosition().end()-1);
      prevPosX = *(firefighter[i].getPosition().end()-2);
//      firefighter[i].setPos(prevPosY + timeStep*SPEED_NODES);
      firefighter[i].setPosition(time, prevPosX, prevPosY + timeStep*SPEED_NODES);
      std::cout << "["<<time<<"] Firefighter " << i << " at ("<< prevPosX << ","<< prevPosY <<")" << std::endl;
    }
   
  }
//  for(int i = 0; i < firefighter[1].getPosition().size(); i++)
//  {
//     std::cout << " " << firefighter[1].getPosition().at(i);
//  }
  //writing output to a file
    for(int j = 0; j < NUM_NODES; j++){ //circle through ndoe
      for(int i = 0; i < firefighter[j].getPosition().size(); i++){ //circle through node's vectorPosition content
	resultsFile << firefighter[j].getPosition().at(i) << " ";
      }
      resultsFile << std::endl;
    }
    
    resultsFile.close();
    return 0;
}
