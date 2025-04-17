#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets>

class WebSocketClient final : public QObject {
    Q_OBJECT

public:
    bool isConnected() const { return connectionActive; }
    int keepAliveInterval() const { return keepaliveTimer.interval(); }

    explicit WebSocketClient(QObject* parent = nullptr);
    void EstablishConnection(const QString& host, const QString& port);
    void Transmit(const QString& content);
    void MaintainConnection();
    void ConfirmActive();
    void HandleConnect();
    void HandleDisconnect();

    signals:
        void DataReceived(const QString& content);
        void ConnectionEstablished();
        void ConnectionLost();
        void errorOccurred(const QString& error);

private:
    QWebSocket webSocket;
    QTimer keepaliveTimer;
    bool connectionActive{false};
};

#endif // WEBSOCKETCLIENT_H