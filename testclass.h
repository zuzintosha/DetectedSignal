#ifndef TESTCLASS_H
#define TESTCLASS_H
#include <QObject>

class TestClass : public QObject
{
    Q_OBJECT
public:
    explicit TestClass(QObject *parent = nullptr);

    void setSize(int size);
    int getSize();

signals:

public slots:

private:
    int size;
};

#endif // TESTCLASS_H
