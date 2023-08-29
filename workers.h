#ifndef WORKERS_H
#define WORKERS_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QObject>

class Acceptor : public QObject
{
    Q_OBJECT
public:
    Acceptor(QObject *parent=nullptr):QObject(parent){}
public slots:
    void acceptData(QByteArray data)
    {
        qDebug()<<data;
    }

};

class Emitter : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QNetworkReply *reply;
private slots:
    void finalizeRequest()
    {
        QByteArray data =reply->readAll();
        reply->close();
        reply->deleteLater();
        emit emitData(data);
    }
public:
    Emitter(QObject *parent=nullptr): QObject(parent){}
    void run()
    {
        QNetworkRequest request;
        request.setUrl(QUrl("https://geo.geosurf.io"));

        reply=manager.get(request);
        connect(reply, &QNetworkReply::finished,
                this, &Emitter::finalizeRequest,
                Qt::UniqueConnection);
    }
signals:
    void emitData(QByteArray data);
};

#endif // WORKERS_H
