#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T18:35:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = battletank
TEMPLATE = app

QMAKE_CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Weffc++ -Wredundant-decls -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -Wno-unused-parameter -ggdb3

CONFIG(release, debug|release): LIBS += -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system -lBox2D
CONFIG(debug, debug|release): LIBS += -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system -lBox2D

INCLUDEPATH += src\
               src/ui\
               src/core\
               src/network\
               src/simulation\
               src/view\
               src/utils

DEPENDPATH += src\
              src/ui\
              src/core\
              src/network\
              src/simulation\
              src/view\
              src/utils

SOURCES += src/main.cpp\
           src/ui/MainWindow.cpp\
           src/core/Game.cpp\
           src/core/Command.cpp\
           src/core/CommandFile.cpp\
           src/core/CommandFactory.cpp\
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
           src/utils/Option.cpp\
           src/utils/StringBuffer.cpp\
           src/utils/Utils.cpp\
           src/view/EntityParser.cpp\
           src/view/EntityViewer.cpp\
           src/view/TextureManager.cpp\
           src/view/Tree.cpp\
           src/view/View.cpp \
    src/ui/MainMenu.cpp \
    src/ui/PlayMenu.cpp

HEADERS  +=src/ui/MainWindow.hpp\
           src/core/Game.hpp\
           src/core/Command.hpp\
           src/core/CommandFile.hpp\
           src/core/CommandFactory.hpp\
           src/network/Client.hpp\
           src/network/NetworkEvent.hpp\
           src/network/Server.hpp\
           src/simulation/Bullet.hpp\
           src/simulation/Change.hpp\
           src/simulation/CollidableEntity.hpp\
           src/simulation/ContactListener.hpp\
           src/simulation/Entity.hpp\
           src/simulation/EntityFactory.hpp\
           src/simulation/Explosion.hpp\
           src/simulation/GameWorld.hpp\
           src/simulation/Obstacle.hpp\
           src/simulation/PhysicWorld.hpp\
           src/simulation/Tank.hpp\
           src/utils/Logger.hpp\
           src/utils/Option.hpp\
           src/utils/StringBuffer.hpp\
           src/utils/Utils.hpp\
           src/view/EntityParser.hpp\
           src/view/EntityViewer.hpp\
           src/view/Event.hpp\
           src/view/TextureManager.hpp\
           src/view/Tree.hpp\
           src/view/View.hpp \
    src/ui/MainMenu.hpp \
    src/ui/PlayMenu.hpp \
    src/ui/Menu.hpp

FORMS += \
    ui/MainMenu.ui \
    ui/PlayMenu.ui
