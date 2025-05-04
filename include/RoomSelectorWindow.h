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
    explicit RoomSelectorWindow(WebSocketClient* connection, QWidget* parent = nullptr);
    ~RoomSelectorWindow() override;

    public slots:
        void HandleSuccessfulConnection();
        void HandleConnectionLoss();

    signals:
        void RoomEntered(const QString& username);

private:
    WebSocketClient* socketLink;
    Ui::RoomSelectorWindow* ui;

    void ProcessAvailableRooms(const QString& roomsData);
    void ProcessRoomDetails();
    void ExecuteJoinProcedure();
    void ExecuteCreationProcedure();
};

#endif // ROOMSELECTORWINDOW_H