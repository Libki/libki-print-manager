#ifndef LOGUTILS_H
#define LOGUTILS_H

#define LOGSIZE 1024 * 100 //log size in bytes
#define LOGFILES 20

#include <QObject>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QTime>

namespace LogUtils
{
    enum LogLevel {
        DebugLevel,
        WarningLevel,
        ErrorLevel,
        OffLevel
    };

    bool initLogging();
    void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg);
}

#endif // LOGUTILS_H
