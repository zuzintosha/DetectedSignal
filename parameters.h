#ifndef PARAMETERS_H
#define PARAMETERS_H


class Parameters
{
public:
    Parameters();
    Parameters(double frequency, double phi_0, double samplingFrequency,double timeOfSignal);

    double frequency;
    double phi_0;
    double samplingFrequency;
    double samplingPeriod;
    double timeOfSignal;
    int sizeOfSignal;

};

#endif // PARAMETERS_H
