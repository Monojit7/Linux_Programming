/*

Prototype allows us to hide the complexity of making new instances from the client. In this sample, 
we used prototype manager to set the registry of concrete prototypes.Basically the intention 
is to hide the complexity of the type of the object and use the same prototype object for other 
similar kind of operations.

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
const int N = 4;

// Prototype
class cDevice 
{
public:
   virtual cDevice* clone() const = 0;
   virtual void store() const = 0;
   virtual ~cDevice() { }
};


// Concrete prototypes : xmlDoc, plainDoc, spreadsheetDoc

class cAppleIphone : public cDevice 
{
public:
   cDevice*   clone() const { return new cAppleIphone; }
   void store() const { cout << "Apple Iphone\n"; }
};

class cSamsungGalaxy : public cDevice
{
public:
   cDevice* clone() const { return new cSamsungGalaxy; }
   void store() const { cout << "Samsung Galaxy\n"; }
};

class cWindowsPhone : public cDevice
{
public:
  cDevice* clone() const { return new cWindowsPhone; }
   void store() const { cout << "Windows Phone\n"; }
};

// makeDevice() calls Concrete Portotype's clone() method 
// inherited from Prototype
class cDeviceManager {
public:
   static cDevice* createDevice( int choice );
   ~cDeviceManager(){}

private:
   static cDevice* mDeviceTypes_[N];
};

cDevice* cDeviceManager::mDeviceTypes_[] = 
{
   0, new cSamsungGalaxy, new cAppleIphone, new cWindowsPhone
};

cDevice* cDeviceManager::createDevice( int choice ) 
{
   return mDeviceTypes_[choice]->clone();
}

// for_each op ()
struct sDestruct
{
    void operator()(cDevice *a) const {
     cout << "Object getting deleted " << endl;
	delete a; 
    }
};

// Client
int main() {
   vector<cDevice*> devices(N);
   int choice;
   cout << "quit(0), samsung Galaxy (1), AppleIphone (2), Windows Phone (3): \n";
   while (true) {
	  cout << "Type in your choice (0-3)\n";
	  cin >> choice;
      if (choice <= 0 || choice >= N)
         break;
      devices [choice] = cDeviceManager::createDevice ( choice );
   }

   for (int i = 1; i < devices.size(); ++i) 
	   if(devices [i]) devices [i]->store();

   struct sDestruct d;
    // this calls Destruct::operator()
    for_each(devices.begin(), devices.end(), d);

   return 0;
}
