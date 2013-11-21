#include <iostream>
#include <vector>

//TODO: ask for the parameters in the cmd line

#define FIELD_SIZE_X 200
#define FIELD_SIZE_Y 200
#define NUM_NODES 3
#define SPEED_NODES 0.4 //m/s
#define STARTING_POS_X 100
#define STARTING_POS_Y 100
#define SAMPLES_NUMBER 900
#define DURATION 900


class Node {
    std::vector<double> vectorPos = std::vector<double>(2);
    double x, y;
    double speed;
  public:
    Node();
    void setPos (double,double);
    void setPos (double);
    void setSpeed(double);
    std::vector<double> getPos();
    double getSpeed();
};

Node::Node ()
{
  speed = SPEED_NODES;
}

void Node::setPos (double a, double b) {
  vectorPos.at(0) = a;
  vectorPos.at(1) = b;
}

void Node::setPos (double b) {
  vectorPos.at(1) = b;
}

void Node::setSpeed(double s){
  speed = s;
}

double Node::getSpeed(){
  return speed;
}


std::vector<double> Node::getPos(){
  return vectorPos;
}


int main(int argc, char **argv) {
    
  double time = 2, prevPos;
  double rfDist = FIELD_SIZE_X / NUM_NODES;
  double timeStep = DURATION/SAMPLES_NUMBER;
  
  Node firefighter[NUM_NODES], dog[NUM_NODES]; 
  
  //set the intial positions
  //TODO: allow the user to enter positions through a txt file

  
  for(int i = 0; i < NUM_NODES ; i++){
    firefighter[i].setPos(0 + i*rfDist, 0);
    std::cout << "Firefighter " << i << " at (" << i*rfDist << ",0)" << std::endl;
    dog[i].setPos(0 + i*rfDist, 0);
  }
  
  std::cout << firefighter[2].getPos().at(0) << std::endl;
  
  while(time <  DURATION){
    
    //update nodes positions
    for(int i = 0; i<NUM_NODES; i++){
      prevPos =  firefighter[i].getPos().at(1);
      firefighter[i].setPos(prevPos + timeStep*SPEED_NODES);
      std::cout << "Firefighter " << i << " at (x, "<< prevPos <<") @(" << time << " s)" << std::endl;
      
    }
    
    time += timeStep;
  }
 
  
    
    
    
    return 0;
}
