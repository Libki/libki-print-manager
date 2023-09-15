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
#include <QFileInfo>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    qDebug() << "NetworkClient::NetworkClient";

    namJobStatus = new QNetworkAccessManager(this);
    connect(namJobStatus, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )), this, SLOT(handleSslErrors(QNetworkReply*, const QList<QSslError> & )));

    namCheckServerForPrintJobs = new QNetworkAccessManager(this);
    connect(namCheckServerForPrintJobs, &QNetworkAccessManager::finished, this, &NetworkClient::onCheckServerForPrintJobsResult);
    connect(namCheckServerForPrintJobs, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )), this, SLOT(handleSslErrors(QNetworkReply*, const QList<QSslError> & )));

    namDownloadPrintFileFinished = new QNetworkAccessManager(this);
    connect(namDownloadPrintFileFinished, &QNetworkAccessManager::finished, this, &NetworkClient::downloadPrintFileFinished);
    connect(namDownloadPrintFileFinished, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )), this, SLOT(handleSslErrors(QNetworkReply*, const QList<QSslError> & )));

    checkServerForPrintJobsTimer = new QTimer(this);
    connect(checkServerForPrintJobsTimer, SIGNAL(timeout()), this, SLOT(checkServerForPrintJobs()));
    checkServerForPrintJobsTimer->start(10 * 1000); //TODO: Move check to a setting?

    QSettings settings;
    settings.setIniCodec("UTF-8");
    libkiServerAddress = settings.value("settings/server").toString();
    printManagerName = settings.value("settings/name").toString();
    printManagerApiKey = settings.value("settings/api_key").toString();

    qDebug() << "LIBKI SERVER ADDRESS: " << libkiServerAddress;
    qDebug() << "PRINT MANAGER NAME: " << printManagerName;
    emit requestShowTrayMessage("Libki Print Manager", "Starting up");
}

void NetworkClient::checkServerForPrintJobs() {
    qDebug() << "NetworkClient::checkServerForPrintJobs";

    QUrlQuery query;
    query.addQueryItem("name", printManagerName);
    query.addQueryItem("api_key", printManagerApiKey);

    QUrl url = QUrl( libkiServerAddress + "/api/printmanager/v1_0/get_pending_job/");
    url.setQuery(query);

    qDebug()<< "url: "<< url.toString(QUrl::FullyEncoded);

    namCheckServerForPrintJobs->get(QNetworkRequest(url));
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
        qDebug() << "ERROR: " << reply->errorString();
    }

    reply->deleteLater();
}

void NetworkClient::downloadPrintFile(QJsonObject job) {
    qDebug() << "NetworkClient::downloadPrintFile";
    qDebug() << "Job ID:" << job["job_id"].toInt();

    QString print_file_id = QString(QString::number(job["print_file_id"].toInt()));
    qDebug() << "Print File ID: " << print_file_id;

    QString print_job_id = QString(QString::number(job["job_id"].toInt()));
    emit requestShowTrayMessage("Libki Print Manager", "Downloading print job " + print_job_id);

    QUrlQuery query;
    query.addQueryItem("name", printManagerName);
    query.addQueryItem("api_key", printManagerApiKey);

    QUrl url = QUrl( libkiServerAddress + "/api/printmanager/v1_0/get_file/" + print_file_id );
    url.setQuery(query);

    qDebug() << "DOWNLOADING PDF FROM URL: " << url.toString();
    QNetworkReply* reply = namDownloadPrintFileFinished->get(QNetworkRequest(url));
    reply->setProperty("job", job);
}

void NetworkClient::downloadPrintFileFinished(QNetworkReply *reply) {
    qDebug() << "NetworkClient::downloadPrintFileFinished";

    QSettings settings;
    settings.setIniCodec("UTF-8");
    QString tempDir = settings.value("settings/tempdir").toString();

    if (reply->error() == QNetworkReply::NoError){
        qDebug() << "1) " << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "2) " << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << "3) " << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << "4) " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "5) " << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        QJsonObject job = reply->property("job").toJsonObject();
        // The line below causes problems when packaged for deployment,
        // the c string library is not included in libstdc++-6.dll apparently
        //qDebug() << "JOB: " << QJsonDocument(job).toJson(QJsonDocument::Compact).toStdString().c_str();
        qDebug() << "PRINT FILE ID: " << QString::number( job["job_id"].toInt() );

        //QString jobId = reply->rawHeader("File-Id");
        QString jobId = QString::number( job["job_id"].toInt() );
        qDebug() << "FILE ID: " + jobId;
        QString printer = job["printer"].toString();
        qDebug() << "VIRTUAL PRINTER NAME: " << printer;
        QString physicalPrinterName = job["physical_printer_name"].toString();
        qDebug() << "PHYSICAL PRINTER NAME: " << physicalPrinterName;
        QString chroming = job["chroming"].toString();
        qDebug() << "CHROMING: " << chroming;
        QString plexing = job["plexing"].toString();
        qDebug() << "PLEXING: " << plexing;

        emit requestShowTrayMessage("Libki Print Manager", "Printing job " + jobId);

        QUrlQuery query;
        query.addQueryItem("name", printManagerName);
        query.addQueryItem("api_key", printManagerApiKey);

        QUrl url = QUrl(QString("%1/api/printmanager/v1_0/job/%2/InProgress").arg(libkiServerAddress, jobId));
        qDebug() << "IN PROGRESS URL: " << url;

        QNetworkRequest req = QNetworkRequest(QUrl(url));
        namJobStatus->get(req);

        if ( tempDir.isEmpty() ) {
            tempDir = QDir::tempPath();
        }
        tempDir = tempDir + "/" + printer;
        QDir(tempDir).mkpath(tempDir);
        qDebug() << "TEMP DIR " << tempDir << " EXISTS: " <<  QDir(tempDir).exists();

        QString tempFile = tempDir + "/" + jobId + ".pdf";
        QFileInfo fi(tempFile);
        tempFile = fi.absoluteFilePath();
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
        QString command = QString("SumatraPDF.exe -silent  -print-settings \"%1,%2\" -print-to \"%3\" \"%4\"").arg(chroming, plexing, physicalPrinterName, tempFile);
        qDebug().noquote() << "PRINT COMMAND: " << command;
        sumatra.start(command);
        sumatra.waitForStarted();
        qDebug() << "PRINTING STARTED";
        sumatra.waitForFinished();
        qDebug() << "PRINTING DONE!";
        qDebug() << "EXIT STATUS: " << sumatra.exitCode();

        if ( sumatra.exitStatus() == QProcess::NormalExit && sumatra.exitCode() == 0 ) {
            url = QUrl(QString("%1/api/printmanager/v1_0/job/%2/Done").arg(libkiServerAddress, jobId));
            url.setQuery(query);

            req = QNetworkRequest(url);
            qDebug() << "DONE URL: " << url;
            qDebug() << "PRINTING " << tempFile << " SUCEEDED!";

            namJobStatus->get(req);
            qDebug() << "NETWORK REQUEST SENT: Done";
        } else {
            url = QUrl(QString("%1/api/printmanager/v1_0/job/%2/Error").arg(libkiServerAddress, jobId));
            url.setQuery(query);

            req = QNetworkRequest(url);
            qDebug() << "PRINTING " << tempFile << " FAILED!";
            qDebug() << "ERROR URL: " << url;

            namJobStatus->get(req);
            qDebug() << "NETWORK REQUEST SENT: Error";
        }

        reply->deleteLater();

        QFile file(tempFile);
        file.remove();
    } else {
        qDebug() << "ERROR: " << reply->errorString();
    }
}

void NetworkClient::handleSslErrors(QNetworkReply *reply, QList<QSslError> error ) {
  reply->ignoreSslErrors(error);
}

