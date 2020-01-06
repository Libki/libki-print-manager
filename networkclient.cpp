#include "networkclient.h"

#include <QDebug>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    qDebug() << "NetworkClient::NetworkClient";
    checkServerForPrintJobsTimer = new QTimer(this);
    connect(checkServerForPrintJobsTimer, SIGNAL(timeout()), this, SLOT(checkServerForPrintJobs()));
    checkServerForPrintJobsTimer->start(5 * 1000); //TODO: Move check to a setting?
}

void NetworkClient::checkServerForPrintJobs() {
    qDebug() << "NetworkClient::checkServerForPrintJobs";
}
