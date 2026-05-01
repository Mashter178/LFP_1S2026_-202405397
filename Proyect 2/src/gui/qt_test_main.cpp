#include <QApplication>
#include <QLabel>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow w;
    w.setWindowTitle("TaskScript - Qt Test");
    QLabel* label = new QLabel("Ventana de prueba Qt funcionando", &w);
    label->setAlignment(Qt::AlignCenter);
    w.setCentralWidget(label);
    w.resize(480, 240);
    w.show();
    return app.exec();
}
