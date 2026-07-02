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
    QString printManagerName;
    QString printManagerApiKey;
    QString customHeaderName;
    QString customHeaderValue;
    QNetworkAccessManager *namCheckServerForPrintJobs;
    QNetworkAccessManager *namDownloadPrintFileFinished;
    QNetworkAccessManager *namJobStatus;

    QNetworkRequest buildRequest(const QUrl &url) const;

signals:
    void requestShowTrayMessage(QString title, QString message);

public slots:

private slots:
    void checkServerForPrintJobs();
    void onCheckServerForPrintJobsResult(QNetworkReply *reply);
    void downloadPrintFileFinished(QNetworkReply *reply);
    void handleSslErrors(QNetworkReply* reply,QList<QSslError> error);
};

#endif // NETWORKCLIENT_H
