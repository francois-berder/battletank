#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T18:35:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = battletank
TEMPLATE = app

QMAKE_CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Weffc++ -Wredundant-decls -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -Wno-unused-parameter

LIBS += -lsfml-graphics-s -lsfml-network-s -lsfml-window-s -lsfml-system-s  -lBox2D
unix {
     LIBS += -lX11 -lGLEW -ljpeg -lXrandr
 }

CONFIG(debug, debug|release): QMAKE_CXXFLAGS += -ggdb3
CONFIG(debug, debug|release): QMAKE_CXXFLAGS += -O2

INCLUDEPATH += \
    src\
    src/ui\
    src/core\
    src/network\
    src/simulation\
    src/view\
    src/utils

DEPENDPATH += \
    src\
    src/ui\
    src/core\
    src/network\
    src/simulation\
    src/view\
    src/utils

SOURCES += \
    src/main.cpp\
    src/ui/MainWindow.cpp\
    src/core/Game.cpp\
    src/network/Client.cpp\
    src/network/NetworkEvent.cpp\
    src/network/Server.cpp\
    src/simulation/Bullet.cpp\
    src/simulation/Change.cpp\
    src/simulation/CollidableEntity.cpp\
    src/simulation/ContactListener.cpp\
    src/simulation/Entity.cpp\
    src/simulation/EntityFactory.cpp\
    src/simulation/Explosion.cpp\
    src/simulation/GameWorld.cpp\
    src/simulation/Obstacle.cpp\
    src/simulation/PhysicWorld.cpp\
    src/simulation/Tank.cpp\
    src/utils/Logger.cpp\
    src/utils/StringBuffer.cpp\
    src/utils/Utils.cpp\
    src/utils/EntityParser.cpp\
    src/view/EntityViewer.cpp\
    src/utils/TextureManager.cpp\
    src/utils/Tree.cpp\
    src/view/View.cpp \
    src/ui/MainMenu.cpp \
    src/ui/PlayMenu.cpp \
    src/ui/JoinDialog.cpp \
    src/ui/ChatMenu.cpp \
    src/ui/Host.cpp \
    src/ui/Player.cpp \
    src/ui/GameData.cpp \
    src/ui/LaunchGameDialog.cpp \
    src/ui/GameMenu.cpp \
    src/ui/StatMenu.cpp

HEADERS  += \
    src/ui/MainWindow.hpp \
    src/core/Game.hpp \
    src/network/Client.hpp \
    src/network/NetworkEvent.hpp \
    src/network/Server.hpp \
    src/simulation/Bullet.hpp \
    src/simulation/Change.hpp \
    src/simulation/CollidableEntity.hpp \
    src/simulation/ContactListener.hpp \
    src/simulation/Entity.hpp \
    src/simulation/EntityFactory.hpp \
    src/simulation/Explosion.hpp \
    src/simulation/GameWorld.hpp \
    src/simulation/Obstacle.hpp \
    src/simulation/PhysicWorld.hpp \
    src/simulation/Tank.hpp \
    src/utils/Logger.hpp \
    src/utils/StringBuffer.hpp \
    src/utils/Utils.hpp \
    src/utils/EntityParser.hpp \
    src/view/EntityViewer.hpp \
    src/view/Event.hpp \
    src/utils/TextureManager.hpp \
    src/utils/Tree.hpp \
    src/view/View.hpp \
    src/ui/MainMenu.hpp \
    src/ui/PlayMenu.hpp \
    src/ui/Menu.hpp \
    src/ui/JoinDialog.hpp \
    src/ui/ChatMenu.hpp \
    src/ui/Host.hpp \
    src/ui/Player.hpp \
    src/ui/GameData.hpp \
    src/ui/LaunchGameDialog.hpp \
    src/ui/GameMenu.hpp \
    src/ui/StatMenu.hpp

FORMS += \
    ui/MainMenu.ui \
    ui/PlayMenu.ui \
    ui/JoinDialog.ui \
    ui/ChatMenu.ui \
    ui/LaunchGameDialog.ui \
    ui/StatMenu.ui
