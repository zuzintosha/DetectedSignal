#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "random"
#include <time.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "QtDebug"
#include <fstream>
#include "signal.h"
#include "parameters.h"
#include "testclass.h"
using namespace std;

#define PI 3.1415

Signal generateDefaultSignal(const Parameters* parameters){
    Signal signal(parameters->sizeOfSignal);

    double arg = 0;
    for(int i = 0; i < parameters->sizeOfSignal; i++){
        arg = 2*PI*parameters->frequency*i*parameters->samplingPeriod;
        if(arg > 2*PI) arg -= 2*PI;
        signal.y[i] = sin(arg + parameters->phi_0);
        signal.x[i] = i*parameters->samplingPeriod;
    }

    return signal;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qInstallMessageHandler(0);

    int size = ui->lineEditNumberOfBits->text().toDouble();
    qDebug() << "Begin initialization: " << size;

    signal = new Signal();
    parameters = new Parameters(
                    ui->lineEditFrequency->text().toDouble(),
                    0,
                    ui->lineEditSamplingFrequency->text().toDouble(),
                    ui->lineEditPeriod->text().toDouble()
                );

    //тут создам сигнал
    //signal = new Signal();
    //signal->setSize(size);
    //qDebug() << "Signal size: " << signal->getSize();


    srand(time(0));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete signal;
    delete parameters;
}

void MainWindow::on_pushButton_clicked()
{
        double frequency = ui->lineEditFrequency->text().toDouble();//25000;
        double phi_0 = 0;
        double samplingFrequency = ui->lineEditSamplingFrequency->text().toDouble();//250000;
        double T = ui->lineEditPeriod->text().toDouble();//0.0001;
        double N = samplingFrequency + 1;
        double samplingPeriod = T / samplingFrequency;
        QVector<double> x(N),y(N);

        Signal signalTest(N);

        qDebug() << "Frequency: " << parameters->frequency;
        qDebug() << "Phi_0: " << parameters->phi_0;
        qDebug() << "Sampling frequency: " << parameters->samplingFrequency;

        ui->signalGraphic->xAxis->setRange(0,T);
        ui->signalGraphic->yAxis->setRange(-1,1);

        double arg = 0;
        for(int i = 0; i < N; i++){
            arg = 2*PI*frequency*i*samplingPeriod;
            if(arg > 2*PI) arg -= 2*PI;
            signalTest.y[i] = y[i] = sin(arg + phi_0);
            signalTest.x[i] = x[i] = i*samplingPeriod;
        }

        Signal defaultSignal = generateDefaultSignal(parameters);

        ui->signalGraphic->clearGraphs();//Если нужно, но очищаем все графики
        //Добавляем один график в signalGraphic
        ui->signalGraphic->addGraph();
        //Говорим, что отрисовать нужно график по нашим двум массивам x и y
        //ui->signalGraphic->graph(0)->setData(x, y);
        //ui->signalGraphic->graph(0)->setData(signalTest.x, signalTest.y);
        ui->signalGraphic->graph(0)->setData(defaultSignal.x,defaultSignal.y);

        ui->signalGraphic->replot();
}

double getRandomNumber(){
    return (double)rand() / (RAND_MAX) > 0.5 ? 1 : 0;
}

void MainWindow::on_pushButton_2_clicked()
{
    double numberOfBits = ui->lineEditNumberOfBits->text().toDouble();
    QVector<double> x(numberOfBits),y(numberOfBits);
    QVector<int> bits(numberOfBits);
    QVector<int> bitsForMinFrequencyModulation(numberOfBits);

   // ui->signalGraphic->xAxis->setRange(0,numberOfBits);
    //ui->signalGraphic->yAxis->setRange(0,1);

    for(int i = 0; i < numberOfBits; i++){
        y[i] = getRandomNumber();
        x[i] = i;

        bits[i] = y[i];
        bitsForMinFrequencyModulation[i] = getRandomNumber();
        //qDebug() << i << " bit for freq modulation: " << bitsForMinFrequencyModulation[i];
    }
    //qDebug() << endl;

    double pi = 3.14;
    double frequency = ui->lineEditFrequency->text().toDouble();//25000;
    double bitRate = ui->lineEditBitRate->text().toDouble();
    double modulationPeriod = 1. / bitRate;
    double allBitTime = modulationPeriod * numberOfBits;
    double phi_0 = 0;
    double samplingFrequency = ui->lineEditSamplingFrequency->text().toDouble();//250000;
    double T = ui->lineEditPeriod->text().toDouble();//0.0001;
    int N = samplingFrequency + 1;
    double N2 = 2*N;
    double d = 0;
    while(d == 0){
        d = rand() % N;
    }
    double L = N2 - d - N;

    qDebug() << "N: " << N;
    qDebug() << "N2: " << N2;
    qDebug() << "d: " << d;
    qDebug() << "L: " << L;

    double samplingPeriod = allBitTime / samplingFrequency;

    double realSamplingPeriod = 1. / samplingFrequency;
    double countOfN = allBitTime / realSamplingPeriod;

    qDebug() << "Sampling period: " << samplingPeriod;
    qDebug() << "Real sampling period: " << realSamplingPeriod;
    qDebug() << "Count N: " << countOfN;


    QVector<double> signalX(N),signalY(N);
    QVector<double> signalX2(N2),signalY2(N2);

    qDebug() << "Also printed in the console!";

    int bitIndex = 0;
    int b = 1;
    double m = 0.5;
    double arg = 0;
    int piIndex = 0;

    double minFrequencyModulationArg = 0;
    double frequencyOfDeviation = bitRate / 2;
    double deviationArg = 0;
    int devB = 0;
    double energySignal = 0;

    for(int i = 0; i < N; i++){
        signalX[i] = i*samplingPeriod;
        bitIndex = floor(signalX[i] / modulationPeriod);

        if(bitIndex < numberOfBits) {

            //b = bits[bitIndex] == 1 ? 1 : -1;
            b = bits[bitIndex] == 1 ? 0 : 1;
            devB = bitsForMinFrequencyModulation[bitIndex] == 1 ? 1 : -1;

            arg = 2*pi*frequency*signalX[i];
            if(arg > 2*pi){
                piIndex = arg / (2*pi);
                //qDebug() << "Arg until: " << arg;
                //qDebug() << "Pi index: " << piIndex;
                //qDebug() << "2Pi: " << 2*pi*piIndex;
                arg -= 2*pi*piIndex;
                //qDebug() << "Arg after: " << arg << endl;
            }

           // qDebug() << "Arg: " << arg;
            //qDebug() << "pi*b: " << pi*b;
            //qDebug() << "Full arg+: " << arg + (pi*b);
            //qDebug() << "Full arg-: " << arg - (pi*b);
           // qDebug() << "Y+pi: " << cos(arg + (pi*b));
            //qDebug() << "Y-pi: " << cos(arg - (pi*b)) << endl;

            //signalY[i] = bits[bitIndex]*cos(2*pi*frequency*signalX[i] + phi_0);//Битовый сигнал
            signalY[i] = (1 + m*b)*cos(arg + phi_0);//AM
            //signalY[i] = cos(arg + (pi*b));//BPSK. Это работает, но совсем незаметно.
                                                               //pi*b очень мало изменяет аргумент

            //if(i < 500) qDebug() << "Deviation arg: " << deviationArg;
            deviationArg += samplingPeriod * devB;
            minFrequencyModulationArg = frequency*signalX[i] + frequencyOfDeviation * deviationArg;
            //signalY[i] = cos(2*pi*minFrequencyModulationArg);//это минимально частотная манипуляция
            //signalY[i] = cos(2*pi*(frequency + frequencyOfDeviation*devB)*signalX[i]);

            energySignal += signalY[i]*signalY[i];
        }else{
            signalY[i] = sin(2*pi*frequency*signalX[i] + phi_0);
        }
    }

    //тут я добавлю шума
    //шум надо добавлять уже после того как сгенерирован сигнал
    QVector<double> noise(N);
    double energyNoise = 0;
    double averageNoise = 0;
    int countAverageNoise = 20;
    double SNR = ui->lineEditSNR->text().toDouble();

    for(int i = 0; i < N; i++){
        averageNoise = 0;
        for(int j = 0; j < countAverageNoise; j++){
            averageNoise += (2*((double)rand()/(double)RAND_MAX)) - 1;
        }
        averageNoise /= countAverageNoise;

        //if(i < 500) qDebug() << averageNoise;

        noise[i] = averageNoise;
        energyNoise += noise[i]*noise[i];
    }

    double energyNoiseSNR = energySignal / (pow(10,SNR / 10));
    double noiseCoeff = sqrt(energyNoiseSNR / energyNoise);
    //double noiseCoeff2 = energySignal / energyNoise / pow(10, SNR / 10);

    qDebug() << "\nEnergy signal: " << energySignal;
    qDebug() << "Energy noise: " << energyNoise;
    qDebug() << "Energy SNR noise: " << energyNoiseSNR;
    qDebug() << "Noise coeff: " << noiseCoeff;
    //qDebug() << "Noise coeff2: " << noiseCoeff2;


    //добавляют шум к сигналу
    for(int i = 0; i < N; i++){
        //signalY[i] = noise[i];
        signalY[i] = signalY[i] + noise[i]*noiseCoeff;
    }


    //Далее буду генерировать второй сигнал
    for(int i = 0; i < d; i++){
        signalX2[i] = i*samplingPeriod;
        signalY2[i] = cos(2*pi*frequency*signalX2[i] + phi_0);
    }
    for(int i = 0; i < N; i++){
        signalX2[i + d] = i*samplingPeriod;
        signalY2[i + d] = signalY[i];//AM
    }
    for(int i = N + d; i < N2; i++){
        signalX2[i] = i*samplingPeriod;
        signalY2[i] = cos(2*pi*frequency*signalX2[i] + phi_0);
    }



    //тут буду считать функцию корреляции
    int tau = N2 - N;
    double maxValue = 0;
    double maxValueIndex = 0;
    ofstream file1;
    file1.open("CrossCorrelationFunction.txt");
    QVector<double> crossCorrelationFunctionY(tau);
    QVector<double> crossCorrelationFunctionX(tau);
//    for(int t = 0; t < tau; t++){
//        //if(tau % 100 == 0) qDebug() << "Tau: " << t;
//        //qDebug() << "Tau: " << t;
//        crossCorrelationFunctionX[t] = t;
//        crossCorrelationFunctionY[t] = 0;
//        for(int j = 0; j < N; j++){
//            //if(j % 10000 == 0 && tau % 100 == 0) qDebug() << "   J: " << j;
//            crossCorrelationFunctionY[t] += signalY[j]*signalY2[j + t];
//        }
//        //file1 << crossCorrelationFunctionY[t] << endl;
//        if(crossCorrelationFunctionY[t] > maxValue){
//            maxValue = crossCorrelationFunctionY[t];
//            maxValueIndex = t;
//        }
//    }

//    for(int j = 0; j < tau; j++){
//        crossCorrelationFunctionY[j] /= maxValue;
//    }
    //file1.close();
    qDebug() << "d: " << d;
    qDebug() << "index: " << maxValueIndex;



    ui->signalGraphic->xAxis->setRange(0,T);
    ui->signalGraphic->yAxis->setRange(-2,2);


    //здесь задам границы для отображения взаимной корреляционной функции
    //ui->signalGraphic->xAxis->setRange(0,tau);
    //ui->signalGraphic->yAxis->setRange(0,1);

    //cout << "Qt version: " << QTCORE_VERSION;

    ui->signalGraphic->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в signalGraphic
    ui->signalGraphic->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->signalGraphic->graph(0)->setData(signalX, signalY);
    //ui->signalGraphic->graph(0)->setData(crossCorrelationFunctionX, crossCorrelationFunctionY);
    //Перерисовываем график
    ui->signalGraphic->replot();
}

void MainWindow::on_pushButton_3_clicked()
{
    double numberOfBits = ui->lineEditNumberOfBits->text().toDouble();
    QVector<double> x(numberOfBits),y(numberOfBits);
    QVector<int> bits(numberOfBits);
    QVector<int> bitsForMinFrequencyModulation(numberOfBits);

    //ui->signalGraphic->xAxis->setRange(0,numberOfBits);
    //ui->signalGraphic->yAxis->setRange(0,1);

    for(int i = 0; i < numberOfBits; i++){
        y[i] = getRandomNumber();
        x[i] = i;

        bits[i] = y[i];
        bitsForMinFrequencyModulation[i] = getRandomNumber();
        //qDebug() << i << " bit for freq modulation: " << bitsForMinFrequencyModulation[i];
    }
    //qDebug() << endl;

    double pi = 3.14;
    double frequency = ui->lineEditFrequency->text().toDouble();//25000;
    double bitRate = ui->lineEditBitRate->text().toDouble();
    double modulationPeriod = 1. / bitRate;
    double allBitTime = modulationPeriod * numberOfBits;
    double phi_0 = 0;
    double samplingFrequency = ui->lineEditSamplingFrequency->text().toDouble();//250000;
    double T = ui->lineEditPeriod->text().toDouble();//0.0001;
    int N = samplingFrequency + 1;
    double N2 = 2*N;
    double d = 0;
    while(d == 0){
        d = rand() % N;
    }
    double L = N2 - d - N;

    qDebug() << "N: " << N;
    qDebug() << "N2: " << N2;
    qDebug() << "d: " << d;
    qDebug() << "L: " << L;

    double samplingPeriod = allBitTime / samplingFrequency;

    double realSamplingPeriod = 1. / samplingFrequency;

    int countOfN = ceil(allBitTime / realSamplingPeriod);
    double countOfN2 = 2*countOfN;
    double dTest = 0;
    while(dTest == 0){
        dTest = rand() % countOfN;
    }
    double LTest = countOfN2 - dTest - countOfN;

    qDebug() << "Sampling period: " << samplingPeriod;
    qDebug() << "Real sampling period: " << realSamplingPeriod;
    qDebug() << "Count N: " << countOfN;

    //signal->setSize(countOfN);
    qDebug() << "Signal size: " << signal->getSize();
    //testClass->setSize(countOfN);
    //qDebug() << "Signal size: " << testClass->getSize();


    //QVector<double> signalX(N),signalY(N);
    QVector<double> signalXTest(countOfN), signalYTest(countOfN);
    QVector<double> signalX2Test(countOfN2), signalY2Test(countOfN2);

    qInstallMessageHandler(0);

    int bitIndex = 0;
    int b = 1;
    double m = 0.5;
    double arg = 0;
    int piIndex = 0;

    //double energySignal = 0;
    double energySignalTest = 0;

//    for(int i = 0; i < N; i++){
//        signalX[i] = i*samplingPeriod;
//        bitIndex = floor(signalX[i] / modulationPeriod);

//        if(bitIndex < numberOfBits) {
//            b = bits[bitIndex] == 1 ? 0 : 1;

//            arg = 2*pi*frequency*signalX[i];
//            if(arg > 2*pi){
//                piIndex = arg / (2*pi);
//                arg -= 2*pi*piIndex;
//            }

//            //signalY[i] = bits[bitIndex]*cos(2*pi*frequency*signalX[i] + phi_0);//Битовый сигнал
//            signalY[i] = (1 + m*b)*sin(arg + phi_0);//AM

//            energySignal += signalY[i]*signalY[i];
//        }else{
//            signalY[i] = sin(2*pi*frequency*signalX[i] + phi_0);
//        }
//    }

    for(int i = 0; i < countOfN; i++){
        signalXTest[i] = i*realSamplingPeriod;
        bitIndex = floor(signalXTest[i] / modulationPeriod);

        if(bitIndex < numberOfBits) {
            b = bits[bitIndex] == 1 ? 0 : 1;

            arg = 2*pi*frequency*signalXTest[i];
            if(arg > 2*pi){
                piIndex = arg / (2*pi);
                arg -= 2*pi*piIndex;
            }

            //signalY[i] = bits[bitIndex]*cos(2*pi*frequency*signalX[i] + phi_0);//Битовый сигнал
            signalYTest[i] = (1 + m*b)*sin(arg + phi_0);//AM

            energySignalTest += signalYTest[i]*signalYTest[i];
        }else{
            signalYTest[i] = sin(2*pi*frequency*signalXTest[i] + phi_0);
        }
    }

    //тут я добавлю шума
    //шум надо добавлять уже после того как сгенерирован сигнал
    QVector<double> noise(countOfN);
    double energyNoise = 0;
    double averageNoise = 0;
    int countAverageNoise = 20;
    double SNR = ui->lineEditSNR->text().toDouble();

    for(int i = 0; i < countOfN; i++){
        averageNoise = 0;
        for(int j = 0; j < countAverageNoise; j++){
            averageNoise += (2*((double)rand()/(double)RAND_MAX)) - 1;
        }
        averageNoise /= countAverageNoise;

        noise[i] = averageNoise;
        energyNoise += noise[i]*noise[i];
    }

    double energyNoiseSNR = energySignalTest / (pow(10,SNR / 10));
    double noiseCoeff = sqrt(energyNoiseSNR / energyNoise);

    //добавляют шум к сигналу
    for(int i = 0; i < countOfN; i++){
        signalYTest[i] = signalYTest[i] + noise[i]*noiseCoeff;
    }

    //Далее буду генерировать второй сигнал
    for(int i = 0; i < dTest; i++){
        signalX2Test[i] = i*realSamplingPeriod;
        signalY2Test[i] = cos(2*pi*frequency*signalX2Test[i] + phi_0);
    }
    for(int i = 0; i < countOfN; i++){
        signalX2Test[i + dTest] = i*realSamplingPeriod;
        signalY2Test[i + dTest] = signalYTest[i];//AM
    }
    for(int i = countOfN + dTest; i < countOfN2; i++){
        signalX2Test[i] = i*realSamplingPeriod;
        signalY2Test[i] = cos(2*pi*frequency*signalX2Test[i] + phi_0);
    }


    //тут буду считать функцию корреляции
    int tau = countOfN2 - countOfN;
    double maxValue = 0;
    double maxValueIndex = 0;

    QVector<double> crossCorrelationFunctionY(tau);
    QVector<double> crossCorrelationFunctionX(tau);
    for(int t = 0; t < tau; t++){
        crossCorrelationFunctionX[t] = t;
        crossCorrelationFunctionY[t] = 0;
        for(int j = 0; j < countOfN; j++){
            crossCorrelationFunctionY[t] += signalYTest[j]*signalY2Test[j + t];
        }
        if(crossCorrelationFunctionY[t] > maxValue){
            maxValue = crossCorrelationFunctionY[t];
            maxValueIndex = t;
        }
    }

    for(int j = 0; j < tau; j++){
        crossCorrelationFunctionY[j] /= maxValue;
    }


    //ui->signalGraphic->xAxis->setRange(0,T);
    ui->signalGraphic->xAxis->setRange(0,allBitTime);
    //ui->signalGraphic->xAxis->setRange(0,tau);

   ui->signalGraphic->yAxis->setRange(-2,2);
    //ui->signalGraphic->yAxis->setRange(0,1);

    ui->signalGraphic->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в signalGraphic
    ui->signalGraphic->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    //ui->signalGraphic->graph(0)->setData(signalX, signalY);
    ui->signalGraphic->graph(0)->setData(signalXTest, signalYTest);
    //ui->signalGraphic->graph(0)->setData(crossCorrelationFunctionX, crossCorrelationFunctionY);
    //Перерисовываем график
    ui->signalGraphic->replot();
}
