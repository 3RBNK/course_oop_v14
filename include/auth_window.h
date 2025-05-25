#ifndef COURSE_AUTH_WINDOW_H
#define COURSE_AUTH_WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "auth_system.h"
#include "user_interface.h"
#include "admin_interface.h"

class AuthWindow : public QWidget {
Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);

private slots:
    void handleLogin();

private:
    QLineEdit *login_edit;
    QLineEdit *password_edit;
    QLabel *status_label;
    AuthSystem *auth_system;

    void setupUI();
};

#endif //COURSE_AUTH_WINDOW_H
