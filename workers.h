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
    const int id;
    QByteArray data_;
    QNetworkReply *reply_;
    volatile bool ready_=false;
    volatile bool in_progress_=false;

    Q_OBJECT

private slots:
    void finishRequest()
    {
        setIn_progress(false);
    }
public:
    Subject(int id, QObject* parent = nullptr):
        QObject(parent),
        id(id)
    {}

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

    const QByteArray data() const
    {
        std::lock_guard _(mu_);
        return data_;
    }

    void setReply(QNetworkReply* reply)
    {
        std::lock_guard _(mu_);
        reply_=reply;
    }

    volatile bool in_progress() const
    {
        std::lock_guard _(mu_);
        return in_progress_;
    }
    void setIn_progress(volatile bool newIn_progress)
    {
        std::lock_guard _(mu_);
        in_progress_ = newIn_progress;
    }

    void operator()(QNetworkAccessManager* nm)
    {
        if(in_progress())
            return;

        setIn_progress(true);
        QNetworkRequest rq;
        rq.setUrl(QUrl("https://geo.geosurf.io"));
        setReply(nm->get(rq));
        connect(reply_, &QNetworkReply::finished,
                this, &Subject::processData,
                Qt::SingleShotConnection);
    }

public slots:
    void processData()
    {
        {
            std::lock_guard _(mu_);
            data_=reply_->readAll();
            reply_->disconnect();
            reply_->deleteLater();
            connect(reply_, &QNetworkReply::destroyed,
                    this, &Subject::finishRequest);
            ready_=true;
        }

        emit data_ready(data_);
    }
signals:
    void data_ready(QByteArray data);
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
private slots:
    void subject1UpdateRequested(const QByteArray data)
    {
        emit subjectUpdateRequested(subject1_);
    }
    void subject2UpdateRequested(const QByteArray data)
    {
        emit subjectUpdateRequested(subject2_);
    }
    void collectRequestData(const QByteArray data)
    {
        QByteArray result="Subject1: "+subject1_->data()+" Subject2: "+data;
        emit requestedDataReady(result);
    }
public:
    Initiator(QObject *parent=nullptr):
        QObject(parent),
        subject1_(new Subject(1)),
        subject2_(new Subject(2))
    {
    }

    void requestData()
    {
        Subject* start_subject=subject2_;

        if(!start_subject->ready())
        {
            connect(start_subject, &Subject::data_ready,
                    this, &Initiator::collectRequestData,
                    Qt::SingleShotConnection);

            start_subject=subject1_;
        }
        else
            collectRequestData(subject2_->data());

        if(!start_subject->ready())
        {
            connect(start_subject, &Subject::data_ready,
                    this, &Initiator::subject2UpdateRequested,
                    Qt::SingleShotConnection);
        }

        emit subjectUpdateRequested(start_subject);
    }

signals:
    void subjectUpdateRequested(Subject*);
    void requestedDataReady(QByteArray data);
};

#endif // WORKERS_H
