#ifndef NODE_H
#define NODE_H

//TODO: error catching in the setter functions

class Node {
    std::vector<double> vectorPosition;
    std::vector<double> nextPosition;
    std::vector<double> versor;
    double x, y;
    double speed;
    
  public:
    bool isAstray = false;
    bool isReturning = false;
    Node();
    void setPosition (double,double, double);
    void setNextPosition (double,double);
    void setVersor (double,double);
    void setSpeed(double);
    const std::vector<double> getPosition();
    const std::vector<double> getNextPosition();
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

void Node::setNextPosition (double a,double b){
  nextPosition.push_back(a);
  nextPosition.push_back(b);
}


void Node::setVersor (double a,double b){
  versor.push_back(a);
  versor.push_back(b);
}

void Node::setSpeed(double s){
  speed = s;
}

const std::vector<double> Node::getPosition(){
  return vectorPosition;
}

const std::vector<double> Node::getNextPosition(){
  return nextPosition;
}

const std::vector<double> Node::getVersor(){
  return versor;
}

const double Node::getSpeed(){
  return speed;
}


#endif // NODE_H
