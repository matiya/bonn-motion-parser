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
#include "vector2D.h"

extern unsigned long SAMPLES_NUMBER;
/**
 * @brief enum containing the diferent states the nodes can have
 *
 */
enum states_t {
  LINEAR,
  SINE,
  AVOIDING_OBSTACLE,
  LINEAR_AVOIDING_OBSTACLE,
  RETURNING,
  ASTRAY,
  RETURNING_TO_FF
};

class Node {

private:
  /**
   * @brief vector which holds the position and the time and speed info of the
   * nodes.
   * It stores for the whole running of the simulation, the data formatted as :
   * {xPosition,yPosition,time, speed}
   * that will be later outputed to the ns_file file.
   */
  std::vector<double> vectorPosition;
  /**
   * @brief vector used to store the next position the node will head to
   *
   */
  std::vector<double> nextPosition = { 0, 0 };

  /**
   * @brief versor that points in the direction of nextPosition
   *
   */
  std::vector<double> versor = { 0, 0 }; // C++11;

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
   * @brief speed of the node, only used when the node is either slowing down or
   *speeding up.
   *
   */
  double alteredSpeed = 0;
  /**
   * @brief time in senconds the nodes' speed is altered
   *
   */
  double powerUpTime = 0;
  /**
   * @brief Firefighter's deviation along the x axis. Simulates small object
   *avoidance.
   *
   */
  double xDeviation = 0;
  /**
   * @brief Time to wait before the node can go into the ASTRAY state again.
   * The idea behind this is that the node can't get into the ASTRAY state twice
   * in a row, as it would be unlikely that a dog that returns to its handle would
   * go astray again after a couple of seconds.
   * 
   */
  double refractoryPeriod = 0;
  /**
   * @brief Time variable that helps the node's remember where in the sine wave
   * they left off when the later has to be interrupted (i.e. the dogs swerve or
   * slow down)
   * 
   */
  double scaleSine = 0;
  
  /**
   * @brief Firefighters use this vector to store the angle of movement.
   * 
   */
  Vector2D directionVector;

  /**
   * @brief appends data to the vectorPosition vector
   *
   * @param x x position
   * @param y y position
   * @param s speed
   * @param t time
   * @return void
   */
  void setPosition(double t, double x, double y, double s);
  /**
   * @brief writes into the versor vector a versor that points in
   * the direction from current position to the next position.
   *
   * @param  a x component
   * @param  b y component
   * @return void
   */
  void setVersor(double a, double b);
  /**
   * @brief calculates the versor which points in the direction goalPosition -
   *currentPosition.
   * It gets the goal position as a parameter and gets the current postition
   *from the last stored
   * values in the vectorPosition. It then calls the setVersor function with the
   *calculated value and
   * stores the goalPosition in the nextPosition vector.
   *
   * @param  goalPosX  x coordinate of the goal position
   * @param  goalPosY  y coordinate of the goal position
   * @return void
   */
  void calcVersor(double goalPosX, double goalPosY);
  /**
   * @brief returns the vectorPosition vector
   *
   * @return const std::vector< double, std::allocator >
   */
  const std::vector<double> getPosition();

  /**
   * @brief returns the nextPosition vector
   *
   * @return std::vector< double, std::allocator >
   */
  std::vector<double> getNextPosition();

  /**
   * @brief return the versor vector
   *
   * @return const std::vector< double, std::allocator >
   */
  const std::vector<double> getVersor();

  /**
   * @brief vector used to store a position the node must return to.
   *
   */
  std::vector<double> previousPosition = { 0, 0 }; // C++11
};

#endif // NODE_H
