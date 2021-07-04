/*

Command pattern comes under behavioural pattern . A request is wrapped
under a Object as Command and pass this command to invoker object .
Invoker object finds appropriate object to process that command 

*/

#include <iostream>
#include <list>
using namespace std;


class Order 
{

  public:

  virtual void execute () = 0;

};

class Stock
{

  private :
  string name = "" ;

  int quantity = 0 ;

  public :
  Stock ( string name , int quantity )
  {
      this->name = name ;
      this->quantity = quantity;
  }

  void buy ()
  {
      cout << " quantity of " << quantity << " for stock " << this->name << " has been bought " << endl;
  }

  void sold ()
  {
      cout << " quantity of " << quantity << " for stock  " << this->name << "  has been sold " << endl;

  }

  

};

class BuyStock : public Order
{
    Stock* mStock = nullptr;

    public:

    BuyStock ( Stock* vStock  )
    {
        this->mStock = vStock;
    }

    void execute ()
    {
        this->mStock->buy();
    }

};

class SellStock : public Order
{
    Stock* mStock = nullptr;

    public:

    SellStock ( Stock* vStock  )
    {
        this->mStock = vStock;
    }

    void execute ()
    {
        this->mStock->sold();
    }

};

class Broker 
{
    private:

    list <Order*>  mOrderList;

    list <Order*> & getOrderList()
    {
        return mOrderList;
    }

    void clearList ()
    {
        mOrderList = {};
    }

    void AddOrder ( Order* mOrder )
    {
        getOrderList().push_back ( mOrder );
    }

   public:

   void takeOrder ( Order* mOrder )
   {
      AddOrder ( mOrder);
   }

   void placeOrder ()
   {
       for ( Order* mOrder :  getOrderList () )
       {
           mOrder->execute ();
       }

       clearList ();
   }



};


int main ()
{
    Stock* mStock_a = new Stock ( "Ashock Leyland ", 10 );

    Stock* mStock_b = new Stock ( " Ambuja Cement ", 100 );

    BuyStock* mBuyStock = new BuyStock ( mStock_a );

    BuyStock* mBuyStock_1 = new BuyStock ( mStock_b );

    Broker* mBroker = new Broker;

    mBroker->takeOrder ( mBuyStock );
    mBroker->takeOrder ( mBuyStock_1 );

    mBroker->placeOrder ();

    SellStock* mSellStock_a = new SellStock ( mStock_a );

    SellStock* mSellStock_b = new SellStock ( mStock_b );

    mBroker->takeOrder ( mSellStock_a );
    mBroker->takeOrder ( mSellStock_b );

    mBroker->placeOrder ();

    
}
