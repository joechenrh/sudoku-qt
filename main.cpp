#include "mainwindow.h"
#include <QApplication>

#include "buttongroup.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //MainWindow w;
    //Qt::WindowFlags flags = nullptr;
    //flags |= Qt::WindowMinimizeButtonHint;
    //w.setWindowFlags(flags); // 设置禁止最大化
    //w.show();

    ButtonGroup *g = new ButtonGroup(75);
    g->show();

    return app.exec();
}
