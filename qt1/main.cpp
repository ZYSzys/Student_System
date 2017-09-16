#include "mainwindow.h"
#include <QApplication>
#include <dbconnect.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbConnect conDialog;
    if(conDialog.exec() != QDialog :: Accepted)
    {
        return -1;
    }

    MainWindow w("student");

    QPalette palette;
    QPixmap pixmap(":/ZAFU.jpeg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    w.setPalette(palette);
    //w.setMask(pixmap.mask());//将图片中透明部分显示为透明
    w.setAutoFillBackground(true);
    w.show();

    return a.exec();
}

