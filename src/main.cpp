#include <QApplication>
#include "../include/auth_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AuthWindow window;
    window.show();

    return app.exec();
}
