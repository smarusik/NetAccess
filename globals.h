#ifndef GLOBALS_H
#define GLOBALS_H

#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QException>
#include <deque>
#include <mutex>

//constexpr char app_type[] = "ONECLICK_VPN_WINDOWS_APPLICATION";
constexpr char app_type[] = "ONECLICK_VPN_ANDROID_APPLICATION";
constexpr char json_content_header[] ="application/json";

constexpr char auth_url[]="https://api.1clickvpn.com/rest/v1";
constexpr char anonym_auth_prefix[]="/registrations/clientApps";
constexpr char anonym_auth_suffix[]="/users/anonymous";
constexpr char auth_suffix[]="/security/tokens/accs";
constexpr char proxy_auth_suffix[]="/security/tokens/accs-proxy";

constexpr char locations_url[]="https://stats.1clickvpn.com/api/rest/v2/entrypoints";
constexpr char autoserver_suffix[]="/autoserver";

constexpr char js_type[]="type";
constexpr char js_value[]="value";
constexpr char js_creationTime[]="creationTime";
constexpr char js_expirationTime[]="expirationTime";
constexpr char js_owner[]="owner";
constexpr char js_owner_id[]="id";
constexpr char js_expired[]="expired";
constexpr char js_clientApp[]="clientApp";
constexpr char js_clientApp_name[]="name";
constexpr char js_features[]="features";
constexpr char js_feature_key[]="key";
constexpr char js_locations[]="locations";
constexpr char js_location_country_code[]="countryCode";
constexpr char js_package[]="package";
constexpr char js_package_id[]="id";
constexpr char js_package_subscription_aware[]="subscriptionAware";

constexpr char js_server_accessType[]="accessType";
constexpr char js_server_name[]="name";
constexpr char js_server_group[]="group";
constexpr char js_server_type[]="type";
constexpr char js_server_address[]="address";
constexpr char js_server_address_primary[]="primary";
constexpr char js_server_address_host[]="host";
constexpr char js_server_address_port[]="port";
constexpr char js_server_weight[]="weight";
constexpr char js_server_signature[]="signature";

constexpr char js_country_code[]="code";
constexpr char js_country_code_iso2[]="iso2";
constexpr char js_country_code_iso3[]="iso3";
constexpr char js_country_title[]="title";
constexpr char js_country_continent[]="continent";
constexpr char js_country_subRegion[]="subRegion";
constexpr char js_country_intermediateRegion[]="intermediateRegion";

constexpr char js_autoserver_server[]="server";
constexpr char js_autoserver_country[]="country";

enum class TokenType{
    ANONYMOUS,
    LOCATION_ACCESS,
    PROXY_ACCESS
};

enum class AccessType{
    RANDOM,
    ACCESSIBLE,
    INACCESSIBLE
};
const std::map<QString, AccessType> accessTypeMapping =
{
    {"RANDOM", AccessType::RANDOM},
    {"ACCESSIBLE", AccessType::ACCESSIBLE},
    {"INACCESSIBLE", AccessType::INACCESSIBLE}
};

enum class ServerType{
    PROXY,
    VPN
};
const std::map<QString, ServerType> serverTypeMapping =
{
    {"PROXY", ServerType::PROXY},
    {"VPN", ServerType::VPN}
};

enum class JsonSerializationErrorType
{
    NO_ERROR,
    WRONG_FIELD,
    WRONG_UUID,
    UNMAPPED_VALUE,
    WRONG_TOKEN_TYPE
};

struct JsonSerializationError
{
    JsonSerializationError(const QString& jsField,
                           JsonSerializationErrorType errType):
        err(errType),
        jsField(jsField)
    {}

    JsonSerializationErrorType err;
    QString jsField;
};

class JsonParseEx:public QException
{
    JsonSerializationError error;
    char* what_;

    // exception interface
public:
    JsonParseEx(JsonSerializationErrorType errType, const QString &jsField):
        QException(),
        error(jsField, errType)
    {
        QString w(("Json parser error %1 '%2'"));
        QString descr;

        switch (errType) {
        case JsonSerializationErrorType::NO_ERROR:
            descr = "No error ";
            break;
        case JsonSerializationErrorType::WRONG_FIELD:
            descr = "Field not found ";
            break;
        case JsonSerializationErrorType::WRONG_UUID:
            descr = "Error converting value to UUID ";
            break;
        case JsonSerializationErrorType::UNMAPPED_VALUE:
            descr = "Error converting value to enum (unmapped) ";
            break;
        default:
            descr = "Unknown error ";
            break;
        }
        what_=w.arg("Wrong field", jsField).toUtf8().data();
    }

    virtual const char *what() const noexcept override
    {
        return what_;
    }

    // QException interface
public:
    virtual void raise() const override
    {
        throw(*this);
    }

    virtual QException *clone() const override
    {
        return new JsonParseEx(*this);
    }

    JsonSerializationError getError() const
    {
        return error;
    }
};

struct Serializable
{
    static void readValue(const QJsonObject& source, const QString& field, QString& result)
    {
        if(source.contains(field) && source.value(field).isString())
        {
            result = source[field].toString();
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    static void readValue(const QJsonObject& source, const QString& field, QUuid& result)
    {
        if(source.contains(field) && source.value(field).isString())
        {
            result = QUuid(source[field].toString());
            if(result.isNull())
                throw JsonParseEx(JsonSerializationErrorType::WRONG_UUID, field);
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    static void readValue(const QJsonObject& source, const QString& field, double& result)
    {
        if(source.contains(field) && source.value(field).isDouble())
        {
            result = source[field].toDouble();
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    static void readValue(const QJsonObject& source, const QString& field, uint& result)
    {
        if(source.contains(field) && source.value(field).isDouble())
        {
            result = source[field].toInt();
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    static void readValue(const QJsonObject& source, const QString& field, bool& result)
    {
        if(source.contains(field) && source.value(field).isBool())
        {
            result = source[field].toBool();
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    static void readValue(const QJsonObject& source, const QString& field, Serializable& result)
    {
        if(source.contains(field) && source.value(field).isObject())
        {
            result.deserialize(source[field].toObject());
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    template <typename T>
    static void readValue(const QJsonObject& source, const QString& field,
                   std::deque<T>& result)
    {
        if(source.contains(field) && source.value(field).isArray())
        {
            QJsonArray featuresArray =source.value(field).toArray();

            for(const auto &item:featuresArray)
            {
                T member;
                member.deserialize(item.toObject());
                result.emplace_back(member);
            }
        }
    }

    template <typename Enum>
    static void readValue(const QJsonObject& source, const QString& field,
                    const std::map<QString, Enum>& map,
                    Enum& result)
    {
        if(source.contains(field)
              && source[field].isString())
        {
            auto it=map.find(source.value(field).toString());

            if (it!=map.end())
            {
                result=it->second;
            }
            else
            {
                throw JsonParseEx(JsonSerializationErrorType::UNMAPPED_VALUE, field);
            };
        }
        else
            throw JsonParseEx(JsonSerializationErrorType::WRONG_FIELD, field);
    }

    virtual void deserialize(const QJsonObject &source)=0;
    virtual bool isValid()=0;
    virtual void clear()=0;
};

template <typename DT>
struct SerializableReadable:public Serializable
{
protected:
    DT data_;
    mutable std::mutex mu;
public:

    DT data()
    {
        std::lock_guard _(mu);
        return data_;
    }
};
#endif // GLOBALS_H
