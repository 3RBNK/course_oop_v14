#ifndef CODE_V1_AUTH_WINDOW_H
#define CODE_V1_AUTH_WINDOW_H

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
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QLabel *statusLabel;
    AuthSystem *authSystem;

    void setupUI();
};

#endif //CODE_V1_AUTH_WINDOW_H
