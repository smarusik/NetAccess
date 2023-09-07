#include "initiator.h"

void Initiator::subject1UpdateRequested(const QByteArray data)
{
    emit subjectUpdateRequested(subject1_);
}

void Initiator::subject2UpdateRequested(const QByteArray data)
{
    emit subjectUpdateRequested(subject2_);
}

void Initiator::collectRequestData(const QByteArray data)
{
    QByteArray result="Subject1: "+subject1_->data()+" Subject2: "+data;
    emit requestedDataReady(result);
}

void Initiator::reportError(QNetworkReply::NetworkError error, QString errorString)
{
    emit dataRequestError(errorString);
}

Initiator::Initiator(QObject *parent):
    QObject(parent),
    subject1_(new Subject(1)),
    subject2_(new Subject(2))
{
}

void Initiator::requestData()
{
    Subject* start_subject=subject2_;

    if(!start_subject->ready())
    {
        connect(start_subject, &Subject::data_ready,
                this, &Initiator::collectRequestData,
                Qt::SingleShotConnection);
        connect(start_subject, &Subject::error_occured,
                this, &Initiator::reportError,
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
        connect(start_subject, &Subject::error_occured,
                this, &Initiator::reportError,
                Qt::SingleShotConnection);
    }

    emit subjectUpdateRequested(start_subject);
}
