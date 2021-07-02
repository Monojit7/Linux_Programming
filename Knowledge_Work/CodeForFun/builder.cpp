/*

Builder pattern is a creational design pattern. When a complex object is build
in step by step out of multiple simple objects

*/


/*

In the below example nonveg meal and veg meal and its price is calculater 
using builder patter.

problem : veg burger , chicken burger is packed with wrapper
          coke , pepsi is packed with bottle


There will be item interface class which will be having name , packing object type and price 
virtual functions .

packing will be another abstract class which will be having pack pure virtual function

burger is inherited from item and will be abstract class and only implements packing and leaves  name and price as pure virtual

cold drinks will be inherited from item and will ibe abstract class and only implement pack and leaves  name and price as pure virtual

wrapper will be inherited from packing and will implement pack 

bottle will be inherited from packing and will implement pack

vegburger will be inherited from burger and will implement name and price

chicken burger will be inherited from burger and will implement name and price

pepsi and coke will be inherited from colddrinks and will implement name and price

meal will be having list of items as attribute  and showItems , add items as method

mealbuilder is a class which has two functions prepareVegMeal and prepareNonVegMeal and 

both the functions return as meal .

prepareVegMeal creates a meal by adding items in it 



*/

#include <iostream>
#include <list>

using namespace std;

class Packing;

class Item
{
    public :
    virtual string name() = 0;
    virtual int price() = 0;

    virtual Packing *packing () = 0;

};

class Packing 
{
    public:
    virtual string pack () = 0;
};

class Wrapper : public Packing
{
    public:
    string pack ()
    {
        return "wrapper packed ";
    }
};

class Bottle : public Packing
{
    public:
    string pack ()
    {
        return "Bottle packed ";
    }
};

class Burger : public Item
{
    public:
    virtual string name ()
    {
       return "";
    }
    virtual int price ()
    {
       return 0;
    }

    Packing* packing ()
    {
        return new Wrapper;
    }
};

class ColdDrinks : public Item 
{
    public:
   virtual string name ()
   {
       return "";
   }
    virtual int price ()
    {
       return 0;
    }

    Packing *packing ()
    {
        return new Bottle;
    }
};


class VegBurger : public Burger
{
    public :

    string name ()
    {
        return "Veg Burger";
    }

    int price ()
    {
        return 12;
    }

};

class ChikenBurger : public Burger
{
        public :

    string name ()
    {
        return "Chicken Burger";
    }

    int price ()
    {
        return 120;
    }

};

class Pepsi : public ColdDrinks
{
 public :

    string name ()
    {
        return "Pepsi";
    }

    int price ()
    {
        return 40;
    }

};

class Coke : public ColdDrinks
{
     public :

    string name ()
    {
        return "Coke";
    }

    int price ()
    {
        return 55;
    }

};

class Meal
{
    public :

    list <Item*> items;

    int cost = 0 ;

    void addItems ( Item* item )
    {
        items.push_back ( item );
    }

    int getCost (  )
    {
        for ( Item *item : items )
        {
            cost += item->price();
        }
        return cost;
    }

    void showItem  ()
    {
        for ( Item *item : items )
        {
            cout << " name " << item->name () << endl;
            cout << " packing type : " << item->packing()->pack () << endl;
            cout << " price will be : " << item->price() << endl;

        }
    }
};

class MealBuilder 
{
    public:
    Meal* prepeareNonVegMeal ()
    {
        cout << "preparing non veg meal " << endl;
        Meal* meal = new Meal;

        meal->addItems ( new ChikenBurger);
        meal->addItems ( new Coke);
        return meal;
    }

    Meal* prepareVegMeal ()
    {
        cout << "preparing Veg meal " << endl;
        Meal* meal = new Meal;
        meal->addItems ( new VegBurger );
        meal->addItems ( new Pepsi );
        return meal;
    }
};


int main()
{

    MealBuilder* mealBuilder = new MealBuilder;
    Meal* nonVegmeal = mealBuilder->prepeareNonVegMeal();

    nonVegmeal->showItem();

    cout << "total cost of veg meal " << nonVegmeal->getCost() << endl;

    Meal* vegMeal = mealBuilder->prepareVegMeal();

    vegMeal->showItem();

    cout << "total cost of nonveg meal " << vegMeal->getCost() << endl;

    return 0;

}













