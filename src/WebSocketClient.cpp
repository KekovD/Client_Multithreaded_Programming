#include "../include/WebSocketClient.h"

WebSocketClient::WebSocketClient(QObject* parent)
    : QObject(parent) {
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::DataReceived);
    connect(&webSocket, &QWebSocket::connected,
            this, &WebSocketClient::HandleConnect);
    connect(&webSocket, &QWebSocket::disconnected,
            this, &WebSocketClient::HandleDisconnect);
    connect(&webSocket, &QWebSocket::pong,
            this, &WebSocketClient::ConfirmActive);
    connect(&keepaliveTimer, &QTimer::timeout,
            this, &WebSocketClient::MaintainConnection);
}

void WebSocketClient::EstablishConnection(const QString& host, const QString& port) {
    QUrl endpoint;
    endpoint.setScheme("ws");
    endpoint.setHost(host);
    bool ok;
    const int portNum = port.toInt(&ok);

    if(!ok || portNum <= 0) {
        emit errorOccurred("Invalid port number");
        return;
    }
    endpoint.setPort(portNum);

    webSocket.open(endpoint);
}

void WebSocketClient::Transmit(const QString& content) {
    webSocket.sendTextMessage(content);
}

void WebSocketClient::MaintainConnection() {
    if(!connectionActive) webSocket.close();
    connectionActive = false;
    webSocket.ping();
}

void WebSocketClient::ConfirmActive() {
    connectionActive = true;
}

void WebSocketClient::HandleConnect() {
    connectionActive = true;
    keepaliveTimer.start(5000);
    emit ConnectionEstablished();
}

void WebSocketClient::HandleDisconnect() {
    keepaliveTimer.stop();
    emit ConnectionLost();
}