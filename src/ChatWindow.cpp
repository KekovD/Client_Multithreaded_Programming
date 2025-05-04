#include "../include/ChatWindow.h"
#include "../include/ChatWindowUi.h"
#include "../include/CommandHandler.h"

ChatWindow::ChatWindow(WebSocketClient* connection, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ChatWindow),
      socketConnection(connection) {
    ui->setupUi(this);

    setMinimumWidth(520);

    if(QFile style(":/styles/roomSelectorWindow.qss"); style.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setStyleSheet(style.readAll());
        style.close();
    }

    ui->messageList->setItemDelegate(new MessageDelegate(ui->messageList, this));

    connect(ui->sendButton, &QPushButton::clicked,
                     this, &ChatWindow::TransmitMessage);
    connect(ui->messageInput, &QLineEdit::returnPressed,
                     this, &ChatWindow::TransmitMessage);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::InitializeRoom(const QString& username) {
    currentUsername = username;
    ui->messageList->clear();

    disconnect(socketConnection, &WebSocketClient::DataReceived,
              this, &ChatWindow::ProcessIncomingMessage);

    connect(socketConnection, &WebSocketClient::DataReceived,
            this, &ChatWindow::ProcessIncomingMessage,
            Qt::UniqueConnection);

    show();
}

void ChatWindow::TransmitMessage() const {
    const QString content = ui->messageInput->text().trimmed();
    if (content.isEmpty()) return;

    QJsonObject messageObj;
    messageObj["user"] = currentUsername;
    messageObj["message"] = content;
    messageObj["clientKey"] = clientKey;
    const QJsonDocument doc(messageObj);
    const QString jsonMessage = doc.toJson(QJsonDocument::Compact);

    ui->messageInput->clear();
    socketConnection->ScheduleTransmit(jsonMessage);
}

void ChatWindow::ProcessIncomingMessage(const QString& message) const {
    if (CommandHandler cmd; CommandHandler::TryParse(message, cmd) && cmd.operation() == "join") {
        auto* item = new QListWidgetItem(CommandHandler::FormatJoinMessage(cmd.userName()));
        item->setTextAlignment(Qt::AlignCenter);
        AddMessageItem(item);
        return;
    }

    if (const QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8()); !doc.isNull() && doc.isObject()) {
        if (QJsonObject obj = doc.object(); obj.contains("user") && obj.contains("message")) {
            const QString user = obj["user"].toString();
            const QString msg = obj["message"].toString();
            const QString client = obj["clientKey"].toString();

            QListWidgetItem* item;
            if (client == clientKey) {
                item = new QListWidgetItem(QString("%1 [%2]")
                    .arg(msg, QDateTime::currentDateTime().toString("hh:mm:ss")));
                item->setTextAlignment(Qt::AlignRight);
            } else {
                item = new QListWidgetItem(QString("[%1] <%2> %3")
                    .arg(QDateTime::currentDateTime().toString("hh:mm:ss"), user, msg));
                item->setTextAlignment(Qt::AlignLeft);
            }

            AddMessageItem(item);
            return;
        }
    }

    auto* item = new QListWidgetItem(message);
    item->setTextAlignment(Qt::AlignLeft);
    AddMessageItem(item);
}

void ChatWindow::HandleDisconnect() {
    disconnect(socketConnection, &WebSocketClient::DataReceived,
                        this, &ChatWindow::ProcessIncomingMessage);
    hide();
}

QString ChatWindow::GenerateRoomKey() const {
    QString randomString = QUuid::createUuid().toString(QUuid::WithoutBraces);
    randomString.remove('-');
    const int randomNum = QRandomGenerator::global()->generate();

    const QString rawData = QString("%1|%2|%3")
        .arg(currentUsername, randomString)
        .arg(randomNum);

    const QByteArray hash = QCryptographicHash::hash(
        rawData.toUtf8(),
        QCryptographicHash::Sha256
    );

    return QString(hash.toHex());
}

void ChatWindow::AddMessageItem(QListWidgetItem* item) const {
    const bool wasAtBottom = ui->messageList->verticalScrollBar()->value() ==
                      ui->messageList->verticalScrollBar()->maximum();

    ui->messageList->addItem(item);

    if (wasAtBottom) {
        ui->messageList->scrollToBottom();
    }
}

void MessageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    painter->save();

    QTextDocument doc;
    doc.setDocumentMargin(5);
    doc.setPlainText(opt.text);

    const qreal viewportWidth = listWidget->viewport()->width();
    const qreal maxWidth = (viewportWidth / 2) - 15;

    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    textOption.setAlignment(opt.displayAlignment);

    if(opt.displayAlignment & Qt::AlignCenter) {
        doc.setTextWidth(viewportWidth - 10);
    } else {
        doc.setTextWidth(maxWidth);
    }

    doc.setDefaultTextOption(textOption);

    opt.text = "";
    opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    painter->translate(opt.rect.left(), opt.rect.top());
    if(opt.displayAlignment & Qt::AlignRight) {
        painter->translate(viewportWidth - maxWidth - 10, 0);
    }

    const QRect clip(0, 0, doc.textWidth(), opt.rect.height());
    doc.drawContents(painter, clip);

    painter->restore();
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem& option,
                                const QModelIndex& index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QTextDocument doc;
    doc.setDocumentMargin(5);
    doc.setPlainText(opt.text);

    const qreal viewportWidth = listWidget->viewport()->width();
    const qreal maxWidth = (viewportWidth / 2) - 15;

    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    textOption.setAlignment(opt.displayAlignment);

    if(opt.displayAlignment & Qt::AlignCenter) {
        doc.setTextWidth(viewportWidth - 10);
    } else {
        doc.setTextWidth(maxWidth);
    }

    doc.setDefaultTextOption(textOption);

    return QSize(doc.idealWidth(), doc.size().height());
}
