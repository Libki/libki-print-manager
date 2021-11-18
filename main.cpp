#include "systemtray.h"
#include "networkclient.h"
#include "logutils.h"

#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    LogUtils::initLogging();

    QCoreApplication::setOrganizationName("Libki");
    QCoreApplication::setOrganizationDomain("libki.org");
    QCoreApplication::setApplicationName("Libki Print Manager");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setIniCodec("UTF-8");

    SystemTray st;
    NetworkClient nc;

    QObject::connect(&nc, &NetworkClient::requestShowTrayMessage, &st, &SystemTray::showTrayMessage);

    return a.exec();
}
