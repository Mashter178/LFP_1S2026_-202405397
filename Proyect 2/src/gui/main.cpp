#include <QApplication>
#include <QIcon>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/medlang.svg"));

    MainWindow window;
    window.setWindowIcon(QIcon(":/icons/medlang.svg"));
    window.show();

    return app.exec();
}
