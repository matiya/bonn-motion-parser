#include "vector2D.h"




Vector2D& Vector2D::Rotate(float angel)
{
    float s = std::sin(angel);
    float c = std::cos(angel);
    
    float nx = c * x - s * y;
    float ny = s * x + c * y;
    
    x = nx;
    y = ny;
    
    return (*this);
}

