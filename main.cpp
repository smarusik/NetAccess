#include "workers.h"
#include "acceptor.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Worker w;
    Initiator init;
    Acceptor acc;

    QObject::connect(&init, &Initiator::subjectUpdateRequested,
                     &w, &Worker::process);

    QObject::connect(&init, &Initiator::requestedDataReady,
                     &acc, &Acceptor::dataReceived);

    for(auto i=0; i<3; ++i)
        init.requestData();

    return a.exec();
}
