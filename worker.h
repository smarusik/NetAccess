#ifndef WORKER_H
#define WORKER_H

#include "subject.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QObject>
#include <mutex>

class Worker : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *nam;
    QThread th;

public:
    Worker(QObject* parent=nullptr);

public slots:
    void process(Subject* subject, QNetworkRequest rq);

    void quit();
};

#endif // WORKER_H
