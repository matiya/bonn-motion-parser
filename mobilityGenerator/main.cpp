#include <iostream>
#include <vector>
#include <fstream>
#include <cmath> 

 
//TODO: ask for the parameters in the cmd line
//TODO: set some more realistic parameters and maybe change FIELD_SIZE_COORDS to DISTANCE_BETWEEN_NODES
#define FIELD_SIZE_X 200
#define FIELD_SIZE_Y 200
#define NUM_NODES 6
#define SPEED_NODES 0.4 //m/s
#define STARTING_POS_X 100
#define STARTING_POS_Y 100
#define SAMPLES_NUMBER 500
#define DURATION 900
#define DELTA_X 15 //(m) this is the distance the dogs will travel before returning
#define PI 3.14159265
#define CONSTAT_I_HAVE_TO_FIND_OUT 0.23
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
    
  double time;
  double prevPosYFF, prevPosXFF, prevPosXD, prevPosYD, yPos;
  double rfDist = (double) FIELD_SIZE_X / NUM_NODES;
  double timeStep = (double) DURATION/SAMPLES_NUMBER;  std::cout << "timestep: " << timeStep << std::endl;
  std::ofstream resultsFile;
  resultsFile.open("../../data/scenario1.movements", std::ofstream::out | std::ofstream::trunc);
  
  Node firefighter[NUM_NODES], dog[NUM_NODES]; 
  
  //set the intial positions
  //TODO: allow the user to enter positions through a txt file

  
  for(int i = 0; i < NUM_NODES ; i++){
    firefighter[i].setPosition(0, 0 + i*rfDist, 0);
    dog[i].setPosition(0, 0 + i*rfDist, 0);
  }
  
  //std::cout << firefighter[2].getPos().at(0) << std::endl;
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
      dog[i].setPosition(time, *(dog[i].getPosition().begin()+1)+DELTA_X*cos(yPos), prevPosYD + timeStep*SPEED_NODES);

      //std::cout << "["<<time<<"] Firefighter " << i << " at ("<< prevPosXFF << ","<< prevPosYFF <<")" << std::endl;
      std::cout << "["<<time<<"] dog " << i << " at ("<< *(dog[i].getPosition().begin()+1)+DELTA_X*cos(yPos) << ","<< yPos <<")" << std::endl;
    }
    yPos += (timeStep*SPEED_NODES); //FIXME: there is no need for this, but I'm too tired to change it now
   
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
      for(int i = 0; i < dog[j].getPosition().size(); i++){ //circle through node's vectorPosition content
	resultsFile << dog[j].getPosition().at(i) << " ";
      }
      resultsFile << std::endl;
    }
    
    resultsFile.close();
    /*
    double angle = 0.0, xpos, ypos;
    for (int i = 0; i < 100 ; i++){
      ypos += 0.1*PI;
      xpos = 5*cos(ypos);

      std::cout << xpos << " " << ypos << std::endl;
      
    }*/
    return 0;
}
