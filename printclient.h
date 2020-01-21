#ifndef PRINTCLIENT_H
#define PRINTCLIENT_H

#include <QObject>

class PrintClient : public QObject
{
    Q_OBJECT
public:
    explicit PrintClient(QObject *parent = 0);

signals:

public slots:
//    void sendJobToPrinter();

};

#endif // PRINTCLIENT_H
