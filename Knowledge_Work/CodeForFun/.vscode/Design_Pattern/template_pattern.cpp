/*

template pattern provides defined way to execute its method. 
An abstract class will be having tempate method which will be 
defined methods which sets the execution of other abstract method

This is part of behavioural pattern

*/


// Abstract class

#include <iostream>
using namespace std;

class Game 
{
    public:

    virtual void init () = 0;
    virtual void startPlay () = 0;
    virtual void endPlay () = 0;

   // play is the template method because this method defines
   // the execution of other three methods
    void play ()
    {
         init();
         startPlay ();
         endPlay ();
    }
};

class Cricket : public Game
{

  public:

  void init ()
  {
      cout << "cricket game initialized "<< endl;
  }

  void startPlay ()
  {
      cout << " started playing cricket game " << endl;
  }

  void endPlay ()
  {
      cout << " playing ends of cricket game " << endl;
  }
 
};

class FootBall : public Game
{

  public:

  void init ()
  {
      cout << "FootBall game initialized "<< endl;
  }

  void startPlay ()
  {
      cout << " started playing FootBall game " << endl;
  }

  void endPlay ()
  {
      cout << " playing ends of FootBall game " << endl;
  }
 
};

int main ()
{
    Game* newGame = new Cricket;
    newGame->play ();

    newGame = new FootBall;
    newGame->play();
    return 0;
}

