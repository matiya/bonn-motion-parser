#ifndef NODE_H
#define NODE_H

#include <cmath> 
#include <vector>
//TODO: error catching in the setter functions

enum states_t {LINEAR, SINE, AVOIDING_OBSTACLE, LINEAR_AVOIDING_OBSTACLE, RETURNING, ASTRAY, RETURNING_TO_FF };

class Node {
  
private:
  /**
   * @brief vector which holds the position and the time and speed info of the nodes.
   * It's formatted as : {xPosition,yPosition,time, speed}
   */
  std::vector<double> vectorPosition;
  /**
   * @brief vector used rr
   * 
   */
  std::vector<double> nextPosition = {0, 0};
  std::vector<double> versor = {0, 0}; //C++11;
  double x = 0, y = 0, xDir = 0, yDir = 0;
    
public:
  states_t state;
//   bool isAstray = false;
//   bool isReturning = false;
//   bool isAvoidingObstacle = false;
  Node();
  void setPosition (double, double, double, double);
  void setVersor (double, double);
  void calcVersor ( double, double);
  const std::vector<double> getPosition();
  std::vector<double> getNextPosition();
  const std::vector<double> getVersor();
  std::vector<double> previousPosition = {0, 0};//C++11

};


#endif // NODE_H
