/*
Bridge pattern is a structural design pattern .

It decouples abstract class with its implementation class by introducing 
bridge implementer class . This bridge implementer could change the formation
of the object

*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;


// Here DrawAPI is a bridge implementer class
class DrawAPI 
{

  public :

  virtual void draw ( int x, int y, int radious ) = 0;

};

class RedCircle : public DrawAPI
{

  public:



  void draw ( int x , int y, int radious )
  {
      cout << " This is Red circle and x : " <<  x << " y " << y << " radious " << radious << endl;
  }

};


class GreenCircle : public DrawAPI
{

  public:



  void draw ( int x , int y, int radious )
  {
      cout << " This is green  circle and x : " <<  x << " y " << y << " radious " << radious << endl;
  }

};

class Shape 
{

  public:

  DrawAPI* mDrawAPI = nullptr;
  int x;
  int y;
  int radious;


  Shape ( DrawAPI* mDrawAPI  )
  {
      this->mDrawAPI = mDrawAPI;
  } 

  virtual void drawCircle (  ) = 0;


};

class Circle : public Shape
{
  public:
    
    Circle (  int x, int y, int radious, DrawAPI* mDrawAPI ): Shape ( mDrawAPI )
    {
        this->x = x;
        this->y = y;
        this->radious = radious;
    }

    void drawCircle ()
    {
        this->mDrawAPI->draw ( x, y, radious);
    } 
  
};

int main ()
{
    Shape* redCircle = new Circle ( 10, 15, 7, new RedCircle );
    Shape* greenCircle = new Circle ( 4, 6, 12, new GreenCircle );

    redCircle->drawCircle();
    greenCircle->drawCircle();
}