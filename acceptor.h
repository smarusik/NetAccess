#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "subject.h"
#include "tokens/accesstokenbase.h"
#include "tokens/accesstoken.h"
#include "tokens/proxyaccesstoken.h"
#include "locations/autoserverlocation.h"

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QJsonDocument>

#include <mutex>
#include <list>

class Acceptor : public QObject
{
    Q_OBJECT
    QThread th;
    mutable std::mutex mu;
    const QByteArray machine_id;

    AccessTokenBase *anonymousToken_;
    AccessToken *accessToken_;
    ProxyAccessToken *proxyAccessToken_;
    AutoserverLocation *autoserverLocation_;

public:
    Acceptor(QObject *parent=nullptr);
    void startProcessing();

public slots:
    void checkData();
    void dataReceived(QByteArray data, DataType tp);

    void errorReceived(QNetworkReply::NetworkError error,
                       QString errorString);
signals:
    void scheduleNextCheck();
    void dataReady();

    void rqAnonymousToken(QNetworkRequest rq, QByteArray payload);
    void rqAccessToken(QNetworkRequest rq, QByteArray payload);
    void rqProxyAccessToken(QNetworkRequest rq, QByteArray payload);
    void rqAutoserverLocation(QNetworkRequest rq, QByteArray payload);

    void data_error(QJsonParseError err,
                       QString errorString);

};
#endif // ACCEPTOR_H
