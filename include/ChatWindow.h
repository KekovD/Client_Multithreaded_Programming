#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "WebSocketClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow final : public QWidget {
    Q_OBJECT

public:
    explicit ChatWindow(WebSocketClient& connection, QWidget* parent = nullptr);
    ~ChatWindow() override;

    void InitializeRoom();

    int sentMessagesCount() const { return sentCount; }

    public slots:
        void HandleDisconnect();

    private slots:
        void TransmitMessage();
    void ProcessIncomingMessage(const QString& message) const;

private:
    int sentCount = 0;
    Ui::ChatWindow* ui;
    WebSocketClient& socketConnection;
};

#endif // CHATWINDOW_H