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
      //angle between sides, in rads
      angle = std::atan(s/r);
  }
  
//   Vector2D& operator *=(float t){
//     x *= t;
//     y *= t;
//     return (*this);
//   }
//   
//   float operator *(const Vector2D& v) const{
//     return (x * v.x + y * v.y);
//   }
//   
//   Vector2D& operator /=(float t)
//   {
//       float f = 1.0F / t;
//       x *= f;
//       y *= f;
//       return (*this);
//   }
// 
//   Vector2D& Normalize(void)
//   {
//       return (*this /= std::sqrt(x * x + y * y));
//   }
//   
  Vector2D& Rotate(float angel);
};



// inline float Dot(const Vector2D& v1, const Vector2D& v2)
// {
//     return (v1 * v2);
// }
// 
// inline float Magnitude(const Vector2D& v)
// {
//     return (std::sqrt(v.x * v.x + v.y * v.y));
// }
// 
// inline float InverseMag(const Vector2D& v)
// {
//     return (1.0F / sqrtf(v.x * v.x + v.y * v.y));
// }
// 
// inline float SquaredMag(const Vector2D& v)
// {
//     return (v.x * v.x + v.y * v.y);
// }

#endif