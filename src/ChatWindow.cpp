#include "../include/ChatWindow.h"
#include "../include/ChatWindowUi.h"
#include "../include/CommandHandler.h"

ChatWindow::ChatWindow(WebSocketClient& connection, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ChatWindow),
      socketConnection(connection) {
    ui->setupUi(this);

    if(QFile style(":/styles/roomSelectorWindow.qss"); style.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setStyleSheet(style.readAll());
        style.close();
    }

    connect(ui->sendButton, &QPushButton::clicked,
                     this, &ChatWindow::TransmitMessage);
    connect(ui->messageInput, &QLineEdit::returnPressed,
                     this, &ChatWindow::TransmitMessage);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::InitializeRoom() {
    ui->messageList->clear();
    connect(&socketConnection, &WebSocketClient::DataReceived,
                     this, &ChatWindow::ProcessIncomingMessage);
    show();
}

void ChatWindow::TransmitMessage() const {
    const QString content = ui->messageInput->text().trimmed();
    if(content.isEmpty()) return;
    ui->messageInput->clear();
    socketConnection.Transmit(content);
}

void ChatWindow::ProcessIncomingMessage(const QString& message) const {
    if(CommandHandler cmd; CommandHandler::TryParse(message, cmd) && cmd.operation() == "join") {
        ui->messageList->addItem(CommandHandler::FormatJoinMessage(cmd.userName()));
        return;
    }
    ui->messageList->addItem(message);
}

void ChatWindow::HandleDisconnect() {
    disconnect(&socketConnection, &WebSocketClient::DataReceived,
                        this, &ChatWindow::ProcessIncomingMessage);
    hide();
}