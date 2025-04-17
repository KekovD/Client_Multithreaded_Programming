#ifndef SERVERCONNECTIONDIALOGWINDOW_H
#define SERVERCONNECTIONDIALOGWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectionDialogWindow; }
QT_END_NAMESPACE

class ConnectionDialogWindow final : public QWidget {
    Q_OBJECT

public:
    explicit ConnectionDialogWindow(QWidget* parent = nullptr);
    ~ConnectionDialogWindow() override;

    signals:
        void ConnectionRequested(const QString& host, const QString& port);

    public slots:
        void HandleConnectionEstablished();
    void HandleNetworkDisconnect();

    private slots:
        void ProcessConnectionAttempt();

private:
    Ui::ConnectionDialogWindow* ui;
};

#endif // SERVERCONNECTIONDIALOGWINDOW_H