#ifndef NODE_H
#define NODE_H

#include <cmath> 
#include <vector>
//TODO: error catching in the setter functions

class Node {
    std::vector<double> vectorPosition;
    std::vector<double> nextPosition = {0, 0}; //C++11
    std::vector<double> versor = {0, 0}; //C++11;
    double x, y, xDir, yDir;
    double speed;
    
  public:
    bool isAstray = false;
    bool isReturning = false;
    Node();
    void setPosition (double,double, double);
    void setVersor (double,double);
    void calcVersor ( double, double);
    void setSpeed(double);
    const std::vector<double> getPosition();
    std::vector<double> getNextPosition();
    const std::vector<double> getVersor();
    const double getSpeed();
};

Node::Node ()
{
  //speed = SPEED_NODES;
}


void Node::setPosition (double a, double b, double c) {
  vectorPosition.push_back(a);
  vectorPosition.push_back(b);
  vectorPosition.push_back(c);
}

void Node::calcVersor ( double goalPosX, double goalPosY){
  /*calculate the versor which points in the direction goalPosition - currentPosition*/
  xDir = goalPosX - *(vectorPosition.end()-2);
  yDir = goalPosY - *(vectorPosition.end()-1);
  setVersor( xDir/std::sqrt(std::pow(xDir,2) + std::pow(yDir,2)) ,
	     yDir/std::sqrt(std::pow(xDir,2) + std::pow(yDir,2)));
  *(nextPosition.begin()) = goalPosX;
  *(nextPosition.begin()+1) = goalPosY;
  
}
void Node::setVersor (double a,double b){
  *(versor.begin()) = a;
  *(versor.begin() +1) = b;
}

void Node::setSpeed(double s){
  speed = s;
}

const std::vector<double> Node::getPosition(){
  return vectorPosition;
}

std::vector<double> Node::getNextPosition(){
  return nextPosition;
}

const std::vector<double> Node::getVersor(){
  return versor;
}

const double Node::getSpeed(){
  return speed;
}


#endif // NODE_H
