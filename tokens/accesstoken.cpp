#include "accesstoken.h"
#include <QJsonArray>

void AccessToken::readCommonValues(const QJsonObject& source)
{
    readValue(source, js_value, data_.value);
    readValue(source, js_creationTime, data_.creationTime);
    readValue(source, js_expirationTime, data_.expirationTime);
    readValue(source, js_owner, data_.owner);
    readValue(source, js_features, data_.features);
    readValue(source, js_locations, data_.locations);
    readValue(source, js_package, data_.package);
    readValue(source, js_expired, data_.expired);
}

AccessToken::AccessToken(QObject *parent):
    QObject(parent)
{
    data_.tokenType=TokenType::LOCATION_ACCESS;
}

void AccessToken::deserialize(const QJsonObject &source)
{
    std::lock_guard _(mu);

    if(!(source.contains(js_type) && source[js_type]=="accs"))
        throw(JsonParseEx(JsonSerializationErrorType::WRONG_TOKEN_TYPE, js_type));

    data_.read(source);
}

bool AccessToken::isValid()
{
    std::lock_guard _(mu);

    return data_.validate();
}

void AccessToken::clear()
{
    std::lock_guard _(mu);

    data_.clear();
}
