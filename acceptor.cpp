#include "acceptor.h"

Acceptor::Acceptor(QObject *parent)
{
    moveToThread(&th);
    th.start();
}

Acceptor::~Acceptor() {}

void Acceptor::dataReceived(QByteArray data)
{
    qDebug()<<data;
}

void Acceptor::errorReceived(QString errorString)
{
    qDebug()<<errorString;
}
