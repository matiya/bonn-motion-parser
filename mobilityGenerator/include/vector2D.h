#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>


class Vector2D
{
public:
  float   x;
  float   y;
  float   angle;
  
  Vector2D() {}
  
  Vector2D(float r, float s){
      x = r;
      y = s;
      //angle between x and y, in rads
      angle = std::atan(s/r);
  }

  Vector2D& Rotate(float angel);
  Vector2D& RotateT(float angel);
};

#endif