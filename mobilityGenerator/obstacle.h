/**
 * @file obstacle.h
 * @author Matias Siracusa
 * @version 
 * @brief obstacle class
 * @date 22.02.2014
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H
#include<cmath>
#include <iostream>


extern unsigned long FIELD_SIZE_X;
/**
 * @brief Class to define the behaviour of obstacles in the field.
 * An obstacle is defined as a line segment given by two points (x1,y1) and (x2,y2). 
 * Several line segments can form a polygon or not, that's left to the user. 
 * An obstacle definition is given in a separated file (-o flag) as a list of floats delimited by commas. Like this:
 * 1.0,2,3.1,4,5,6,7,8
 * That would define two line segments in (1.0;2.3), (3.1,4) and (5,6);(7,8).
 * Any node would have to notice an obstacle as soon as it enters its line of sight, then ask itself if given its current 
 * movement, a collision might happen. If yes, calculate the shortest path to avoid collision. While avoiding an obstacle, 
 * both ff and dogs would leave their charachteristic movement, at least at first.
 * After _cf47038600_, when an obstacle gets in the line of sight of a firefighter it will calculate the closest vertex of 
 * the obstacle and the it will head into a direction given by (closestVertexX +- 5; closestVertexY +-5) as long as that 
 * position lies inside the field. 
 * If there are no obstacles in the new calc position, the ff will head in that direction. It will then try to return to its 
 * original position and if not possible (because there is an obstacle in the way) it will continue until the way is clear. 
 * The dog interrupts its movement and tags along the ff.
 */
class Obstacle {

public:
  /**
   * @brief The vertices structure defines a line segment from (x1,y1) to (x2,y2)
   * 
   */
  struct vertices
  {
      double x1 = 0;
      double x2 = 0;
      double y1 = 0;
      double y2 = 0;
  };
  /**
   * @brief This structure defines a single vertex in (x,y)
   * 
   */
  struct vertex
  {
    double x = 0;
    double y = 0;
  };
  /**
   * @brief This structure holds information useful to determine whether a line is in front of a point.
   * 
   */
  struct linePointParameters
  {
  
      double px = 0; /*<x coordinate of the projection of the point along the line*/
      double py = 0; /*<y coordinate of the projection of the point along the line*/
      double r = 0; /*< parameter that indicates, whether the point falls within the line, when 0<r<1*/
      double s = 0; /*< parameter that indicates, whether the point falls within the line, when s=0*/
      double distance = 0; /*<distance between point and line*/
  };
  
private:  
  /**
   * @brief This structure contains the vertices of each obstacle.
   * 
   */
  vertices vertiStruct;
  /**
   * @brief This structure contains one vertex of an obstacle.
   * 
   */
  vertex vertixStruct;
  /**
   * @brief This structure contains information about the distance and possible collision between
   * a node and an obstacle.
   * 
   */
  linePointParameters lpStruct;
  /**
   * @brief Length in meters of the line segment that describe an obstacle.
   * 
   */
  double lengthOfLineSegment = 0;
  
public:
  /**
   * @brief Default constructor
   * 
   */
  Obstacle();
  /**
   * @brief Returns an obstacle's closest vertex to the point (a,b).
   * It checks whether the path to avoid said vertex lies within 0 and FIELD_SIZE_X. If not
   * it returns the other vertex.
   * If both vertex are equal (x1,y1) == (x2,y2), throws a warning and returns either vertex.
   * 
   * @param  a x coordinate of the point we need to find the closest vertex to
   * @param  b y coordinate of the point we need to find the closest vertex to
   * @return Obstacle::vertex Closest vertex to (a,b)
   */
  vertex getClosestVertex(double a, double b);
  /**
   * @brief Sets the vertiStruct structure and calculates the lengthOfLineSegment parameter.
   * 
   * @param  a x coordinate of 1st vertex
   * @param  b y coordinate of 1st vertex
   * @param  c x coordinate of 2nd vertex
   * @param  d y coordinate of 2nd vertex
   * @return void
   */
  void setVertices(double a, double b, double c, double d);
  /**
   * @brief Returns a structure with info about the position of a point relative to the obstacle.
   * It takes a point (a node position) as a parameter, then it calculates where in the space said point is,
   * relative to the position of the obstacle. Thus, it can be known whether the node will collide into the
   * obstacle.
   * 
   * @param  px x coordinate of point
   * @param  py y coordinate of point
   * @return Obstacle::linePointParameters
   */
  linePointParameters getDistanceToPoint(double px, double py);
  /**
   * @brief Returns a structure with the vertices that conform the obstacle.
   * 
   * @return Obstacle::vertices
   */
  vertices getVertices();
};

#endif // OBSTACLE_H
 