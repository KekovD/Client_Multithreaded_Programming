#include <QApplication>
#include "../include/ConnectionDialogWindow.h"
#include "../include/RoomSelectorWindow.h"
#include "../include/ChatWindow.h"
#include "../include/WebSocketClient.h"

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    WebSocketClient connection;
    ConnectionDialogWindow serverWindow;
    const RoomSelectorWindow roomSelector(connection);
    const ChatWindow chatWindow(connection);

    serverWindow.show();

    QObject::connect(&connection, &WebSocketClient::ConnectionEstablished,
                     &serverWindow, &ConnectionDialogWindow::HandleConnectionEstablished);
    QObject::connect(&connection, &WebSocketClient::ConnectionEstablished,
                     &roomSelector, &RoomSelectorWindow::HandleSuccessfulConnection);

    QObject::connect(&connection, &WebSocketClient::ConnectionLost,
                     &serverWindow, &ConnectionDialogWindow::HandleNetworkDisconnect);
    QObject::connect(&connection, &WebSocketClient::ConnectionLost,
                     &roomSelector, &RoomSelectorWindow::HandleConnectionLoss);
    QObject::connect(&connection, &WebSocketClient::ConnectionLost,
                     &chatWindow, &ChatWindow::HandleDisconnect);

    QObject::connect(&serverWindow, &ConnectionDialogWindow::ConnectionRequested,
                     &connection, &WebSocketClient::EstablishConnection);

    QObject::connect(&roomSelector, &RoomSelectorWindow::RoomEntered,
                     &chatWindow, &ChatWindow::InitializeRoom);

    return QApplication::exec();
}