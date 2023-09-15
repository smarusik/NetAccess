#include "subject.h"
#include <QNetworkAccessManager>

void Subject::finishRequest()
{
    setIn_progress(false);
}

Subject::Subject(DataType tp, QObject *parent):
    QObject(parent),
    dataType_(tp)
{
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
    return in_progress_.load();
}

void Subject::setIn_progress(volatile bool newIn_progress)
{
    in_progress_.store(newIn_progress);
}

void Subject::operator()(QNetworkAccessManager *nm,
                         QNetworkRequest rq, QByteArray payload)
{
    if(in_progress())
        return;

    setIn_progress(true);
    if(payload.isEmpty())
        setReply(nm->get(rq));
    else
        setReply(nm->post(rq,payload));

    //Singleshot
    connect(reply_, &QNetworkReply::finished,
            this, &Subject::processData,
            Qt::QueuedConnection);
}

void Subject::processData()
{
    {
        std::lock_guard _(mu_);
        if(reply_->error()==QNetworkReply::NoError)
        {
            data_=reply_->readAll();
            emit data_ready(data_, dataType_);
        }
        else
        {
            emit error_occured(reply_->error(), reply_->errorString());
        }
        disconnect(this, nullptr, nullptr, nullptr);
        connect(reply_, &QNetworkReply::destroyed,
                this, &Subject::finishRequest);
        reply_->close();
        reply_->disconnect();
        reply_->deleteLater();

    }
}
