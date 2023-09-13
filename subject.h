#ifndef SUBJECT_H
#define SUBJECT_H

#include <QNetworkReply>
#include <QObject>
#include <mutex>

enum class DataType
{
    ANONYMOUS_TOKEN,
    ACCESS_TOKEN,
    PROXY_ACCESS_TOKEN,
    AUTOSERVER_LOCATION,
    ALL_LOCATIONS,
    CA_CERT_KEY
};
Q_DECLARE_METATYPE(DataType)

class Subject:public QObject
{
    Q_OBJECT

    mutable std::mutex mu_;
    QByteArray data_;
    QNetworkReply *reply_;
    volatile bool ready_=false;
    volatile bool in_progress_=false;
    const DataType dataType_;

private slots:
    void finishRequest();
public:
    Subject(DataType tp, QObject* parent = nullptr);

    bool ready();

    void setReady(volatile bool r);

    const QByteArray data() const;

    void setReply(QNetworkReply* reply);

    volatile bool in_progress() const;
    void setIn_progress(volatile bool newIn_progress);

    void operator()(QNetworkAccessManager* nm,
                    QNetworkRequest request,
                    QByteArray payload);

public slots:
    void processData();
signals:
    void data_ready(QByteArray data, DataType tp);
    void error_occured(QNetworkReply::NetworkError error,
                       QString error_string);
};
#endif // SUBJECT_H
