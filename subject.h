#ifndef SUBJECT_H
#define SUBJECT_H

#include <QNetworkReply>
#include <QObject>
#include <mutex>
#include <atomic>

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
    std::atomic_bool in_progress_=false;
    const DataType dataType_;

private slots:
    void finishRequest();
public:
    Subject(DataType tp, QObject* parent = nullptr);

    bool ready();

    void setReady(volatile bool r);

    const QByteArray data() const;

    void setReply(QNetworkReply* reply);

    inline bool in_progress() const;
    inline void setIn_progress(bool newIn_progress);

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
