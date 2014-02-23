/**
 * @file node.h
 * @author Matias Siracusa
 * @version 
 * @brief node class
 * @date 22.02.2014
 * @bugs add error catching in the setter functions
*/

#ifndef NODE_H
#define NODE_H

#include <cmath> 
#include <vector>


/**
 * @brief enum containing the diferent states the nodes can have
 * 
 */
enum states_t {LINEAR, SINE, AVOIDING_OBSTACLE, LINEAR_AVOIDING_OBSTACLE, RETURNING, ASTRAY, RETURNING_TO_FF };

class Node {
  
private:
  /**
   * @brief vector which holds the position and the time and speed info of the nodes.
   * It stores for the whole running of the simulation, the data formatted as : {xPosition,yPosition,time, speed}
   * that will be later outputed to the ns_file file.
   */
  std::vector<double> vectorPosition;
  /**
   * @brief vector used to store the next position the node will head to
   * 
   */
  std::vector<double> nextPosition = {0, 0};
  
  /**
   * @brief versor that points in the direction of nextPosition
   * 
   */
  std::vector<double> versor = {0, 0}; //C++11;
    
public:
  
  /**
   * @brief variable to hold the node's current state
   * 
   */
  states_t state;
  
  /**
   * @brief constructor TODO: add the thing with the names 
   * 
   */
  Node();
  
  /**
   * @brief appends data to the vectorPosition vector
   * 
   * @param x x position
   * @param y y position
   * @param s speed
   * @param t time
   * @return void
   */
  void setPosition (double x, double y, double t, double s);
  /**
   * @brief writes into the versor vector a versor that points in 
   * the direction from current position to the next position.
   * 
   * @param  a x component
   * @param  b y component
   * @return void
   */
  void setVersor (double a, double b);
  /**
   * @brief calculate the versor 
   * 
   * @param  ...
   * @param  ...
   * @return void
   */
  void calcVersor ( double, double);
  const std::vector<double> getPosition();
  std::vector<double> getNextPosition();
  const std::vector<double> getVersor();
  std::vector<double> previousPosition = {0, 0};//C++11

};


#endif // NODE_H
