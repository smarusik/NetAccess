#include "worker.h"

Worker::Worker(QObject *parent):
    nam(new QNetworkAccessManager(this))
{
    moveToThread(&th);
    th.start();
}

void Worker::process(Subject *subject)
{
    subject->setReady(false);
    (*subject)(nam);
}

void Worker::quit()
{
    th.quit();
    th.wait();
}
