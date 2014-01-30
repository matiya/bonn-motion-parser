#ifndef OBSTACLE_H
#define OBSTACLE_H


class Obstacle {

public:
  struct vertices
  {
      double x1 = 0;
      double x2 = 0;
      double y1 = 0;
      double y2 = 0;
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
  linePointParameters lpStruct;
  double L = 0;
  
public:
  Obstacle();
  
  void setVertices(double, double, double, double);
  linePointParameters getDistanceToPoint(double , double);
  vertices getVertices();
};

Obstacle::Obstacle(){  
}

void Obstacle::setVertices(double a, double b, double c, double d){
  vertiStruct.x1 = a;
  vertiStruct.y1 = b;
  vertiStruct.x2 = c;
  vertiStruct.y2 = d;
  L = std::sqrt(std::pow((vertiStruct.y2-vertiStruct.y1),2) + std::pow((vertiStruct.x2-vertiStruct.x1),2));
  std::cout << "L : " << L << std::endl;
}

Obstacle::vertices Obstacle::getVertices(){
  return vertiStruct;
}

Obstacle::linePointParameters Obstacle::getDistanceToPoint(double a, double b){
//   Logic taken from http://www.exaflop.org/docs/cgafaq/cga1.html

  lpStruct.r = ( (vertiStruct.y1 - b)*(vertiStruct.y1-vertiStruct.y2) - (vertiStruct.x1 - a)*(vertiStruct.x2-vertiStruct.x1) )/( std::pow(L,2) );
  lpStruct.s = ( (vertiStruct.y1 - b)*(vertiStruct.y2-vertiStruct.y1) - (vertiStruct.x1 - a)*(vertiStruct.x2-vertiStruct.x1) )/( std::pow(L,2) );
  lpStruct.distance = lpStruct.s*L;


  if(lpStruct.s*L < 40 && lpStruct.s*L > 0 && lpStruct.s>0 && lpStruct.r<1 ){
    //FIXME: This is totally off!!
      lpStruct.px = vertiStruct.x1 + lpStruct.r*(vertiStruct.x2-vertiStruct.x1);
      lpStruct.py = vertiStruct.y1 + lpStruct.r*(vertiStruct.y2-vertiStruct.y1);
//       std::cout << c <<" distance to point: " << r << "\t" << s*L << std::endl;
    }
  
  return lpStruct;

}


#endif // OBSTACLE_H
