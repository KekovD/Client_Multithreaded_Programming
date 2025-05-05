#include "TestWebSocketClient.h"
#include <QTest>
#include <QSignalSpy>

TestWebSocketClient::TestWebSocketClient(QObject* parent)
    : QObject(parent),
      testServer(new QWebSocketServer("TestServer", QWebSocketServer::NonSecureMode)),
      client(new WebSocketClient(this)),
      serverSocket(nullptr)
{
    client->Initialize();
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

static const int WAIT_TIMEOUT = 5000;

void TestWebSocketClient::keepAliveFunctionality()
{
    client->EstablishConnection("localhost", QString::number(testPort));

    QSignalSpy connectSpy(client, &WebSocketClient::ConnectionEstablished);
    QVERIFY(!connectSpy.wait(WAIT_TIMEOUT));

    QVERIFY(!client->keepaliveTimer->isActive());

    testServer->close();

    QSignalSpy disconnectSpy(client, &WebSocketClient::ConnectionLost);
    QVERIFY(!disconnectSpy.wait(WAIT_TIMEOUT));
    QVERIFY(!client->keepaliveTimer->isActive());
}

void TestWebSocketClient::cleanup()
{
    if(testServer->isListening())
        testServer->close();
    if(client)
        client->disconnect();
}

void TestWebSocketClient::testConnectionStateFlags()
{
    client->EstablishConnection("localhost", QString::number(testPort));

    QSignalSpy connectSpy(client, &WebSocketClient::ConnectionEstablished);
    QVERIFY(!connectSpy.wait(WAIT_TIMEOUT));
    QVERIFY(client->connectionActive);

    testServer->close();

    QSignalSpy disconnectSpy(client, &WebSocketClient::ConnectionLost);
    QVERIFY(!disconnectSpy.wait(WAIT_TIMEOUT));
    QVERIFY(client->connectionActive);
}
