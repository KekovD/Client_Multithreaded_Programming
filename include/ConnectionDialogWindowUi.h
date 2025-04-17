#ifndef SERVERCONNECTIONDIALOGWINDOWUI_H
#define SERVERCONNECTIONDIALOGWINDOWUI_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConnectionDialogWindow {
public:
    QGridLayout* mainLayout;
    QLineEdit* serverAddressInput;
    QLineEdit* portInput;
    QPushButton* connectButton;

    void setupUi(QWidget* window) {
        configureMainWindow(window);
        initializeComponents(window);
        setupLayoutStructure();
        configureElementsBehavior();
        translateInterface(window);
        QMetaObject::connectSlotsByName(window);
    }

private:
    void configureMainWindow(QWidget* window) {
        window->setObjectName("ServerConnection");
        window->resize(350, 180);

        QFont baseFont;
        baseFont.setPointSize(12);
        window->setFont(baseFont);
    }

    void initializeComponents(QWidget* parent) {
        mainLayout = new QGridLayout(parent);
        serverAddressInput = new QLineEdit(parent);
        portInput = new QLineEdit(parent);
        connectButton = new QPushButton(parent);
    }

    void setupLayoutStructure() {
        mainLayout->setVerticalSpacing(10);
        mainLayout->addWidget(serverAddressInput, 0, 0);
        mainLayout->addWidget(portInput, 1, 0);
        mainLayout->addWidget(connectButton, 2, 0);
    }

    void configureElementsBehavior() {
        configureInputField(serverAddressInput);
        configureInputField(portInput);
        configureActionButton(connectButton);
    }

    void configureInputField(QLineEdit* field) {
        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        policy.setHorizontalStretch(0);
        field->setSizePolicy(policy);
    }

    void configureActionButton(QPushButton* button) {
        QSizePolicy btnPolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        btnPolicy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
        button->setSizePolicy(btnPolicy);
    }

    void translateInterface(QWidget* window) {
        window->setWindowTitle(QCoreApplication::translate("ServerConnect", "Server Connection"));
        serverAddressInput->setPlaceholderText(QCoreApplication::translate("ServerConnect", "Server Address"));
        portInput->setPlaceholderText(QCoreApplication::translate("ServerConnect", "Port Number"));
        connectButton->setText(QCoreApplication::translate("ServerConnect", "Connect to Server"));
    }
};

namespace Ui {
    class ConnectionDialogWindow: public Ui_ConnectionDialogWindow {};
}

QT_END_NAMESPACE

#endif // SERVERCONNECTIONDIALOGWINDOWUI_H