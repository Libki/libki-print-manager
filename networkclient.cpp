#include "networkclient.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDir>
#include <QProcess>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    qDebug() << "NetworkClient::NetworkClient";

    checkServerForPrintJobsTimer = new QTimer(this);
    connect(checkServerForPrintJobsTimer, SIGNAL(timeout()), this, SLOT(checkServerForPrintJobs()));
    checkServerForPrintJobsTimer->start(10 * 1000); //TODO: Move check to a setting?
}

void NetworkClient::checkServerForPrintJobs() {
    qDebug() << "NetworkClient::checkServerForPrintJobs";

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &NetworkClient::onCheckServerForPrintJobsResult);

    QUrlQuery query;
    query.addQueryItem("name", "PrintManager1"); //TODO: Move name to config file

    QUrl url = QUrl("http://192.168.56.1:3000/api/printmanager/v1_0/get_pending_job/"); //TODO: Move address and port to config file
    url.setQuery(query);

    qDebug()<< "url: "<< url.toString(QUrl::FullyEncoded);

    nam->get(QNetworkRequest(url));
}

void NetworkClient::onCheckServerForPrintJobsResult(QNetworkReply *reply){
    qDebug() << "NetworkClient::onCheckServerForPrintJobsResult";

    if (reply->error() == QNetworkReply::NoError){

        QByteArray result = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
        QJsonObject obj = jsonResponse.object();
        QJsonValue jobSet = obj.value(QString("job"));
        QJsonObject job = jobSet.toObject();
        qDebug() << "Response: " << jsonResponse.toJson(QJsonDocument::Compact);
        qDebug() << "Job:" << job;
        qDebug() << "job_id: " << job["job_id"].toInt();
        qDebug() << "copies: " << job["copies"].toInt();
        qDebug() << "printer: " << job["printer"].toString();
        qDebug() << "user_id: " << job["user_id"].toString();
        qDebug() << "print_file_id" << job["print_file_id"].toInt();

        if ( job["job_id"].toInt() != 0 ) {
            downloadPrintFile(job);
        } // else job was empty, no job to print
    }
    else {
        qDebug() << "ERROR";
    }

    reply->deleteLater();
}

void NetworkClient::downloadPrintFile(QJsonObject job) {
    qDebug() << "NetworkClient::downloadPrintFile";
    qDebug() << "Job ID:" << job["job_id"].toInt();

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &NetworkClient::downloadPrintFileFinished);

    QString print_file_id = QString(QString::number(job["print_file_id"].toInt()));
    qDebug() << "Print File ID: " << print_file_id;

    QUrl url = QUrl("http://192.168.56.1:3000/api/printmanager/v1_0/get_file/" + print_file_id ); //TODO: Move address and port to config file
    qDebug() << "PDF URL: " << url.toString();
    nam->get(QNetworkRequest(url));
}

void NetworkClient::downloadPrintFileFinished(QNetworkReply *reply) {
    qDebug() << "NetworkClient::downloadPrintFileFinished";

    QString fileId = reply->rawHeader("File-Id");
    qDebug() << "FILE ID: " + fileId;

    QString tempDir = QDir::tempPath();
    qDebug() << "Temp Dir: " << tempDir;

    QString tempFile = tempDir + "/" + fileId + ".pdf";
    QFile localFile( tempFile );
    if (!localFile.open(QIODevice::WriteOnly))
        return;
    localFile.write(reply->readAll());
    localFile.close();

    QProcess::startDetached("C:\\SumatraPDF.exe -print-to BrotherLaser " + tempFile);

    reply->deleteLater();
    qDebug() << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
}

