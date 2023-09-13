#ifndef PROXYACCESSTOKEN_H
#define PROXYACCESSTOKEN_H

#include "globals.h"
#include "tokenattributes.h"

class ProxyAccessToken : public QObject,public SerializableReadable<ProxyAccessTokenData>
{
    Q_OBJECT
public:
    explicit ProxyAccessToken(QObject *parent = nullptr);

    // Serializable interface
public:
    void deserialize(const QJsonObject &source) override;
    bool isValid() override;
    void clear() override;

};

#endif // PROXYACCESSTOKEN_H
