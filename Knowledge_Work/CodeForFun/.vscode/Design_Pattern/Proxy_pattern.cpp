/*

Proxy pattern is a structural pattern where a class represent functionality
of another class

*/


#include <iostream>
#include <list>

using namespace std;


class Image 
{
    public :

    virtual ~Image ()
    {

    }

    virtual void display() = 0;

};

class RealImage  : public Image
{
    public :

    string FileName = {};
    RealImage ( string fileName )
    {
       this->FileName = fileName;
       loadfromDisk();
    }

    void loadfromDisk ()
    {
        cout << "Loading : " << this->FileName << endl;
    }

    void display ()
    {
        cout << "Displaying : " << this->FileName << endl;
    }
    
};

class ProxyImage : public Image 
{
    public:

    Image* mRealImage = nullptr;

    ProxyImage ( string fileName )
    {
        if ( mRealImage ==  nullptr )
        {
            mRealImage = new RealImage ( fileName );
        }
    }

    void display ()
    {
        mRealImage->display();
    }
};


int main()
{
    Image* mImage = new ProxyImage ("xyz.jpeg");

    mImage->display();

    mImage->display();


}





