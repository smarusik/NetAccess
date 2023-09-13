#include "proxyaccesstoken.h"

ProxyAccessToken::ProxyAccessToken(QObject *parent)
    : QObject(parent)
{}

void ProxyAccessToken::deserialize(const QJsonObject &source)
{
    std::lock_guard _(mu);

    if(!(source.contains(js_type) && source[js_type]=="accs-proxy"))
        throw(JsonParseEx(JsonSerializationErrorType::WRONG_TOKEN_TYPE,
                          js_type));

    data_.read(source);
}

bool ProxyAccessToken::isValid()
{
    std::lock_guard _(mu);

    return data_.validate();
}

void ProxyAccessToken::clear()
{
    std::lock_guard _(mu);

    data_.clear();
}
