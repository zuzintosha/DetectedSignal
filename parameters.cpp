#include "parameters.h"

Parameters::Parameters()
{
    this->frequency = -1;
    this->phi_0 = 0;
    this->samplingFrequency = -1;
}

Parameters::Parameters(double frequency, double phi_0, double samplingFrequency, double timeOfSignal){
    this->frequency = frequency;
    if(frequency <= 0) this->frequency = 1;

    this->phi_0 = phi_0;

    this->samplingFrequency = samplingFrequency;
    if(samplingFrequency <= 0) this->samplingFrequency = 100;

    this->samplingPeriod = 1. / this->samplingFrequency;

    this->timeOfSignal = timeOfSignal;
    this->sizeOfSignal = timeOfSignal / this->samplingPeriod;
}
