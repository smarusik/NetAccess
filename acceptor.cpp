#include "acceptor.h"

Acceptor::Acceptor(QObject *parent):
    anonymousToken_(new AccessTokenBase),
    accessToken_(new AccessToken),
    proxyAccessToken_(new ProxyAccessToken),
    autoserverLocation_(new AutoserverLocation),
    machine_id(QSysInfo::machineUniqueId())
{
    moveToThread(&th);
    connect(this, &Acceptor::scheduleNextCheck,
            this, &Acceptor::checkData,
            Qt::QueuedConnection);
    th.start();
}

void Acceptor::startProcessing()
{
    emit scheduleNextCheck();
}

void Acceptor::checkData()
{
    if(!anonymousToken_->isValid())
    {
        QNetworkRequest rq;
        rq.setUrl(QUrl(QString(auth_url)+
                       anonym_auth_prefix+'/'
                       +app_type+anonym_auth_suffix));

        rq.setHeader(QNetworkRequest::ContentTypeHeader, json_content_header);

        QJsonObject rootObj;
        rootObj["externalId"]= QString(machine_id);
        QJsonObject clientAppObj;
        clientAppObj["name"]=app_type;
        rootObj["clientApp"]=clientAppObj;

        QJsonDocument payload;
        payload.setObject(rootObj);

        emit rqAnonymousToken(rq, payload.toJson());
    }
    else if(!accessToken_->isValid())
    {
        QNetworkRequest rq;
        rq.setUrl(QUrl(QString(auth_url)+auth_suffix));

        rq.setHeader(QNetworkRequest::ContentTypeHeader, json_content_header);
        rq.setRawHeader("Authorization",
                             QByteArray("Bearer ")+anonymousToken_->data().value.toUtf8());


        QJsonObject rootObj;
        rootObj["externalId"]= QString(machine_id);
        rootObj["type"]="accs";

        QJsonObject clientAppObj;
        clientAppObj["name"]=app_type;
        rootObj["clientApp"]=clientAppObj;

        QJsonDocument payload;
        payload.setObject(rootObj);

        emit rqAccessToken(rq, payload.toJson());
    }
    else if(!autoserverLocation_->isValid())
    {
        QNetworkRequest rq;
        rq.setUrl(QUrl(QString(locations_url)+autoserver_suffix));

        rq.setRawHeader("X-Client-App", app_type);
        rq.setRawHeader("Authorization",
                             QByteArray("Bearer ")+accessToken_->data().value.toUtf8());

        emit rqAutoserverLocation(rq, QByteArray());

    }
    else if (!proxyAccessToken_->isValid())
    {
        QNetworkRequest rq;
        rq.setUrl(QUrl(QString(auth_url)+proxy_auth_suffix));

        rq.setHeader(QNetworkRequest::ContentTypeHeader, json_content_header);
        rq.setRawHeader("Authorization",
                             QByteArray("Bearer ")+accessToken_->data().value.toUtf8());


        QJsonObject rootObj;
        rootObj["externalId"]= QString(machine_id);
        rootObj["type"]="accs-proxy";
        rootObj["signature"]=proxyAccessToken_->data().signature;

        QJsonObject clientAppObj;
        clientAppObj["name"]=app_type;
        rootObj["clientApp"]=clientAppObj;

        QJsonDocument payload;
        payload.setObject(rootObj);

        emit rqProxyAccessToken(rq, payload.toJson());
    }

    emit scheduleNextCheck();
}

void Acceptor::dataReceived(QByteArray data, DataType tp)
{
    qDebug()<<data;
    QJsonParseError err;
    QJsonDocument jsData=QJsonDocument::fromJson(data, &err);

    if(err.error!=QJsonParseError::NoError)
    {
        emit data_error(err, err.errorString());
    }

    try
    {
        switch (tp) {
        case DataType::ANONYMOUS_TOKEN:
            anonymousToken_->deserialize(jsData.object());
            break;
        case DataType::ACCESS_TOKEN:
            accessToken_->deserialize(jsData.object());
            break;
        case DataType::AUTOSERVER_LOCATION:
            autoserverLocation_->deserialize(jsData.object());
            break;
        case DataType::PROXY_ACCESS_TOKEN:
            proxyAccessToken_->deserialize(jsData.object());
            break;
        default:
            break;
        }
    }
    catch (JsonParseEx& e)
    {
        emit data_error(err, e.what());
    }
}

void Acceptor::errorReceived(QNetworkReply::NetworkError error,
                             QString errorString)
{
    qDebug()<<errorString<<error;
}
