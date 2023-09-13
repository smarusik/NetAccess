#include "worker.h"
#include "acceptor.h"
#include "subject.h"
#include "initiator.h"
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

    QObject::connect(&init, &Initiator::reportError,
                     &acc, &Acceptor::errorReceived);

    QObject::connect(&acc, &Acceptor::rqAnonymousToken,
                     &init, &Initiator::anonymousTokenRq);
    QObject::connect(&acc, &Acceptor::rqAccessToken,
                     &init, &Initiator::accessTokenRq);
    QObject::connect(&acc, &Acceptor::rqAutoserverLocation,
                     &init, &Initiator::autoserverLocationRq);
    QObject::connect(&acc, &Acceptor::rqProxyAccessToken,
                     &init, &Initiator::proxyAccessTokenRq);

    acc.startProcessing();
    return a.exec();
}
