#ifndef LOCATIONATTRIBUTES_H
#define LOCATIONATTRIBUTES_H

#include "globals.h"

struct AddressDescription : public Serializable {
    QString host;
    uint port;

    // Serializable interface
public:
    virtual void deserialize(const QJsonObject &source) override
    {
        readValue(source, js_server_address_host, host);
        readValue(source, js_server_address_port, port);
    }
    virtual bool isValid() override
    {
        return !host.isEmpty();
    }
    virtual void clear() override
    {
        host=QString();
        port=0;
    }
};

struct ServerAddress : public Serializable
{
    AddressDescription primary;
    // Serializable interface
public:
    void deserialize(const QJsonObject &source) override
    {
        readValue(source, js_server_address_primary, primary);
    }

    bool isValid() override
    {
        return primary.isValid();
    }

    void clear() override
    {
        primary.clear();
    }
};

struct CountryCode:public Serializable
{
    QString iso2, iso3;

    // Serializable interface
public:
    virtual void deserialize(const QJsonObject &source) override
    {
        readValue(source, js_country_code_iso2, iso2);
        readValue(source, js_country_code_iso3, iso3);
    }
    virtual bool isValid() override
    {
        return !(iso2.isEmpty() || iso3.isEmpty());
    }
    virtual void clear() override
    {
        iso2=QString();
        iso3=QString();
    }
};

struct LocationCountry:public Serializable
{
    CountryCode code;
    QString title;
    QString continent;
    QString subRegion;
    QString intermediateRegion;

    // Serializable interface
public:
    void deserialize(const QJsonObject &source)
    {
        readValue(source, js_country_code, code);
        readValue(source, js_country_title, title);
        readValue(source, js_country_continent, continent);
        readValue(source, js_country_subRegion, subRegion);
        readValue(source, js_country_intermediateRegion, intermediateRegion);
    }

    bool isValid()
    {
        return code.isValid();
    }
    void clear()
    {
        code.clear();
    }
};

struct LocationServer : public Serializable
{
    AccessType accessType;
    QString name;
    QString group;
    ServerType serverType;
    ServerAddress address;
    uint weight;
    QString signature;

public:
    LocationServer(){}

    // Serializable interface
public:
    virtual void deserialize(const QJsonObject &source) override
    {
        readValue(source, js_server_accessType, accessTypeMapping, accessType);
        readValue(source, js_server_name, name);
        readValue(source, js_server_group, group);
        readValue(source, js_server_type, serverTypeMapping, serverType);
        readValue(source, js_server_address, address);
        readValue(source, js_server_weight, weight);
        readValue(source, js_server_signature, signature);
    }

    virtual bool isValid() override
    {
        return true;
    }

    virtual void clear() override
    {
        return;
    }
};

struct CountryLocationData
{
    LocationCountry country;
    std::deque<LocationServer> servers;
};

struct LocationData
{
    std::deque<CountryLocationData> locations;
};

#endif // LOCATIONATTRIBUTES_H
