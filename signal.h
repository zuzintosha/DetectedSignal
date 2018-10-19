#ifndef SIGNAL_H
#define SIGNAL_H

#include <QVector>
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

    QVector<double> x;
    QVector<double> y;

private:
    int size;
};

#endif // SIGNAL_H
