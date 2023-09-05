#include "workers.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Worker w;
    Initiator init;

    QObject::connect(&init, &Initiator::subjectUpdateRequested,
                     &w, &Worker::process);

    init.getData();

    return a.exec();
}
