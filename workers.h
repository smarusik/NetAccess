#ifndef WORKERS_H
#define WORKERS_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QObject>
#include <mutex>

class Subject:public QObject
{
    mutable std::mutex mu_;
    QByteArray data_;
    volatile bool ready_=false;

    Q_OBJECT

private slots:
public:
    void operator()(QNetworkAccessManager* nm)
    {
        QNetworkRequest rq;
        rq.setUrl(QUrl("https://geo.geosurf.io"));
        connect(nm, &QNetworkAccessManager::finished,
                this, &Subject::processData,
                Qt::UniqueConnection);
        nm->get(rq);
    }

    bool ready()
    {
        std::lock_guard _(mu_);
        return ready_;
    }

    void setReady(volatile bool r)
    {
        std::lock_guard _(mu_);
        ready_=r;
    }

public slots:
    void processData(QNetworkReply *reply)
    {
        {
            std::lock_guard _(mu_);
            data_=reply->readAll();
            reply->disconnect();
            reply->deleteLater();
            qDebug()<<data_;
            ready_=true;
        }

        emit data_ready();
    }
signals:
    void data_ready();
};

class Worker : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *nam;
    QThread th;

public:
    Worker(QObject* parent=nullptr):
        nam(new QNetworkAccessManager(this))
    {
        moveToThread(&th);
        th.start();
    }

public slots:
    void process(Subject* subject)
    {
        subject->setReady(false);
        (*subject)(nam);
    }

    void quit()
    {
        th.quit();
        th.wait();
    }
};

class Initiator: public QObject
{
    Q_OBJECT

    Subject *subject1_, *subject2_;

public:
    Initiator(QObject *parent=nullptr):
        QObject(parent),
        subject1_(new Subject),
        subject2_(new Subject)
    {
    }

    void requestData()
    {
        emit subjectUpdateRequested(subject1_);
    }

signals:
    void subjectUpdateRequested(Subject*);
};

#endif // WORKERS_H
