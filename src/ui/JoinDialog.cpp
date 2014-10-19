#include <SFML/Network.hpp>
#include <QMessageBox>
#include "JoinDialog.hpp"
#include "ui_JoinDialog.h"

JoinDialog::JoinDialog(QWidget *parent):
QDialog(parent),
ui(new Ui::JoinDialog),
m_pseudo(),
m_serverAddress()
{
    ui->setupUi(this);
}

JoinDialog::~JoinDialog()
{
    delete ui;
}

void JoinDialog::accept()
{
    if(ui->pseudo->text().isEmpty())
        QMessageBox::critical(this, "Invalid pseudo", "Your need to enter a valid pseudo.");

    // Ping to server
    sf::TcpSocket socket;
    if(socket.connect(ui->serverAddress->text().toStdString(), 9999) == sf::Socket::Done)
    {
        sf::Packet packet;
        packet << "PING_GAME" << ui->pseudo->text().toStdString();
        socket.send(packet);
        packet.clear();
        socket.receive(packet);
        std::string answer;
        packet >> answer;
        if(answer == "PSEUDO_ALREADY_IN_USE")
            QMessageBox::critical(this, "Pseudo already in use", "This pseudo is already used by someone else. Change your pseudo and try again.");
        else if(answer == "GAME_ALREADY_STARTED")
            QMessageBox::critical(this, "Game already started", "The game has already started.");
        else if(answer == "GAME_ALREADY_FULL")
            QMessageBox::critical(this, "Maximum players limit reached", "The host cannot accept anymore players.");
        else
        {
            m_pseudo = ui->pseudo->text();
            m_serverAddress = ui->serverAddress->text();
            setResult(QDialog::Accepted);
            hide();
        }
        socket.disconnect();
    }
    else
        QMessageBox::critical(this, "Connection problem", "Failed to connect to server.");
}

QString JoinDialog::getPseudo()
{
    return m_pseudo;
}

QString JoinDialog::getServerAddress()
{
    return m_serverAddress;
}
