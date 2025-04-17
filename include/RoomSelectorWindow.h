#ifndef ROOMSELECTORWINDOW_H
#define ROOMSELECTORWINDOW_H

#include <QWidget>
#include "WebSocketClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RoomSelectorWindow; }
QT_END_NAMESPACE

class RoomSelectorWindow final : public QWidget {
    Q_OBJECT

public:
    explicit RoomSelectorWindow(WebSocketClient& connection, QWidget* parent = nullptr);
    ~RoomSelectorWindow() override;

    public slots:
        void HandleSuccessfulConnection();
        void HandleConnectionLoss();

    signals:
        void RoomEntered();

private:
    Ui::RoomSelectorWindow* ui;
    WebSocketClient& socketLink;

    void ProcessAvailableRooms(const QString& roomsData);
    void ProcessRoomDetails(const QString& roomData);
    void ExecuteJoinProcedure();
    void ExecuteCreationProcedure();
};

#endif // ROOMSELECTORWINDOW_H