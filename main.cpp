#include "workers.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Emitter emitter=new Emitter;
    Acceptor acceptor=new Acceptor;

    QObject::connect(&emitter, &Emitter::emitData,
            &acceptor, &Acceptor::acceptData);

    emitter.run();

    return a.exec();
}
