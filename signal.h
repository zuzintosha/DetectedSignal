#ifndef SIGNAL_H
#define SIGNAL_H

#include <vector>
#include <iostream>
using namespace std;

class Signal
{
public:
    Signal();
    Signal(int size);
    int getSize();
    void setSize(int size);
private:
    vector<double> x;
    vector<double> y;
    int size;
};

#endif // SIGNAL_H
