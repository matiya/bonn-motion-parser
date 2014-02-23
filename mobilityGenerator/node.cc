/**
 * @file node.cc
 * @author Matias Siracusa
 * @version 
 * @brief node class
 * @date 22.02.2014
*/
#include "node.h"


Node::Node ()
{
 //TODO: name here  
}


void Node::setPosition (double x, double y, double t, double s) {
  vectorPosition.push_back(x);
  vectorPosition.push_back(y);
  vectorPosition.push_back(t);
  vectorPosition.push_back(s);
}

void Node::calcVersor ( double goalPosX, double goalPosY){
  /*calculate the versor which points in the direction goalPosition - currentPosition*/
  double xDir = goalPosX - *(vectorPosition.end()-3);
  double yDir = goalPosY - *(vectorPosition.end()-2);
  setVersor( xDir/std::sqrt(std::pow(xDir,2) + std::pow(yDir,2)) ,
	     yDir/std::sqrt(std::pow(xDir,2) + std::pow(yDir,2)));
  *(nextPosition.begin()) = goalPosX;
  *(nextPosition.begin()+1) = goalPosY;
  
}
void Node::setVersor (double a,double b){
  *(versor.begin()) = a;
  *(versor.begin() +1) = b;
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