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

    for(int i = 0; i < 4; ++i)
    {
        ui->players->setCellWidget(i, 0, new QLabel);
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setDisabled(true);
        ui->players->setCellWidget(i, 1, checkBox);
    }

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
    ui->text->append("<b>" + pseudo + ": </b>" + message);
}

void ChatMenu::addPlayer(QString pseudo)
{
    for(int i = 0; i < 4; ++i)
    {
        QLabel *label = dynamic_cast<QLabel*>(ui->players->cellWidget(i,0));

        if(label->text().isEmpty())
        {
            label->setText(pseudo);
            if(pseudo == m_player.getPseudo())
                dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1))->setEnabled(true);

            break;
        }
    }

    ui->text->append("<i>" + pseudo + " joined this game.</i>");
}

void ChatMenu::removePlayer(QString pseudo)
{
    for(int i = 0; i < 4; ++i)
    {
        QLabel *label = dynamic_cast<QLabel*>(ui->players->cellWidget(i,0));

        if(label->text() == pseudo)
        {
            label->clear();
            QCheckBox *checkBox = dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1));
            checkBox->setChecked(false);
            checkBox->setDisabled(true);
            break;
        }
    }
    ui->text->append("<i>" + pseudo + " left this game.</i>");
}

void ChatMenu::clean()
{
    m_player.leave();

    // Allow some time for the host to remove the player
    // before stopping it
    QThread::msleep(100);

    m_host.stop();

    for(int i = 0; i < 4; ++i)
    {
        dynamic_cast<QLabel*>(ui->players->cellWidget(i,0))->clear();
        QCheckBox *checkBox = dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1));
        checkBox->setChecked(false);
        checkBox->setEnabled(false);
    }

    ui->text->clear();
    ui->textToSend->clear();
}
