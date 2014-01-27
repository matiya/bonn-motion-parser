#ifndef OBSTACLE_H
#define OBSTACLE_H


class Obstacle {
  double x1 = 0;
  double x2 = 0;
  double y1 = 0;
  double y2 = 0;
public:
  Obstacle();
  void setVertices(double, double, double, double);
  void getDistanceToPoint(double, double);
};

Obstacle::Obstacle(){  
}

void Obstacle::setVertices(double a, double b, double c, double d){
  x1 = a;
  y1 = b;
  x2 = c;
  y2 = d;
}

void Obstacle::getDistanceToPoint(double a, double b){
//  std::cout << "gello" << std::endl;
}


#endif // OBSTACLE_H
