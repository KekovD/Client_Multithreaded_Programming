#include "TestWebSocketClient.h"
#include <QTest>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    int status = 0;
    TestWebSocketClient test1;
    status |= QTest::qExec(&test1, argc, argv);

    return status;
}