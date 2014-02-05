#ifndef OBSTACLE_H
#define OBSTACLE_H

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

Obstacle::Obstacle(){  
}

void Obstacle::setVertices(double a, double b, double c, double d){
  vertiStruct.x1 = a;
  vertiStruct.y1 = b;
  vertiStruct.x2 = c;
  vertiStruct.y2 = d;
  L = std::sqrt(std::pow((vertiStruct.y2-vertiStruct.y1),2) + std::pow((vertiStruct.x2-vertiStruct.x1),2));
//   std::cout << "L : " << L << std::endl;
}

Obstacle::vertices Obstacle::getVertices(){
  return vertiStruct;
}

Obstacle::vertex Obstacle::getClosestVertex(double a, double b){
  double lDistance = 0;
  double rDistance = 0;
  rDistance = std::sqrt( std::pow(vertiStruct.x2 -a,2) + std::pow(vertiStruct.y2 -b,2));
  lDistance = std::sqrt( std::pow(vertiStruct.x1 -a,2) + std::pow(vertiStruct.y1 -b,2));
//   std::cout << "X: " << FIELD_SIZE_X << std::endl;
  if(rDistance >  lDistance){
    if(vertiStruct.x1 - 5 > 0){
      vertixStruct.x = vertiStruct.x1;
      vertixStruct.y = vertiStruct.y1;
    }
    else{
      vertixStruct.x = vertiStruct.x2;
      vertixStruct.y = vertiStruct.y2;
    }
  }
  else if(rDistance <  lDistance){
    if(vertiStruct.x2 + 5 < FIELD_SIZE_X){
      vertixStruct.x = vertiStruct.x2;
      vertixStruct.y = vertiStruct.y2;
    }
    else{
      vertixStruct.x = vertiStruct.x1;
      vertixStruct.y = vertiStruct.y1;
    }
  }
  else if(rDistance == lDistance){
    if(vertiStruct.x1 == vertiStruct.x2){
      if(vertiStruct.y1 == vertiStruct.y2){
	std::cout << "[W] Trivial definition of obstacle. "<< std::endl; //FIXME
      }
      std::cout << "[W][W] "<< std::endl; //FIXME
    }
    vertixStruct.x = vertiStruct.x1;
    vertixStruct.y = vertiStruct.y1;
  }
  else{
    std::cout << "[E] Obstacle can't be avoided." << std::endl;
    std::terminate();
  }
  return vertixStruct;
  
}

Obstacle::linePointParameters Obstacle::getDistanceToPoint(double px, double py){
//   Logic taken from http://www.exaflop.org/docs/cgafaq/cga1.html
//http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html  
//   double ix, iy;
//   double dist;
//   double temp;
  lpStruct.r = ( (vertiStruct.y1 - py)*(vertiStruct.y1-vertiStruct.y2) - (vertiStruct.x1 - px)*(vertiStruct.x2-vertiStruct.x1) )/( std::pow(L,2) );
  lpStruct.s = ( (vertiStruct.y1 - py)*(vertiStruct.y2-vertiStruct.y1) - (vertiStruct.x1 - px)*(vertiStruct.x2-vertiStruct.x1) )/( std::pow(L,2) );
  //lpStruct.distance = lpStruct.s*L;
//   lpStruct.distance = std::abs( (vertiStruct.y1 - py)*(vertiStruct.x2-vertiStruct.x1) - (vertiStruct.x1 - px)*(vertiStruct.y2-vertiStruct.y1) )/L;
//   temp = (((px - vertiStruct.x1) * (vertiStruct.x2 - /*vertiStruct*/.x1)) + ((py - vertiStruct.y1) * (vertiStruct.y2 - vertiStruct.y1)))/( std::pow(L,2) );
  if ( (lpStruct.r <= 0.0 || lpStruct.r > 1.0 ) ){
    lpStruct.px =  std::sqrt(std::pow((vertiStruct.y2-py),2) + std::pow((vertiStruct.x2-px),2));
    lpStruct.py =  std::sqrt(std::pow((vertiStruct.y1-py),2) + std::pow((vertiStruct.x1-px),2));
    if(lpStruct.px > lpStruct.py){
      lpStruct.distance = lpStruct.py;
//       std::cout <<"lpStruct.py : " << lpStruct.py << std::endl;
    }
    else{
      lpStruct.distance = lpStruct.px;
//       std::cout <<"lpStruct.px : " << lpStruct.px << std::endl;
    }
  }
  else{
    lpStruct.px = vertiStruct.x1 + lpStruct.r * (vertiStruct.x2 - vertiStruct.x1);
    lpStruct.py = vertiStruct.y1 + lpStruct.r * (vertiStruct.y2 - vertiStruct.y1);
    lpStruct.distance = std::sqrt(std::pow((lpStruct.py-py),2) + std::pow((lpStruct.px-px),2));
//     std::cout << " r: " << lpStruct.r << "\t s: " << lpStruct.s << "\t distance : " << lpStruct.distance << "\t temp: " << temp << std::endl;
    if(py > vertiStruct.y1 && py > vertiStruct.y2){//obstacle lies behind node
//       vertiStruct.y1 > vertiStruct.y2 ? lpStruct.py = vertiStruct.y1 : lpStruct.py = vertiStruct.y2;
      lpStruct.r = 100;
    }
    else if ( (px > vertiStruct.x1 && px > vertiStruct.x2) || (px < vertiStruct.x1 && px < vertiStruct.x2)){ //obstacle is out of the way
      lpStruct.r = 100;
    }
  }
//  if(lpStruct.py < py){ //obstacle lies behind node
//     lpStruct.r = 10;
//   }
  
  return lpStruct;
}


#endif // OBSTACLE_H
