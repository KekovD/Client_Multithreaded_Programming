#ifndef ROOMSELECTORWINDOWUI_H
#define ROOMSELECTORWINDOWUI_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RoomSelectorWindow {
public:
    QLineEdit* userNameInput;
    QLineEdit* roomNameInput;
    QComboBox* roomSelector;
    QPushButton* joinButton;
    QPushButton* createButton;

    void setupUi(QWidget* window) {
        configureWindow(window);
        initializeComponents(window);
        setupLayoutStructure(window);
        configureElementsBehavior();
        translateUIElements(window);
        QMetaObject::connectSlotsByName(window);
    }

private:
    QGridLayout* mainLayout = nullptr;

    static void configureWindow(QWidget* window) {
        window->setObjectName("RoomSelection");
        window->resize(400, 250);
        QFont baseFont;
        baseFont.setPointSize(12);
        window->setFont(baseFont);
    }

    void initializeComponents(QWidget* parent) {
        mainLayout = new QGridLayout(parent);
        userNameInput = new QLineEdit(parent);
        roomNameInput = new QLineEdit(parent);
        roomSelector = new QComboBox(parent);
        joinButton = new QPushButton(parent);
        createButton = new QPushButton(parent);
    }

    void setupLayoutStructure(QWidget* parent) const {
        mainLayout->setVerticalSpacing(10);
        mainLayout->addWidget(userNameInput, 0, 0);
        mainLayout->addWidget(roomNameInput, 1, 0);
        mainLayout->addWidget(roomSelector, 2, 0);
        mainLayout->addWidget(joinButton, 3, 0);
        mainLayout->addWidget(createButton, 4, 0);
    }

    void configureElementsBehavior() {
        setupSizePolicy(userNameInput);
        setupSizePolicy(roomNameInput);
        setupComboBoxBehavior();
        setupButtonBehavior(joinButton);
        setupButtonBehavior(createButton);
    }

    static void setupSizePolicy(QWidget* element) {
        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        element->setSizePolicy(policy);
    }

    void setupComboBoxBehavior() const {
        roomSelector->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        QSizePolicy comboPolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        comboPolicy.setHeightForWidth(roomSelector->sizePolicy().hasHeightForWidth());
        roomSelector->setSizePolicy(comboPolicy);
    }

    static void setupButtonBehavior(QPushButton* button) {
        QSizePolicy btnPolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        btnPolicy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
        button->setSizePolicy(btnPolicy);
    }

    void translateUIElements(QWidget* window) const {
        window->setWindowTitle(QCoreApplication::translate("RoomSelect", "Room Selection"));
        userNameInput->setPlaceholderText(QCoreApplication::translate("RoomSelect", "Username"));
        roomNameInput->setPlaceholderText(QCoreApplication::translate("RoomSelect", "New Room Name"));
        roomSelector->setPlaceholderText(QCoreApplication::translate("RoomSelect", "Available Rooms"));
        joinButton->setText(QCoreApplication::translate("RoomSelect", "Join Room"));
        createButton->setText(QCoreApplication::translate("RoomSelect", "Create Room"));
    }
};

namespace Ui {
    class RoomSelectorWindow: public Ui_RoomSelectorWindow {};
}

QT_END_NAMESPACE

#endif // ROOMSELECTORWINDOWUI_H