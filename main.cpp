#include "systemtray.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    //MainWindow w;
    //w.show();

    SystemTray st;
    //qDebug() << "THIS IS A TEST";
    //QProcess::startDetached("C:\\SumatraPDF.exe -print-to BrotherLaser C:\\sample.pdf");

    return a.exec();
}
