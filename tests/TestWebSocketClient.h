#ifndef TESTWEBSOCKETCLIENT_H
#define TESTWEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QtTest>
#include "../include/WebSocketClient.h"

class TestWebSocketClient : public QObject
{
    Q_OBJECT

public:
    explicit TestWebSocketClient(QObject* parent = nullptr);
    ~TestWebSocketClient() override;

    private slots:
        void initialization();
        void dataTransmission();
        void keepAliveFunctionality();
        void cleanup();
        void testConnectionStateFlags();
        void testSendWhenDisconnected();
        void testKeepAliveTimerInterval();

private:
    QWebSocketServer* testServer;
    WebSocketClient* client;
    QWebSocket* serverSocket;
    int testPort = 0;
    const int waitTimeout = 1000;
};

#endif // TESTWEBSOCKETCLIENT_H