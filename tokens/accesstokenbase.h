#ifndef ACCESSTOKENBASE_H
#define ACCESSTOKENBASE_H

#include "globals.h"
#include "tokenattributes.h"

#include <QNetworkReply>
#include <QDateTime>
#include <memory>

class AccessTokenBase:public QObject, public SerializableReadable<AnonymousTokenData>
{
    Q_OBJECT
protected:

public:
    AccessTokenBase(QObject *parent=nullptr);
    virtual ~AccessTokenBase()
    {
        qDebug()<<"~AccessTokenBase";
    }

    //Serializeble, Requestable IF
    virtual void deserialize(const QJsonObject &source) override;
    bool isValid() override;
    void clear() override;
};

#endif // ACCESSTOKENBASE_H
