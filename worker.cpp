#include "worker.h"

Worker::Worker(QObject *parent):
    nam(new QNetworkAccessManager(this))
{
    moveToThread(&th);
    th.start();
}

void Worker::process(Subject *subject,
                     QNetworkRequest rq, QByteArray payload)
{
    subject->setReady(false);
    (*subject)(nam,rq,payload);
}

void Worker::quit()
{
    th.quit();
    th.wait();
}
