#ifndef AUTOSERVERLOCATION_H
#define AUTOSERVERLOCATION_H

#include "globals.h"
#include "locationattributes.h"

class AutoserverLocation : public QObject, public SerializableReadable<CountryLocationData>
{
    Q_OBJECT

public:
    AutoserverLocation(QObject* parent=nullptr);

    // Serializable interface
public:
    virtual void deserialize(const QJsonObject &source) override;

    virtual bool isValid() override;

    virtual void clear() override;

};

#endif // AUTOSERVERLOCATION_H
