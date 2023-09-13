#include "accesstokenbase.h"

void AccessTokenBase::deserialize(const QJsonObject &source)
{
    std::lock_guard _(mu);

    if(!(source.contains(js_type) && source[js_type]=="anonm"))
        throw(JsonParseEx(JsonSerializationErrorType::WRONG_TOKEN_TYPE,
                          js_type));

    data_.read(source);
}

bool AccessTokenBase::isValid()
{
    std::lock_guard _(mu);

    return data_.validate();
}

void AccessTokenBase::clear()
{
    std::lock_guard _(mu);

    data_.clear();
}

AccessTokenBase::AccessTokenBase(QObject *parent):
    QObject(parent)
{
}
