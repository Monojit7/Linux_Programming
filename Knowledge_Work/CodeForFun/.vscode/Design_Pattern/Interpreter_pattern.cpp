/*

Interpreter pattern is a behavioural pattern .

Interpreter pattern provides way to interpret language expression.

There will be expression interpret class. TerminalExpression is a 

Concrete class of it and it actually interprets the main Request.

AndExpression and OrExpression provides combination of expressions .

*/

#include <iostream>

using namespace std;

class Expression
{

public:

virtual bool interpret ( string context ) = 0 ;

};

class TerminalExpression : public Expression
{
  string data = {};  
  public:
  TerminalExpression ( string data )
  {
      this->data = data;
  }

  bool interpret ( string context )
  {
      bool status = false;
      if ( context.find ( data ) != string::npos )
      {
          status = true;
      }

      return status;
  }
 
};

class AndExpression : public Expression
{
    private:
            Expression* expr1;
            Expression* expr2;

    public:
           AndExpression ( Expression* expr1 , Expression* expr2 )
           {
               this->expr1 = expr1;
               this->expr2 = expr2;
           } 

           bool interpret ( string context )
           {
               return expr1->interpret ( context ) && expr2->interpret ( context );
           } 

};

class OrExpression : public Expression
{

        private:
            Expression* expr1;
            Expression* expr2;

    public:
           OrExpression ( Expression* expr1 , Expression* expr2 )
           {
               this->expr1 = expr1;
               this->expr2 = expr2;
           } 

           bool interpret ( string context )
           {
               return expr1->interpret ( context ) || expr2->interpret ( context );
           } 

};

class InterpreterPatternDemo
{

   public :

   Expression* getMaleExpression ()
   {
       TerminalExpression* robert = new TerminalExpression ("Robert");
       TerminalExpression* amatya = new TerminalExpression ("Amatya");

       OrExpression* mOrExpression = new OrExpression ( robert, amatya );

       return mOrExpression;
       
   }

      Expression* getMarriedExpression ()
   {
       TerminalExpression* roshni = new TerminalExpression ("Roshni");
       TerminalExpression* married = new TerminalExpression ("Married");

       AndExpression* mAndExpression = new AndExpression ( roshni, married );

       return mAndExpression;
       
   }



};

int main ()
{
    InterpreterPatternDemo* mInterpreterPatternDemo = new InterpreterPatternDemo;
    Expression* exp1 = mInterpreterPatternDemo->getMaleExpression();
    Expression* exp2 = mInterpreterPatternDemo->getMarriedExpression();

    cout << "Robert is male ? " << exp1->interpret ("Robert") << endl;
    cout << "Roshni is married woman ?" <<  exp2->interpret("Roshni Married")<< endl;
}