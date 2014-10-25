#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QThread>
#include "ChatMenu.hpp"
#include "ui_ChatMenu.h"
#include "Menu.hpp"
#include "LaunchGameDialog.hpp"

ChatMenu::ChatMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::ChatMenu),
m_player(),
m_host(m_data),
m_data()
{
    ui->setupUi(this);

    for(int i = 0; i < 4; ++i)
    {
        ui->players->setCellWidget(i, 0, new QLabel);
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setEnabled(false);
        ui->players->setCellWidget(i, 1, checkBox);
    }
    ui->players->setHorizontalHeaderLabels(QStringList() << "name" << "ready?");
    ui->players->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QObject::connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    QObject::connect(ui->leaveButton, SIGNAL(clicked()), this, SLOT(leave()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), &m_host, SLOT(launchGame()));

    QObject::connect(&m_player, SIGNAL(receivedMessage(QString, QString)), this, SLOT(printMessage(QString, QString)));
    QObject::connect(&m_player, SIGNAL(playerJoined(QString)), this, SLOT(addPlayer(QString)));
    QObject::connect(&m_player, SIGNAL(playerLeft(QString)), this, SLOT(removePlayer(QString)));
    QObject::connect(&m_player, SIGNAL(playerReady(QString)), this, SLOT(playerReady(QString)));
    QObject::connect(&m_player, SIGNAL(playerNotReady(QString)), this, SLOT(playerNotReady(QString)));
    QObject::connect(&m_player, SIGNAL(gameCancelled()), this, SLOT(cancel()));
    QObject::connect(&m_player, SIGNAL(existingText(QString)), ui->text, SLOT(setText(QString)));
    QObject::connect(&m_player, SIGNAL(gameLaunchStarted()), this, SLOT(reportGameLaunchStarted()));
    QObject::connect(&m_player, SIGNAL(gameLaunchAborted(QString)), this, SLOT(reportGameLaunchAborted(QString)));
    QObject::connect(&m_player, SIGNAL(errorJoined(QString)), this, SLOT(reportErrorJoined(QString)));

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
    ui->startButton->show();
    ui->startButton->setEnabled(false);
    ui->IPAddress->show();
    ui->labelIPAdress->show();

    ui->IPAddress->setText(m_player.getIPAddress());
}

void ChatMenu::join(QString pseudo, QString serverAddress)
{
    m_player.setPseudo(pseudo);
    m_player.join(serverAddress);

    ui->startButton->hide();
    ui->IPAddress->hide();
    ui->labelIPAdress->hide();
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
    QString msg = "<b>" + pseudo + ": </b>" + message;
    ui->text->append(msg);
    if(m_host.isRunning())
        m_data.appendText(msg);
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
            {
                QCheckBox *checkBox = dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1));
                QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(sendPlayerReadiness(int)));
                checkBox->setEnabled(true);
            }
            if(m_host.isRunning())
                m_data.addPlayer(pseudo);
            break;
        }
    }
    QString msg = "<i>" + pseudo + " joined this game.</i>";
    ui->text->append(msg);
    if(m_host.isRunning())
        m_data.appendText(msg);

    checkCanStart();
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
            if(pseudo == m_player.getPseudo())
                QObject::disconnect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(sendPlayerReadiness(int)));
            checkBox->setChecked(false);
            checkBox->setEnabled(false);
            if(m_host.isRunning())
                m_data.removePlayer(pseudo);
            break;
        }
    }
    QString msg = "<i>" + pseudo + " left this game.</i>";
    ui->text->append(msg);
    if(m_host.isRunning())
        m_data.appendText(msg);

    checkCanStart();
}

void ChatMenu::cancel()
{
    QMessageBox::information(this, "Game cancelled", "This game has been cancelled by the host.");
    leave();
}

void ChatMenu::sendPlayerReadiness(int isReady)
{
    if(isReady == Qt::Checked)
        m_player.setReadiness(true);
    else
        m_player.setReadiness(false);
}

void ChatMenu::playerReady(QString pseudo)
{
    changePlayerReady(pseudo, true);
}

void ChatMenu::playerNotReady(QString pseudo)
{
    changePlayerReady(pseudo, false);
}

void ChatMenu::changePlayerReady(QString pseudo, bool isReady)
{
    for(int i = 0; i < 4; ++i)
    {
        QLabel *label = dynamic_cast<QLabel*>(ui->players->cellWidget(i,0));

        if(label->text() == pseudo)
        {
            dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1))->setChecked(isReady);
            if(m_host.isRunning())
                m_data.setPlayerReadiness(pseudo, isReady);
            break;
        }
    }

    checkCanStart();
}

void ChatMenu::reportGameLaunchStarted()
{
    LaunchGameDialog dialog;
    QObject::connect(&m_player, SIGNAL(gameLaunchAborted(QString)), &dialog, SLOT(close()));
    int ret = dialog.exec();
    if(ret == QDialog::Accepted)
        emit changeInterface(GAME_MENU);
    else
        m_player.abortLaunch();
}

void ChatMenu::reportGameLaunchAborted(QString pseudo)
{
    if(pseudo == m_player.getPseudo())
        return;

    QMessageBox::information(this, "Game launch aborted", pseudo + " has aborted the launch of this game");
}

void ChatMenu::reportErrorJoined(QString error)
{
    QMessageBox::information(this, "Error joining game", "Could not join game. Reason: " + error);
    leave();
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
        QLabel *label = dynamic_cast<QLabel*>(ui->players->cellWidget(i,0));
        QCheckBox *checkBox = dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1));
        if(label->text() == m_player.getPseudo())
            QObject::disconnect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(sendPlayerReadiness(int)));
        label->clear();
        checkBox->setChecked(false);
        checkBox->setEnabled(false);
    }

    ui->text->clear();
    ui->textToSend->clear();
    m_data.clean();
}

/*
 * Can start if:
 * at least two players joined the game
 * and all players are ready.
 */
void ChatMenu::checkCanStart()
{
    int nbPlayersJoined = 0, nbPlayersReady = 0;
    for(int i = 0; i < 4; ++i)
    {
        QLabel *label = dynamic_cast<QLabel*>(ui->players->cellWidget(i,0));
        QCheckBox *checkBox = dynamic_cast<QCheckBox*>(ui->players->cellWidget(i,1));
        if(!label->text().isEmpty())
        {
            ++nbPlayersJoined;

            if(checkBox->isChecked())
                ++nbPlayersReady;
        }
    }

    if(nbPlayersJoined >= 2 && nbPlayersJoined == nbPlayersReady)
        ui->startButton->setEnabled(true);
    else
        ui->startButton->setEnabled(false);
}
