#ifndef TOKENATTRIBUTES_H
#define TOKENATTRIBUTES_H

#include "globals.h"

class TokenOwner:public Serializable
{
    QUuid id;

public:

    //Serializeble IF
    virtual void deserialize(const QJsonObject &source) override
    {
        readValue(source, js_owner_id, id);
    }
    bool isValid() override
    {
        return !id.isNull();
    }
    void clear() override
    {
        id=QUuid();
    }
};

struct ClientApplication:public Serializable
{
    QString name;

    // Serializable interface
public:
    void deserialize(const QJsonObject &source)
    {
        readValue(source, js_clientApp_name, name);
    }

    bool isValid()
    {
        return !name.isEmpty();
    }

    void clear()
    {
        name=QString();
    }
};

struct Feature:public Serializable
{
    QString key;

    // Serializable interface
public:
    void deserialize(const QJsonObject &source)
    {
        readValue(source, js_feature_key,key);
    }

    bool isValid()
    {
        return !key.isEmpty();
    }

    void clear()
    {
        key=QString();
    }
};

struct Location:public Serializable
{
    QString countryCode;

    // Serializable interface
public:
    void deserialize(const QJsonObject &source)
    {
        readValue(source, js_location_country_code, countryCode);
    }

    bool isValid()
    {
        return true;
    }

    void clear()
    {
        return;
    }
};

struct Package:public Serializable
{
    QUuid id;
    bool subscriptionAware;

    // Serializable interface
public:
    void deserialize(const QJsonObject &source)
    {
        readValue(source, js_package_id, id);
        readValue(source, js_package_subscription_aware, subscriptionAware);
    }

    bool isValid()
    {
        return !id.isNull();
    }

    void clear()
    {
        id=QUuid();
    }
};

struct AnonymousTokenData
{
    TokenType tokenType = TokenType::ANONYMOUS;
    QString value=QString();
    double creationTime;
    TokenOwner owner;
    bool expired;

    virtual void read(const QJsonObject& source)
    {
        Serializable::readValue(source, js_value, value);
        Serializable::readValue(source, js_creationTime, creationTime);
        Serializable::readValue(source, js_owner, owner);
        Serializable::readValue(source, js_expired, expired);
    }

    virtual void clear()
    {
        creationTime=0;
        owner.clear();
        value=QString();
    }

    virtual bool validate()
    {
        return !value.isEmpty()
               && owner.isValid()
               && !expired;
    }

};

struct CommonAccessTokenData: public AnonymousTokenData
{
    double expirationTime;
    Package package;
    std::deque<Feature> features;
    std::deque<Location> locations;

    virtual void read(const QJsonObject& source) override
    {
        AnonymousTokenData::read(source);
        Serializable::readValue(source, js_expirationTime, expirationTime);
        Serializable::readValue(source, js_features, features);
        Serializable::readValue(source, js_locations, locations);
        Serializable::readValue(source, js_package, package);
    }

    virtual void clear() override
    {
        AnonymousTokenData::clear();

        expirationTime=0;
        creationTime=0;
        package.clear();

    }

    virtual bool validate() override
    {
        return AnonymousTokenData::validate()
        && package.isValid()
        && QDateTime::currentSecsSinceEpoch()<expirationTime;
    }
};

struct AccessTokenData : public CommonAccessTokenData
{
    AccessTokenData()
    {
        tokenType=TokenType::LOCATION_ACCESS;
    }

    ClientApplication clientApp;
    virtual void read(const QJsonObject& source) override
    {
        CommonAccessTokenData::read(source);
        Serializable::readValue(source, js_clientApp, clientApp);
    }

    virtual void clear() override
    {
        CommonAccessTokenData::clear();
        clientApp.clear();
    }

    virtual bool validate() override
    {
        return CommonAccessTokenData::validate()
               && clientApp.isValid();

    }
};

struct ProxyAccessTokenData: public CommonAccessTokenData
{
    QString signature;

    ProxyAccessTokenData()
    {
        tokenType=TokenType::PROXY_ACCESS;
    }

    virtual void read(const QJsonObject& source) override
    {
        CommonAccessTokenData::read(source);
    }

    virtual void clear() override
    {
        CommonAccessTokenData::clear();
    }

    virtual bool validate() override
    {
        return CommonAccessTokenData::validate();
    }
};

#endif // TOKENATTRIBUTES_H
