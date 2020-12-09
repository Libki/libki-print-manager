#include "systemtray.h"
#include "networkclient.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QCoreApplication::setOrganizationName("Libki");
    QCoreApplication::setOrganizationDomain("libki.org");
    QCoreApplication::setApplicationName("Libki Print Manager");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setIniCodec("UTF-8");

    SystemTray st;
    NetworkClient nc;

    return a.exec();
}
