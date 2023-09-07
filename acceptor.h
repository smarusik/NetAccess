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
    Acceptor(QObject *parent=nullptr);

    virtual ~Acceptor();

public slots:
    void dataReceived(QByteArray data);

    void errorReceived(QString errorString);

};
#endif // ACCEPTOR_H
