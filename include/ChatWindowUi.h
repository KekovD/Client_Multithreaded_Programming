#ifndef CHATWINDOWUI_H
#define CHATWINDOWUI_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
QT_BEGIN_NAMESPACE

class Ui_ChatWindow {
public:
    QListWidget* messageList;
    QLineEdit* messageInput;
    QPushButton* sendButton;

    void setupUi(QWidget* window) {
        configureWindow(window);
        initializeLayouts(window);
        setupMessageArea(window);
        setupInputComponents(window);
        applyLayoutSettings();
        translateUI(window);
        QMetaObject::connectSlotsByName(window);
    }

private:
    QGridLayout* mainLayout = nullptr;
    QHBoxLayout* inputLayout = nullptr;
    static void configureWindow(QWidget* window) {
        window->setObjectName("ChatWindow");
        window->resize(620, 440);

        QFont baseFont;
        baseFont.setPointSize(12);
        window->setFont(baseFont);
    }

    void initializeLayouts(QWidget* parent) {
        mainLayout = new QGridLayout(parent);
        mainLayout->setContentsMargins(5, 5, 5, 5);

        inputLayout = new QHBoxLayout();
        inputLayout->setSpacing(8);
    }

    void setupMessageArea(QWidget* parent) {
        messageList = new QListWidget(parent);
        messageList->setObjectName("MessageList");
        messageList->setMinimumHeight(300);
        mainLayout->addWidget(messageList, 0, 0);
    }

    void setupInputComponents(QWidget* parent) {
        messageInput = new QLineEdit(parent);
        messageInput->setObjectName("MessageInput");
        messageInput->setMinimumWidth(400);

        sendButton = new QPushButton(parent);
        sendButton->setObjectName("SendButton");
        sendButton->setFixedWidth(100);

        inputLayout->addWidget(messageInput);
        inputLayout->addWidget(sendButton);
        mainLayout->addLayout(inputLayout, 1, 0);
    }

    void applyLayoutSettings() const {
        mainLayout->setRowStretch(0, 8);
        mainLayout->setRowStretch(1, 1);
    }

    void translateUI(QWidget* window) const {
        window->setWindowTitle(QCoreApplication::translate("ChatRoom", "Chat"));
        messageInput->setPlaceholderText(QCoreApplication::translate("ChatRoom", "Type message..."));
        sendButton->setText(QCoreApplication::translate("ChatRoom", "Send"));
    }
};

namespace Ui {
    class ChatWindow : public Ui_ChatWindow {};
}

QT_END_NAMESPACE

#endif // CHATWINDOWUI_H