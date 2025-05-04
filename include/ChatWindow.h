#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QScrollBar>
#include <QtWidgets/qlistwidget.h>
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QPainter>
#include <QTextOption>
#include <QTextDocument>
#include "WebSocketClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow final : public QWidget {
    Q_OBJECT

public:
    explicit ChatWindow(WebSocketClient& connection, QWidget* parent = nullptr);
    ~ChatWindow() override;

    void InitializeRoom(const QString& username);

    public slots:
        void HandleDisconnect();

    private slots:
        void TransmitMessage() const;
        void ProcessIncomingMessage(const QString& message) const;

private:
    void AddMessageItem(QListWidgetItem* item) const;
    QString GenerateRoomKey() const;

    QString currentUsername;
    QString clientKey = GenerateRoomKey();
    Ui::ChatWindow* ui;
    WebSocketClient& socketConnection;
};

class MessageDelegate : public QStyledItemDelegate {
    QListWidget* listWidget;

public:
    explicit MessageDelegate(QListWidget* list, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), listWidget(list) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};


#endif // CHATWINDOW_H