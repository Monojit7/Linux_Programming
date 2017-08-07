/*

tuple is a variadic class template which accepts unlimited set of different type of values
defined when instantiating the tuple

*/

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;




int main()
{

vector<tuple<int , string>> mVtuple;

mVtuple.push_back(make_tuple(4, "monojit"));
mVtuple.push_back(make_tuple(5, "Kashinath"));
mVtuple.push_back(make_tuple(6, "chatterjee"));

for (auto& i : mVtuple )
{

cout << "int: " << get<0> (i) << " string:  " << get<1> (i) << endl;

}

return 0;
}
