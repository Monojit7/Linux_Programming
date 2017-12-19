#include <iostream>
#include <thread>

class MyFunctor
{
public:
    void operator()() {
        std::cout << "functor\n";
    }
};

int main()
{
    MyFunctor fnctor, functor1;
    std::thread t(fnctor), t1(functor1), t2 ( ( MyFunctor()  )  );
    std::cout << "main thread\n";
    t.join();
    t1.join ();
    t2.join ();
    
    return 0;
}

