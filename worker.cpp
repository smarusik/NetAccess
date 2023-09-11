#include "worker.h"

Worker::Worker(QObject *parent):
    nam(new QNetworkAccessManager(this))
{
    moveToThread(&th);
    th.start();
}

void Worker::process(Subject *subject,
                     QNetworkRequest rq)
{
    subject->setReady(false);
    (*subject)(nam,rq);
}

void Worker::quit()
{
    th.quit();
    th.wait();
}
