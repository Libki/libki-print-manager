#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>

class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

signals:

private slots:
    void doQuitAction();
    void doAboutAction();

public slots:
};

#endif // SYSTEMTRAY_H
