#include <QTcpSocket>
#include <QMessageBox>
#include "JoinDialog.hpp"
#include "ui_JoinDialog.h"

JoinDialog::JoinDialog(QWidget *parent):
QDialog(parent),
ui(new Ui::JoinDialog)
{
    ui->setupUi(this);
}

JoinDialog::~JoinDialog()
{
    delete ui;
}

void JoinDialog::accept()
{
    // Ping to server
    QTcpSocket socket;
    socket.connectToHost(ui->serverAddress->text(), 10000);
    if(socket.state() == QAbstractSocket::ConnectedState)
    {
        socket.write("EXIST_GAME");
        QString correctAnswer = "GAME_EXIST";
        char response[correctAnswer.size()+1];
        socket.read(response, correctAnswer.size());
        if(correctAnswer != response)
            QMessageBox::critical(this, "Game inexistent", "No game is hosted on this server.");
        else
        {
            setResult(QDialog::Accepted);
            hide();
        }
        socket.disconnect();
    }
    else
        QMessageBox::critical(this, "Connection problem", "Failed to connect to server.");
}
