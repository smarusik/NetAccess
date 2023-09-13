#include "autoserverlocation.h"

AutoserverLocation::AutoserverLocation(QObject *parent):
    QObject(parent)
{}

void AutoserverLocation::deserialize(const QJsonObject &source)
{
    std::lock_guard _(mu);

    readValue(source.value(js_autoserver_server).toObject(),
              js_autoserver_country,data_.country);

    data_.servers.clear();
    LocationServer srv;
    readValue(source, js_autoserver_server, srv);
    data_.servers.emplace_back(srv);
}

bool AutoserverLocation::isValid()
{
    std::lock_guard _(mu);

    return data_.country.isValid()
           && !data_.servers.empty()
           && data_.servers.at(0).isValid();
}

void AutoserverLocation::clear()
{
    std::lock_guard _(mu);

    data_.servers.clear();
    data_.country.clear();
}
