#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = 0);

signals:

public slots:
    void checkServerForPrintJobs();
};

#endif // NETWORKCLIENT_H
