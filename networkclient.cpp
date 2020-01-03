#include "networkclient.h"

#include <QDebug>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{

}

void NetworkClient::checkServerForPrintJobs() {
    qDebug() << "CALLED checkServerForPrintJobs";
}
