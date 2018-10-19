#include "signal.h"

Signal::Signal() {
    this->size = -123;
}

Signal::Signal(int size) {
    this->size = size;
    if(size < 0) this->size = 0;
    this->x.resize(size);
    this->y.resize(size);
}

int Signal::getSize() {
    return this->size;
}

void Signal::setSize(int size) {
    this->size = size;
}
