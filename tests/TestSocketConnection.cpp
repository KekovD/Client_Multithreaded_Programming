#include "TestWebSocketClient.h"
#include <QTest>
#include <QSignalSpy>

TestWebSocketClient::TestWebSocketClient(QObject* parent)
    : QObject(parent),
      testServer(new QWebSocketServer("TestServer", QWebSocketServer::NonSecureMode)),
      client(new WebSocketClient(this)),
      serverSocket(nullptr)
{
    testServer->listen(QHostAddress::LocalHost);
    testPort = testServer->serverPort();
}

TestWebSocketClient::~TestWebSocketClient()
{
    delete testServer;
    delete client;
}

void TestWebSocketClient::initialization()
{
    QVERIFY(testServer->isListening());
    QCOMPARE(client->parent(), this);
}

void TestWebSocketClient::dataTransmission()
{
    QSignalSpy connectSpy(client, &WebSocketClient::ConnectionEstablished);
    QSignalSpy dataSpy(client, &WebSocketClient::DataReceived);

    client->EstablishConnection("localhost", "8080");
    QVERIFY(connectSpy.wait(5000));
    QVERIFY(dataSpy.wait(5000));

    QVERIFY(dataSpy.first().first().toString().contains("rooms"));
}

void TestWebSocketClient::keepAliveFunctionality()
{
    client->EstablishConnection("localhost", QString::number(testPort));

    testServer->close();
    QSignalSpy disconnectSpy(client, &WebSocketClient::ConnectionLost);
    QVERIFY(disconnectSpy.wait(15000));
}

void TestWebSocketClient::cleanup()
{
    if(testServer->isListening())
        testServer->close();
    if(client)
        client->disconnect();
}

void TestWebSocketClient::testSendWhenDisconnected()
{
    QSignalSpy spy(client, &WebSocketClient::errorOccurred);

    client->Transmit("Test message");
    QVERIFY(!spy.wait(waitTimeout));
}

void TestWebSocketClient::testKeepAliveTimerInterval()
{
    QSignalSpy connectSpy(client, &WebSocketClient::ConnectionEstablished);

    client->EstablishConnection("localhost", QString::number(testPort));
    QVERIFY(!connectSpy.wait(waitTimeout));
}

void TestWebSocketClient::testConnectionStateFlags()
{
    QSignalSpy connectSpy(client, &WebSocketClient::ConnectionEstablished);

    client->EstablishConnection("localhost", QString::number(testPort));
    serverSocket = testServer->nextPendingConnection();
    QVERIFY(!connectSpy.wait(waitTimeout));
}
