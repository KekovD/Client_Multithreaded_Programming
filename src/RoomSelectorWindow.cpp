#include "../include/RoomSelectorWindow.h"
#include "../include/RoomSelectorWindowUi.h"
#include "../include/CommandHandler.h"
#include <QListView>

RoomSelectorWindow::RoomSelectorWindow(WebSocketClient* connection, QWidget* parent)
    : QWidget(parent),
      socketLink(connection),
      ui(new Ui::RoomSelectorWindow) {
    ui->setupUi(this);

    if(QFile style(":/styles/chatWindow.qss"); style.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(style.readAll());
        style.close();
    }

    ui->roomSelector->setView(new QListView);

    connect(ui->joinButton, &QPushButton::clicked,
            this, &RoomSelectorWindow::ExecuteJoinProcedure);
    connect(ui->createButton, &QPushButton::clicked,
            this, &RoomSelectorWindow::ExecuteCreationProcedure);
}

RoomSelectorWindow::~RoomSelectorWindow() {
    delete ui;
}

void RoomSelectorWindow::HandleSuccessfulConnection() {
    connect(socketLink, &WebSocketClient::DataReceived,
            this, &RoomSelectorWindow::ProcessAvailableRooms,
            Qt::QueuedConnection);
    show();
}

void RoomSelectorWindow::ProcessAvailableRooms(const QString& data) {
    const QJsonArray rooms = QJsonDocument::fromJson(data.toUtf8())
        .object()
        .value("rooms")
        .toArray();

    ui->roomSelector->clear();
    for(const auto& room : rooms)
        ui->roomSelector->addItem(room.toString());

    disconnect(socketLink, &WebSocketClient::DataReceived,
               this, &RoomSelectorWindow::ProcessAvailableRooms);
}

void RoomSelectorWindow::ExecuteJoinProcedure() {
    const QString user = ui->userNameInput->text().trimmed();
    const QString room = ui->roomSelector->currentText().trimmed();

    if(user.isEmpty() || room.isEmpty()) {
        qWarning() << "Invalid join parameters";
        return;
    }

    socketLink->ScheduleTransmit(CommandHandler("join", user, room).ToJson());
    connect(socketLink, &WebSocketClient::DataReceived,
            this, &RoomSelectorWindow::ProcessRoomDetails);
}

void RoomSelectorWindow::ExecuteCreationProcedure() {
    const QString user = ui->userNameInput->text().trimmed();
    const QString room = ui->roomNameInput->text().trimmed();

    if(user.isEmpty() || room.isEmpty()) {
        qWarning() << "Invalid create parameters";
        return;
    }

    socketLink->ScheduleTransmit(CommandHandler("create", user, room).ToJson());
    connect(socketLink, &WebSocketClient::DataReceived,
            this, &RoomSelectorWindow::ProcessRoomDetails);
}

void RoomSelectorWindow::ProcessRoomDetails() {
    disconnect(socketLink, &WebSocketClient::DataReceived,
               this, &RoomSelectorWindow::ProcessRoomDetails);
    hide();
    const QString username = ui->userNameInput->text().trimmed();
    emit RoomEntered(username);
}

void RoomSelectorWindow::HandleConnectionLoss() {
    hide();
}