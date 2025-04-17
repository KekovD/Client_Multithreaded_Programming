cmake_minimum_required(VERSION 3.30)
project(tests)

set(CMAKE_CXX_STANDARD 23)
find_package(Qt6 COMPONENTS Core Gui Test Widgets Network WebSockets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

enable_testing(true)

add_executable(tests main.cpp
        TestSocketConnection.cpp
        TestWebSocketClient.h
        ../include/WebSocketClient.h
        ../include/CommandHandler.h
        ../src/WebSocketClient.cpp)

target_link_libraries(tests
        Qt::Core
        Qt::Gui
        Qt::Test
        Qt::Widgets
        Qt::Network
        Qt::WebSockets)