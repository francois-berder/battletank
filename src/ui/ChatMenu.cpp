#include <QCheckBox>
#include <QLabel>
#include <QThread>
#include "ChatMenu.hpp"
#include "ui_ChatMenu.h"
#include "Menu.hpp"

ChatMenu::ChatMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::ChatMenu),
m_player(),
m_host()
{
    ui->setupUi(this);

    QObject::connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    QObject::connect(ui->leaveButton, SIGNAL(clicked()), this, SLOT(leave()));

    QObject::connect(&m_player, SIGNAL(receivedMessage(QString, QString)), this, SLOT(printMessage(QString, QString)));
    QObject::connect(&m_player, SIGNAL(playerJoined(QString)), this, SLOT(addPlayer(QString)));
    QObject::connect(&m_player, SIGNAL(playerLeft(QString)), this, SLOT(removePlayer(QString)));

}

ChatMenu::~ChatMenu()
{
    clean();

    delete ui;
}

void ChatMenu::host(QString pseudo)
{
    m_host.start();

    // Allow some time for the host to be created
    // before joining it
    QThread::msleep(100);

    join(pseudo, "localhost");
}

void ChatMenu::join(QString pseudo, QString serverAddress)
{
    m_player.setPseudo(pseudo);
    m_player.join(serverAddress);
}

void ChatMenu::sendMessage()
{
    QString str = ui->textToSend->text();
    if(str.isEmpty())
        return;

    ui->textToSend->clear();
    m_player.sendMessage(str);
}

void ChatMenu::leave()
{
    clean();

    emit changeInterface(PLAY_MENU);
}

void ChatMenu::printMessage(QString pseudo, QString message)
{
    ui->text->append("<strong>" + pseudo + ": </strong>" + message);
}

void ChatMenu::addPlayer(QString pseudo)
{
    QLabel *label = new QLabel(pseudo);
    ui->gridLayout->addWidget(label, ui->gridLayout->rowCount(), 0);
    QCheckBox *checkBox = new QCheckBox;
    ui->gridLayout->addWidget(checkBox, ui->gridLayout->rowCount()-1, 1);
}

void ChatMenu::removePlayer(QString pseudo)
{
    for(int i = 0; i < ui->gridLayout->rowCount(); ++i)
    {
        QString p = dynamic_cast<QLabel*>(ui->gridLayout->itemAtPosition(i,0))->text();
        if(p == pseudo)
        {
            QLayoutItem *item = ui->gridLayout->itemAtPosition(i, 0);
            ui->gridLayout->removeItem(item);
            delete item;

            item = ui->gridLayout->itemAtPosition(i, 1);
            ui->gridLayout->removeItem(item);
            delete item;
        }
    }
}

void ChatMenu::clean()
{
    m_player.leave();
    m_host.stop();

    for(int i = 0; i < ui->gridLayout->rowCount(); ++i)
    {
        QLayoutItem *item = ui->gridLayout->itemAtPosition(i, 0);
        ui->gridLayout->removeItem(item);
        delete item;

        item = ui->gridLayout->itemAtPosition(i, 1);
        ui->gridLayout->removeItem(item);
        delete item;
    }

    ui->text->clear();
    ui->textToSend->clear();
}
