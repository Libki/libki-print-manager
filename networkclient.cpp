#include "networkclient.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    qDebug() << "NetworkClient::NetworkClient";

    checkServerForPrintJobsTimer = new QTimer(this);
    connect(checkServerForPrintJobsTimer, SIGNAL(timeout()), this, SLOT(checkServerForPrintJobs()));
    checkServerForPrintJobsTimer->start(5 * 1000); //TODO: Move check to a setting?
}

void NetworkClient::checkServerForPrintJobs() {
    qDebug() << "NetworkClient::checkServerForPrintJobs";

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &NetworkClient::onCheckServerForPrintJobsResult);

    QUrlQuery query;
    query.addQueryItem("name", "PrintManager1"); //TODO: Move name to config file

    QUrl url = QUrl("http://192.168.1.135:3000/api/printmanager/v1_0/get_pending_job/");
    url.setQuery(query);

    qDebug()<< "url: "<< url.toString(QUrl::FullyEncoded);

    nam->get(QNetworkRequest(url));
}

void NetworkClient::onCheckServerForPrintJobsResult(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){

        QByteArray result = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
        QJsonObject obj = jsonResponse.object();
        qDebug()<<"code: " << obj["code"].toInt();
        qDebug()<<"lang: " << obj["lang"].toString();
        QJsonArray array = obj["text"].toArray();

        for(const QJsonValue & value : array) {
           qDebug()<< "text: " <<value.toString();
        }
    }
    else {
        qDebug() << "ERROR";
    }

    reply->deleteLater();
}
