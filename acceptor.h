#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <QObject>
#include <QDebug>
#include <QThread>

class Acceptor : public QObject
{
    Q_OBJECT
    QThread th;
public:
    Acceptor(QObject *parent=nullptr)
    {
        moveToThread(&th);
        th.start();
    }

    virtual ~Acceptor() {}

public slots:
    void dataReceived(QByteArray data)
    {
        qDebug()<<data;
    }
};
#endif // ACCEPTOR_H
