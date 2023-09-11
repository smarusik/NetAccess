#ifndef INITIATOR_H
#define INITIATOR_H

#include "subject.h"

class Initiator: public QObject
{
    Q_OBJECT

    Subject *subject1_, *subject2_;
private slots:
    void subject1UpdateRequested(const QByteArray data);

    void subject2UpdateRequested(const QByteArray data);

    void collectRequestData(const QByteArray data);

    void reportError(QNetworkReply::NetworkError error,
                     QString errorString);

public:
    Initiator(QObject *parent=nullptr);

    void requestData();

signals:
    void subjectUpdateRequested(Subject*,
                                QNetworkRequest rq);
    void requestedDataReady(QByteArray data);
    void dataRequestError(QString description);
};

#endif // INITIATOR_H
