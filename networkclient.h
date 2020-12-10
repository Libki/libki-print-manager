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
    void downloadPrintFile(QJsonObject job);
    QString libkiServerAddress;
    QNetworkAccessManager *namCheckServerForPrintJobs;

signals:

public slots:

private slots:
    void checkServerForPrintJobs();
    void onCheckServerForPrintJobsResult(QNetworkReply *reply);
    void downloadPrintFileFinished(QNetworkReply *reply);

};

#endif // NETWORKCLIENT_H
