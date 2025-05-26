#include "../include/auth_window.h"

AuthWindow::AuthWindow(QWidget *parent)
        : QWidget(parent), auth_system(new AuthSystem(this)) {
    setupUI();
}

void AuthWindow::setupUI() {
    QVBoxLayout * layout = new QVBoxLayout(this);

    QLabel * loginLabel = new QLabel("Логин:");
    login_edit = new QLineEdit();

    QLabel * passwordLabel = new QLabel("Пароль:");
    password_edit = new QLineEdit();
    password_edit->setEchoMode(QLineEdit::Password);

    QPushButton * loginButton = new QPushButton("Войти");
    status_label = new QLabel();

    layout->addWidget(loginLabel);
    layout->addWidget(login_edit);
    layout->addWidget(passwordLabel);
    layout->addWidget(password_edit);
    layout->addWidget(loginButton);
    layout->addWidget(status_label);

    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::handleLogin);

    setLayout(layout);
    setWindowTitle("Авторизация");
    resize(300, 180);
}

void AuthWindow::handleLogin() {
    QString login = login_edit->text();
    QString password = password_edit->text();

    User *user = auth_system->login(login, password);
    if (user) {
        status_label->setText("Успешный вход!");

        Interface *ui = user->role() == "admin" ?
                        static_cast<Interface *>(new AdminInterface()) :
                        static_cast<Interface *>(new UserInterface(user));

        ui->display();

        close();
    } else {
        status_label->setText("Неверный логин или пароль");
    }
}
