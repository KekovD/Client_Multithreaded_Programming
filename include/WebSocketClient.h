#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets>

class WebSocketClient final : public QObject {
    Q_OBJECT

public:
    explicit WebSocketClient(QObject* parent = nullptr);
    bool isConnected() const { return connectionActive; }
    void ScheduleTransmit(const QString& content);
    QTimer* keepaliveTimer = nullptr;
    QWebSocket* webSocket = nullptr;
    bool connectionActive{false};

    signals:
        void internalTransmit(const QString& content);
        void DataReceived(const QString& content);
        void ConnectionEstablished();
        void ConnectionLost();
        void errorOccurred(const QString& error);
        void establishConnectionRequested(const QString& host, const QString& port);

    public slots:
        void Initialize();
        void EstablishConnection(const QString& host, const QString& port);
        void Transmit(const QString& content) const;
        void MaintainConnection();
        void ConfirmActive();
        void HandleConnect();

    void HandleDisconnect();

// private:
};

#endif // WEBSOCKETCLIENT_H