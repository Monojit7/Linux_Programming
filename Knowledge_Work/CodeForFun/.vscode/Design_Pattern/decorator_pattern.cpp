/*

decorator pattern is used when we need to add extra functionaly to a class without changing 
the class structure. Since extra functionality has been wrapped up to the existing class
this is part of structural pattern.

*/

#include <iostream>

using namespace std;

class Shape
{
  public:

  virtual void draw () = 0;
};


class Circle : public Shape
{
  public:

  void draw ()
  {
    cout << "drawing a Circle "<< endl;
  }
};

class Rectangle : public Shape
{
  public:

  void draw ()
  {
    cout << "drawing a Rectangle "<< endl;
  }
};

class ShapeDecorator : public Shape 
{

  protected :
  Shape* mShapeDecorator;
  public:

  ShapeDecorator ( Shape* mShapeDecorator )
  {
     this->mShapeDecorator = mShapeDecorator;
  }

  virtual void draw () = 0;


};

class RedShapeDecorator : public ShapeDecorator
{

  public:

  RedShapeDecorator ( Shape* mRedShapeDecorator ) : ShapeDecorator ( mRedShapeDecorator )
  {

  }

  void draw ()
  {
     mShapeDecorator->draw ();
     setRedBorderLine ( mShapeDecorator ); 

     
  }

  void setRedBorderLine ( Shape* mRedShapeDecorator )
  {
    cout << "Red Border Line is set " << endl;
  }

};

int main ()
{
  Shape* mCircle = new Circle ;

  mCircle->draw();

  Shape* mRectangle = new Rectangle ;

  mRectangle->draw();

  Shape* mRedCircle = new RedShapeDecorator (new Circle );

  mRedCircle->draw ();

  Shape* mRedRectangle = new RedShapeDecorator (new Rectangle );

  mRedRectangle->draw ();

}

