#include "../include/auth_window.h"

AuthWindow::AuthWindow(QWidget *parent)
        : QWidget(parent), authSystem(new AuthSystem(this))
{
    setupUI();
}

void AuthWindow::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *loginLabel = new QLabel("Логин:");
    loginEdit = new QLineEdit();

    QLabel *passwordLabel = new QLabel("Пароль:");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *loginButton = new QPushButton("Войти");
    statusLabel = new QLabel();

    layout->addWidget(loginLabel);
    layout->addWidget(loginEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);

    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::handleLogin);

    setLayout(layout);
    setWindowTitle("Авторизация");
    resize(300, 180);
}

void AuthWindow::handleLogin() {
    QString login = loginEdit->text();
    QString password = passwordEdit->text();

    User *user = authSystem->login(login, password);
    if (user) {
        statusLabel->setText("Успешный вход!");

        Interface *ui = user->role() == "admin" ?
                        static_cast<Interface*>(new AdminInterface(user)) :
                        static_cast<Interface*>(new UserInterface(user));

        ui->display();


        ui->display();

        close();
    } else {
        statusLabel->setText("Неверный логин или пароль");
    }
}
