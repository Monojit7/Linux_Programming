/*

createButon is a factory method it solves the creation of object in runtime
and returns a object of desired type.

Problem : Two platform has the same functionality . We need to create  platform indepent code when
          when functionalities are same but functionality is same. 

Solution : Functionality or action will be the base class. Each Platform will be having different 
           Implementation . CreateButton () will provide the hook of the button in runtime based
           on the object type. Calling doAct () will call respective object doAct () at runtime.

*/

#include <iostream>
#include <string>

using namespace std;

class Button {
public:
	virtual void doAct() = 0;
};
 
class LinuxButton: public Button {
public:
	void doAct() {
		cout << "Linux button \n";
	}
};
 
class WindowsButton: public Button  {
public:
	void doAct() {
	          cout << "Windows button \n";
	}
};
 
class GUIFactory {
public:
	virtual Button *createButton(string type) = 0;
};

class Factory: public GUIFactory {
public:
	Button *createButton(string type) {
		if( type == "Windows" ) {
			return new WindowsButton;
		}
		else if( type == "OSX" ) {
			return new LinuxButton;
		}
	}

};


int main()
{
	GUIFactory* guiFactory;
	Button *btn;

	guiFactory = new Factory;

	btn = guiFactory->createButton("OSX");
	btn -> doAct();
	btn = guiFactory->createButton("Windows");
	btn -> doAct();

	return 0;
}
