#include "systemtray.h"
#include "networkclient.h"

#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QTextStream>

void messageHandler( QtMsgType type, const QMessageLogContext &context, const QString &message ) {
    QString levelText;
    switch(type) {
        case QtDebugMsg:
            levelText = "Debug";
            break;
        case QtInfoMsg:
            levelText = "Info";
            break;
        case QtWarningMsg:
            levelText = "Warning";
            break;
        case QtCriticalMsg:
            levelText = "Critical";
            break;
        case QtFatalMsg:
            levelText = "Fatal";
            break;
    }

    QString text = QString("%3 [%1] %2")
        .arg(levelText)
        .arg(message)
        .arg(QDateTime::currentDateTime().toString(Qt::ISODate));

    QFile file("printmanager.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&file);
    textStream << text << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    qInstallMessageHandler(messageHandler);

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
