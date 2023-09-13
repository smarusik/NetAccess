#include "initiator.h"

Initiator::Initiator(QObject *parent):
    QObject(parent),
    anonymousToken(new Subject(DataType::ANONYMOUS_TOKEN)),
    accessToken(new Subject(DataType::ACCESS_TOKEN)),
    proxyAccessToken(new Subject(DataType::PROXY_ACCESS_TOKEN)),
    autoserverLocation(new Subject(DataType::AUTOSERVER_LOCATION))
{
    connect(anonymousToken, &Subject::data_ready,
            this, &Initiator::requestedDataReady,
            Qt::QueuedConnection);
    connect(anonymousToken, &Subject::error_occured,
            this, &Initiator::reportError,
            Qt::QueuedConnection);

    connect(accessToken, &Subject::data_ready,
            this, &Initiator::requestedDataReady,
            Qt::QueuedConnection);
    connect(accessToken, &Subject::error_occured,
            this, &Initiator::reportError,
            Qt::QueuedConnection);

    connect(proxyAccessToken, &Subject::data_ready,
            this, &Initiator::requestedDataReady,
            Qt::QueuedConnection);
    connect(proxyAccessToken, &Subject::error_occured,
            this, &Initiator::reportError,
            Qt::QueuedConnection);

    connect(autoserverLocation, &Subject::data_ready,
            this, &Initiator::requestedDataReady,
            Qt::QueuedConnection);
    connect(autoserverLocation, &Subject::error_occured,
            this, &Initiator::reportError,
            Qt::QueuedConnection);
}

void Initiator::anonymousTokenRq(QNetworkRequest rq, QByteArray payload)
{
    emit subjectUpdateRequested(anonymousToken, rq, payload);
}

void Initiator::accessTokenRq(QNetworkRequest rq, QByteArray payload)
{
    emit subjectUpdateRequested(accessToken,rq, payload);
}

void Initiator::proxyAccessTokenRq(QNetworkRequest rq, QByteArray payload)
{
    emit subjectUpdateRequested(proxyAccessToken,rq, payload);
}

void Initiator::autoserverLocationRq(QNetworkRequest rq, QByteArray payload)
{
    emit subjectUpdateRequested(autoserverLocation,rq, payload);
}
