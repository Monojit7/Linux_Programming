/*

Factory pattern is a creational design pattern. 

In Factory pattern object creation logic will be hidden , instead
object will be created using factory method.

Shape is a interface class which has draw method

Circle, Rectangle implements Shapes draw method

ShapeFactory is a class which has getShape method

which takes sharetype as argument and returns Shape object


*/

#include <iostream>

using namespace std;

class Shape
{

public:
    virtual void draw() = 0;
};

class Circle : public Shape
{
public:
    void draw()
    {
        cout << "drawing circle " << endl;
    }
};

class Rectangle : public Shape
{
public:
    void draw()
    {
        cout << "drawing Rectangle " << endl;
    }
};

class ShapeFactory
{
public:
    Shape *getShape(string shape)
    {
        if (Compare(shape, "circle"))
            return new Circle;
        else if (Compare(shape, "rectangle"))
            return new Rectangle;
    }

private:
    bool Compare(string a1, string a2)
    {
        if (a1 == a2)
            return true;
        else
        {
            return false;
        }
    }

};

int main()
{
    ShapeFactory *shapefactory = new ShapeFactory;
    Shape *shape_circle = shapefactory->getShape("circle");
    shape_circle->draw();

    Shape *shape_rect = shapefactory->getShape("rectangle");
    shape_rect->draw();
}
