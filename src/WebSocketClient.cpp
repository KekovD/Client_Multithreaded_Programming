#include "../include/WebSocketClient.h"

WebSocketClient::WebSocketClient(QObject* parent)
    : QObject(parent) {}

void WebSocketClient::Initialize() {
    webSocket = new QWebSocket;
    keepaliveTimer = new QTimer(this);

    connect(webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::DataReceived);
    connect(webSocket, &QWebSocket::connected,
            this, &WebSocketClient::HandleConnect);
    connect(webSocket, &QWebSocket::disconnected,
            this, &WebSocketClient::HandleDisconnect);
    connect(webSocket, &QWebSocket::pong,
            this, &WebSocketClient::ConfirmActive);
    connect(keepaliveTimer, &QTimer::timeout,
            this, &WebSocketClient::MaintainConnection);

    connect(this, &WebSocketClient::establishConnectionRequested,
            this, &WebSocketClient::EstablishConnection, Qt::QueuedConnection);
    connect(this, &WebSocketClient::internalTransmit,
           this, &WebSocketClient::Transmit, Qt::QueuedConnection);
}


void WebSocketClient::EstablishConnection(const QString& host, const QString& port) {
    QUrl endpoint;
    endpoint.setScheme("ws");
    endpoint.setHost(host);
    bool ok;
    const int portNum = port.toInt(&ok);
    if (!ok || portNum <= 0) {
        emit errorOccurred("Invalid port number");
        return;
    }
    endpoint.setPort(portNum);
    webSocket->open(endpoint);
}

void WebSocketClient::Transmit(const QString& content) const {
    if(webSocket && webSocket->state() == QAbstractSocket::ConnectedState) {
        QMetaObject::invokeMethod(webSocket,
            [this, content]() {
                webSocket->sendTextMessage(content);
            },
            Qt::QueuedConnection
        );
    }
}

void WebSocketClient::ScheduleTransmit(const QString& content) {
    emit internalTransmit(content);
}

void WebSocketClient::MaintainConnection() {
    if(!connectionActive) webSocket->close();
    connectionActive = false;
    webSocket->ping();
}

void WebSocketClient::ConfirmActive() {
    connectionActive = true;
}

void WebSocketClient::HandleConnect() {
    connectionActive = true;
    QMetaObject::invokeMethod(keepaliveTimer, "start", Qt::QueuedConnection, Q_ARG(int, 5000));
    emit ConnectionEstablished();
}

void WebSocketClient::HandleDisconnect() {
    keepaliveTimer->stop();
    emit ConnectionLost();
}