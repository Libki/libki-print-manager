#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>

class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

private:
    QSystemTrayIcon *trayIcon;

signals:

private slots:
    void doQuitAction();
    void doAboutAction();

public slots:
    void showTrayMessage(QString title, QString message);
};

#endif // SYSTEMTRAY_H
