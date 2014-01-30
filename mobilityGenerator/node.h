#ifndef NODE_H
#define NODE_H

#include <cmath> 
#include <vector>
//TODO: error catching in the setter functions

class Node {
  
private:
  std::vector<double> vectorPosition;
  std::vector<double> nextPosition = {0, 0}; //C++11
  std::vector<double> versor = {0, 0}; //C++11;
  double x = 0, y = 0, xDir = 0, yDir = 0;
//     double speed = 0;
    
public:
  bool isAstray = false;
  bool isReturning = false;
  Node();
  void setPosition (double, double, double, double);
  void setVersor (double, double);
  void calcVersor ( double, double);
//     void setSpeed(double);
  const std::vector<double> getPosition();
  std::vector<double> getNextPosition();
  const std::vector<double> getVersor();
//     const double getSpeed();
};

Node::Node ()
{
//    std::cout << "vecor size: " << vectorPosition.size()<< std::endl;
   
}


void Node::setPosition (double x, double y, double t, double s) {
  vectorPosition.push_back(x);
  vectorPosition.push_back(y);
  vectorPosition.push_back(t);
  vectorPosition.push_back(s);
}

void Node::calcVersor ( double goalPosX, double goalPosY){
  /*calculate the versor which points in the direction goalPosition - currentPosition*/
  xDir = goalPosX - *(vectorPosition.end()-3);
  yDir = goalPosY - *(vectorPosition.end()-2);
  setVersor( xDir/std::sqrt(std::pow(xDir,2) + std::pow(yDir,2)) ,
	     yDir/std::sqrt(std::pow(xDir,2) + std::pow(yDir,2)));
  *(nextPosition.begin()) = goalPosX;
  *(nextPosition.begin()+1) = goalPosY;
  
}
void Node::setVersor (double a,double b){
  *(versor.begin()) = a;
  *(versor.begin() +1) = b;
}
/*
void Node::setSpeed(double s){
  speed = s;
}*/

const std::vector<double> Node::getPosition(){
  return vectorPosition;
}

std::vector<double> Node::getNextPosition(){
  return nextPosition;
}

const std::vector<double> Node::getVersor(){
  return versor;
}
/*
const double Node::getSpeed(){
  return speed;
}*/


#endif // NODE_H
