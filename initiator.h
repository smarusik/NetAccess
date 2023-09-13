#ifndef INITIATOR_H
#define INITIATOR_H

#include "subject.h"

class Initiator: public QObject
{
    Q_OBJECT
Subject *anonymousToken, *accessToken, *proxyAccessToken,
        *autoserverLocation;
public slots:
    void anonymousTokenRq(QNetworkRequest rq, QByteArray payload);
    void accessTokenRq(QNetworkRequest rq, QByteArray payload);
    void proxyAccessTokenRq(QNetworkRequest rq, QByteArray payload);
    void autoserverLocationRq(QNetworkRequest rq, QByteArray payload);

public:
    Initiator(QObject *parent=nullptr);

signals:
    void subjectUpdateRequested(Subject*,
                                QNetworkRequest rq,
                                QByteArray payload);
    void requestedDataReady(QByteArray data, DataType tp);
    void reportError(QNetworkReply::NetworkError error,
                     QString errorString);

};

#endif // INITIATOR_H
