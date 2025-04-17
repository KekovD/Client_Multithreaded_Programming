#include "../include/ConnectionDialogWindow.h"
#include "../include/ConnectionDialogWindowUi.h"
#include <QFile>
#include <QKeyEvent>

ConnectionDialogWindow::ConnectionDialogWindow(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ConnectionDialogWindow) {
    ui->setupUi(this);

    if(QFile style(":/styles/connectionDialogWindow.qss"); style.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(style.readAll());
        style.close();
    }

    connect(ui->connectButton, &QPushButton::clicked,
            this, &ConnectionDialogWindow::ProcessConnectionAttempt);
    ui->connectButton->setShortcut(Qt::Key_Return);
    ui->connectButton->setShortcut(Qt::Key_Enter);
}

ConnectionDialogWindow::~ConnectionDialogWindow() {
    delete ui;
}

void ConnectionDialogWindow::ProcessConnectionAttempt() {
    emit ConnectionRequested(
        ui->serverAddressInput->text().trimmed(),
        ui->portInput->text().trimmed()
    );
}

void ConnectionDialogWindow::HandleConnectionEstablished() {
    hide();
}

void ConnectionDialogWindow::HandleNetworkDisconnect() {
    show();
}