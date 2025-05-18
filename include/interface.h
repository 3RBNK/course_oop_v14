#ifndef CODE_V1_INTERFACE_H
#define CODE_V1_INTERFACE_H

#include <QWidget>

class Interface : public QWidget
{
Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~Interface() {}

    virtual void display() = 0;
};

#endif //CODE_V1_INTERFACE_H
