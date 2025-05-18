#include <QApplication>
#include "../include/user.h"
#include "../include/user_interface.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    User *user = new User(123, "Иванов И. И.", "Студент", "ivanov", "1234");
    UserInterface *ui = new UserInterface(user);

    ui->show_schedule();
    ui->display();

    return app.exec();
}
