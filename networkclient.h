#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>

#include <QtNetwork/QNetworkReply>
#include <QTimer>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = 0);

private:
    QTimer *checkServerForPrintJobsTimer;

signals:

public slots:

private slots:
    void checkServerForPrintJobs();
    void onCheckServerForPrintJobsResult(QNetworkReply *reply);

};

#endif // NETWORKCLIENT_H
