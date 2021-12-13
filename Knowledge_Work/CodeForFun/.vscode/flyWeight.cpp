/*

Flyweight design pattern is primary used to reduce the number of objects created and decrease 

memory footprint and increase performance.

This is part of structural design pattern since this decreases object count and increases 

object structure.

Flyweight design pattern tries to reuse already existing similar kind of objects by storing 

them and creates new object when no matching object found. We will demonstrate this pattern 

by drawing 20 circles of different locations but we will create only 5 object . Only 5 colors 

are present hence colour property is used to check already existing circle objects.

*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <ctime>
#include <vector>

using namespace std;

class Shape
{

public:
    virtual void draw() = 0;
};

class Circle : public Shape
{

private:
    string color;
    int x;
    int y;
    int radious;

public:
    Circle(string color)
    {
        this->color = color;
    }

    void setX(int x)
    {
        this->x = x;
    }

    void setY(int y)
    {
        this->y = y;
    }

    void setRadious(int radious)
    {
        this->radious = radious;
    }

    void draw()
    {
        cout << "Drawing Circle [ color : " << this->color << " x : " << this->x << " y : " << y << " radious : " << this->radious << "]" << endl;
    }
};

class ShapeFactory
{

private:
    unordered_map<string, Shape *> mUnMap = {};

public:
    // this method also can be static method
    Shape *getCircle(string color)
    {
        Shape *vShape = nullptr;
        auto it = this->mUnMap.find(color);

        if (it == this->mUnMap.end())
        {
            vShape = new Circle(color);
            this->mUnMap.insert(make_pair(color, vShape));
            cout << " creating new circle of color : " << color << endl;
        }
        else
        {
            cout << "reUsing the existing circle of color " << color << endl;
            vShape = it->second;
        }

        return vShape;
    }
};

class cRanDomGen
{
public:
    cRanDomGen()
    {
    }

    string getRandomizeColors(std::vector<string> colors)
    {

        string random = colors[rand() % colors.size()];

        return random;
    }

    int getRandomTill(int range)
    {
        cout << "range " << std::rand() / ((RAND_MAX + 1u) / (range - 1)) << endl;
        return rand() % range;
    }

    int getRandomX()
    {

        return getRandomTill(100);
    }

    int getRandomY()
    {

        return getRandomTill(55);
    }
    int getRandomRadious()
    {

        return getRandomTill(45);
    }
};

int main()
{

    // creating 20 circles out of 5 colors

    ShapeFactory *mShapeFactory = new ShapeFactory;

    srand(std::time(0));

    cRanDomGen *mRanDomGen = new cRanDomGen;

    vector<string> colors = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE"};

    for (int i = 0; i < 20; i++)
    {
        Circle *circle = dynamic_cast<Circle *>(mShapeFactory->getCircle(mRanDomGen->getRandomizeColors(colors)));

        circle->setX(mRanDomGen->getRandomX());

        circle->setY(mRanDomGen->getRandomY());

        circle->setRadious(mRanDomGen->getRandomRadious());

        circle->draw();
    }
}