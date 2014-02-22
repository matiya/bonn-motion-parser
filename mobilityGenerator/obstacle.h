#ifndef OBSTACLE_H
#define OBSTACLE_H
#include<cmath>
#include <iostream>


extern unsigned long FIELD_SIZE_X;
class Obstacle {

public:
  struct vertices
  {
      double x1 = 0;
      double x2 = 0;
      double y1 = 0;
      double y2 = 0;
  };
  struct vertex
  {
    double x = 0;
    double y = 0;
  };
  struct linePointParameters
  {
      double px = 0;
      double py = 0;
      double r = 0;
      double s = 0;
      double distance = 0;
  };
  
private:  
  vertices vertiStruct;
  vertex vertixStruct;
  linePointParameters lpStruct;
  double L = 0;
  
public:
  Obstacle();
  vertex getClosestVertex(double, double);
  void setVertices(double, double, double, double);
  linePointParameters getDistanceToPoint(double , double);
  vertices getVertices();
};

#endif // OBSTACLE_H
