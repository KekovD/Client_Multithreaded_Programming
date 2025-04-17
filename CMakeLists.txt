cmake_minimum_required(VERSION 3.30)
project(qt_client)

add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        WebSockets
        REQUIRED)

add_executable(qt_client src/main.cpp
        src/WebSocketClient.cpp
        include/WebSocketClient.h
        src/ConnectionDialogWindow.cpp
        include/ConnectionDialogWindow.h
        src/RoomSelectorWindow.cpp
        include/RoomSelectorWindow.h
        src/ChatWindow.cpp
        include/ChatWindow.h
        include/CommandHandler.h
        include/ConnectionDialogWindowUi.h
        include/RoomSelectorWindowUi.h
        include/ChatWindowUi.h
        src/resources.qrc
)

target_link_libraries(qt_client
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::WebSockets
)