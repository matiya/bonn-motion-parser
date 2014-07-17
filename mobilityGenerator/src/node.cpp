/**
 * @file node.cc
 * @author Matias Siracusa
 * @version
 * @brief node class
 * @date 22.02.2014
*/
#include "node.h"
#include <iostream>

Node::Node() {
    // TODO: name here
  vectorPosition.reserve(4*SAMPLES_NUMBER + 4 );
}


void Node::setPosition(double t, double x, double y, double s) {
  vectorPosition.push_back(t);
  vectorPosition.push_back(x);
  vectorPosition.push_back(y);
  vectorPosition.push_back(s);
}

/**
 * @brief calculate the unit vector which points in the direction goalPosition - currentPosition
 * @param goalPosX 
 * @param goalPosY ...
 * @return void
 */
void Node::calcVersor(double goalPosX, double goalPosY) {
  double xDir = goalPosX - *(vectorPosition.end() - 3);
  double yDir = goalPosY - *(vectorPosition.end() - 2);
  double length = std::sqrt(std::pow(xDir, 2) + std::pow(yDir, 2));
  if (length == 0){
    setVersor(0, 0);
  }
  else{
    setVersor(xDir / length, yDir / length);
    nextPosition[0] = goalPosX;
    nextPosition[1] = goalPosY;
  }
}



void Node::setVersor(double a, double b) {
  versor[0] = a;
  versor[1] = b;
}

const std::vector<double> Node::getPosition() { return vectorPosition; }

std::vector<double> Node::getNextPosition() { return nextPosition; }

const std::vector<double> Node::getVersor() { return versor; }