#include <stdio.h>
#include <condition_variable>
#include <thread>
#include <iostream>

using namespace std;

condition_variable cond_var;
mutex m;

bool oddFlag = true;
bool evenFlag = false;
int count_ = 1;
void printOdd();
void printEven();


void printOdd()
{
    for (; count_ < 100;)
    {
        unique_lock<mutex> mu(m);
        cond_var.wait(mu, []
                      { return oddFlag; });
        oddFlag = false;
        evenFlag = true;
        cout << "  " << count_ << endl;
        count_++;
        cond_var.notify_all();
    }
}

void printEven()
{
    for (; count_ < 100 ;)
    {
        unique_lock <mutex> mu (m);
        cond_var.wait ( mu, [] {return evenFlag;});
        cout << " " << count_ << endl;
        count_++;
        oddFlag = true;
        evenFlag = false;
        cond_var.notify_all();
    }
}

int main()
{
    thread t1(printOdd);
    thread t2(printEven);

    t1.join();
    t2.join();
}
