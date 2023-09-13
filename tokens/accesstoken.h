#ifndef ACCESSTOKEN_H
#define ACCESSTOKEN_H

#include "globals.h"
#include "tokenattributes.h"
#include <deque>

class AccessToken : public QObject, public SerializableReadable<AccessTokenData>
{
    Q_OBJECT
    AccessTokenData data_;
protected:
    void readCommonValues(const QJsonObject &source);
public:
    explicit AccessToken(QObject *parent = nullptr);

    // Serializable interface
public:
    void deserialize(const QJsonObject &source) override;
    bool isValid() override;
    void clear() override;
};

#endif // ACCESSTOKEN_H
