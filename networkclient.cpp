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
#include <QSettings>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    qDebug() << "NetworkClient::NetworkClient";

    checkServerForPrintJobsTimer = new QTimer(this);
    connect(checkServerForPrintJobsTimer, SIGNAL(timeout()), this, SLOT(checkServerForPrintJobs()));
    checkServerForPrintJobsTimer->start(10 * 1000); //TODO: Move check to a setting?

    QSettings settings;
    settings.setIniCodec("UTF-8");
    libkiServerAddress = settings.value("settings/server").toString();
    qDebug() << "LIBKI SERVER ADDRESS: " << libkiServerAddress;
}

void NetworkClient::checkServerForPrintJobs() {
    qDebug() << "NetworkClient::checkServerForPrintJobs";

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &NetworkClient::onCheckServerForPrintJobsResult);

    QUrlQuery query;
    query.addQueryItem("name", "PrintManager1"); //TODO: Move name to config file

    QUrl url = QUrl( libkiServerAddress + "/api/printmanager/v1_0/get_pending_job/");
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

    QUrl url = QUrl( libkiServerAddress + "/api/printmanager/v1_0/get_file/" + print_file_id );
    qDebug() << "DOWNLOADING PDF FROM URL: " << url.toString();
    QNetworkReply* reply = nam->get(QNetworkRequest(url));
    reply->setProperty("job", job);
}

void NetworkClient::downloadPrintFileFinished(QNetworkReply *reply) {
    qDebug() << "NetworkClient::downloadPrintFileFinished";


    if (reply->error()) {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    } else {
        qDebug() << "1) " << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "2) " << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << "3) " << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << "4) " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "5) " << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        QJsonObject job = reply->property("job").toJsonObject();
        qDebug() << "JOB: " << QJsonDocument(job).toJson(QJsonDocument::Compact).toStdString().c_str();
        qDebug() << "PRINT FILE ID: " << QString::number( job["job_id"].toInt() );

        //QString jobId = reply->rawHeader("File-Id");
        QString jobId = QString::number( job["job_id"].toInt() );
        qDebug() << "FILE ID: " + jobId;
        QString physicalPrinterName = job["physical_printer_name"].toString();
        qDebug() << "PRINTER NAME: " << physicalPrinterName;

        QNetworkAccessManager nam;
        nam.get(QNetworkRequest(QUrl(libkiServerAddress + "/api/printmanager/v1_0/job/" + jobId + "/InProgress" )));

        QString tempDir = QDir::tempPath();
        qDebug() << "Temp Dir: " << tempDir;

        QString tempFile = tempDir + "/" + jobId + ".pdf";
        QFile localFile(tempFile);
        if (localFile.open(QIODevice::WriteOnly)) {
           localFile.write(reply->readAll());
           localFile.flush();
           localFile.close();
        } else {
            qDebug() << "Failed to open file for writing: " + tempFile;
        }

        QProcess sumatra;
        qDebug() << "PRINTING TO START";
        QString command = "C:\\SumatraPDF.exe -silent -print-to " + physicalPrinterName + " " + tempFile;
        qDebug() << "PRINT COMMAND: " << command;
        sumatra.start(command);
        sumatra.waitForStarted();
        qDebug() << "PRINTING STARTED";
        sumatra.waitForFinished();
        qDebug() << "PRINTING DONE!";
        qDebug() << "EXIT STATUS: " << sumatra.exitCode();

        if ( sumatra.exitStatus() == QProcess::NormalExit && sumatra.exitCode() == 0 ) {
            qDebug() << "PRINTING " << tempFile << " SUCEEDED!";
            nam.get(QNetworkRequest(QUrl(libkiServerAddress + "/api/printmanager/v1_0/job/" + jobId + "/Done" )));
        } else {
            qDebug() << "PRINTING " << tempFile << " FAILED!";
            nam.get(QNetworkRequest(QUrl(libkiServerAddress + "/api/printmanager/v1_0/job/" + jobId + "/Error" )));
        }

        reply->deleteLater();

        QFile file(tempFile);
        file.remove();
    }
}

