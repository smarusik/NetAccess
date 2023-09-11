#include "subject.h"
#include <QNetworkAccessManager>

void Subject::finishRequest()
{
    setIn_progress(false);
}

Subject::Subject(int id, QObject *parent):
    QObject(parent),
    id(id)
{}

bool Subject::ready()
{
    std::lock_guard _(mu_);
    return ready_;
}

void Subject::setReady(volatile bool r)
{
    std::lock_guard _(mu_);
    ready_=r;
}

const QByteArray Subject::data() const
{
    std::lock_guard _(mu_);
    return data_;
}

void Subject::setReply(QNetworkReply *reply)
{
    std::lock_guard _(mu_);
    reply_=reply;
}

volatile bool Subject::in_progress() const
{
    std::lock_guard _(mu_);
    return in_progress_;
}

void Subject::setIn_progress(volatile bool newIn_progress)
{
    std::lock_guard _(mu_);
    in_progress_ = newIn_progress;
}

void Subject::operator()(QNetworkAccessManager *nm,
                         QNetworkRequest rq)
{
    if(in_progress())
        return;

    setIn_progress(true);
    setReply(nm->get(rq));

    //Singleshot
    connect(reply_, &QNetworkReply::finished,
            this, &Subject::processData,
            Qt::QueuedConnection);
}

const QString &Subject::url()
{
    return url_;
}

void Subject::processData()
{
    {
        std::lock_guard _(mu_);
        if(reply_->error()==QNetworkReply::NoError)
        {
            data_=reply_->readAll();
            ready_=true;
            emit data_ready(data_);
        }
        else
        {
            emit error_occured(reply_->error(), reply_->errorString());
        }
        reply_->close();
        reply_->disconnect();
        reply_->deleteLater();
        disconnect(this, nullptr, nullptr, nullptr);
        connect(reply_, &QNetworkReply::destroyed,
                this, &Subject::finishRequest);

    }
}
