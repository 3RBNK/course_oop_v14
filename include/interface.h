#ifndef COURSE_INTERFACE_H
#define COURSE_INTERFACE_H

#include <QWidget>

class Interface : public QWidget
{
Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~Interface() {}

    virtual void display() = 0;
};

#endif //COURSE_INTERFACE_H
