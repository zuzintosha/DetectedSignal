#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
    this->size = -11;
}

void TestClass::setSize(int size){
    this->size = size;
}

int TestClass::getSize(){
    return this->size;
}
