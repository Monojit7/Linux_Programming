/*

Facade pattern hides the complexity of the system and provides an interface to the client
using which client could access the system. this is part of structural design pattern

*/


#include <iostream>
#include <list>

using namespace std;

class Shape 
{
    public:
    virtual ~Shape ()
    {

    }
    virtual void draw () = 0;
};

class Circle : public Shape 
{
    public :

    void draw ()
    {
        cout << "draw circle " << endl;
    }
};

class Rectangle : public Shape 
{
    public :

    void draw ()
    {
        cout << "draw rectangle " << endl;
    }
};

class ShapeMaker
{
    public:

    Circle* mCircle = nullptr;
    Rectangle* mRectangle = nullptr;

    ShapeMaker()
    {
       mCircle = new Circle;
       mRectangle = new Rectangle;
       
    }

    void drawCircle ()
    {
        mCircle->draw();
    }

   void drawRect ()
    {
        mRectangle->draw();
    }
};

int main()
{
    ShapeMaker* mShapeMaker = new ShapeMaker;
    mShapeMaker->drawCircle();
    mShapeMaker->drawRect();
}





