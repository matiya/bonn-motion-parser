#include "vector2D.h"
#include <iostream>

/**
 * @brief Rotates the x and y coordinates anticlock-wise
 * by an angle of angel
 * 
 * @param angel ...
 * @return Vector2D&
 */
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

/**
 * @brief Same as Rotate but clock wise
 * 
 * @param angel ...
 * @return Vector2D&
 */
Vector2D& Vector2D::RotateT(float angel)
{
    float s = std::sin(angel); //-1
    float c = std::cos(angel); //0
    
    float nx =  c * x + s * y;
    float ny = -s * x + c * y;

    x = nx;
    y = ny;
    
    return (*this);
}