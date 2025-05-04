#include <QApplication>
#include <memory>
#include "../include/ConnectionDialogWindow.h"
#include "../include/RoomSelectorWindow.h"
#include "../include/ChatWindow.h"
#include "../include/WebSocketClient.h"

int main(int argc, char* argv[]) {
    const QApplication app{argc, argv};

    const auto socketThread = std::make_unique<QThread>();
    auto connection = new WebSocketClient();
    connection->moveToThread(socketThread.get());
    QObject::connect(socketThread.get(), &QThread::started, connection, &WebSocketClient::Initialize);
    socketThread->start();

    ConnectionDialogWindow serverWindow;
    const RoomSelectorWindow roomSelector(connection);
    const ChatWindow chatWindow(connection);

    serverWindow.show();

    QObject::connect(&serverWindow, &ConnectionDialogWindow::ConnectionRequested,
    [connection](const QString& host, const QString& port) {
        QMetaObject::invokeMethod(connection, "EstablishConnection",
            Qt::QueuedConnection,
            Q_ARG(QString, host),
            Q_ARG(QString, port));
        }
    );

    QObject::connect(connection, &WebSocketClient::ConnectionEstablished,
                &serverWindow, &ConnectionDialogWindow::HandleConnectionEstablished);
    QObject::connect(connection, &WebSocketClient::ConnectionEstablished,
                     &roomSelector, &RoomSelectorWindow::HandleSuccessfulConnection);

    QObject::connect(connection, &WebSocketClient::ConnectionLost,
                     &serverWindow, &ConnectionDialogWindow::HandleNetworkDisconnect);
    QObject::connect(connection, &WebSocketClient::ConnectionLost,
                     &roomSelector, &RoomSelectorWindow::HandleConnectionLoss);
    QObject::connect(connection, &WebSocketClient::ConnectionLost,
                     &chatWindow, &ChatWindow::HandleDisconnect);

    QObject::connect(&roomSelector, &RoomSelectorWindow::RoomEntered,
                     &chatWindow, &ChatWindow::InitializeRoom);

    QObject::connect(&app, &QApplication::aboutToQuit, [&] {
        socketThread->quit();
        socketThread->wait();
    });

    return QApplication::exec();
}