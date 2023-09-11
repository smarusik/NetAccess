#ifndef SUBJECT_H
#define SUBJECT_H

#include <QNetworkReply>
#include <QObject>
#include <mutex>


class Subject:public QObject
{
    mutable std::mutex mu_;
    const int id;
    QByteArray data_;
    QNetworkReply *reply_;
    volatile bool ready_=false;
    volatile bool in_progress_=false;
    const QString url_="https://geo.geosurf.io";

    Q_OBJECT

private slots:
    void finishRequest();
public:
    Subject(int id, QObject* parent = nullptr);

    bool ready();

    void setReady(volatile bool r);

    const QByteArray data() const;

    void setReply(QNetworkReply* reply);

    volatile bool in_progress() const;
    void setIn_progress(volatile bool newIn_progress);

    void operator()(QNetworkAccessManager* nm,
                    QNetworkRequest request);

    virtual const QString &url();

public slots:
    void processData();
signals:
    void data_ready(QByteArray data);
    void error_occured(QNetworkReply::NetworkError error,
                       QString error_string);
};
#endif // SUBJECT_H
