#include <QInputDialog>
#include <QDir>
#include "PlayMenu.hpp"
#include "ui_PlayMenu.h"
#include "Menu.hpp"
#include "JoinDialog.hpp"

PlayMenu::PlayMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::PlayMenu)
{
    ui->setupUi(this);

    QObject::connect(ui->hostButton, SIGNAL(clicked()), this, SLOT(hostGame()));
    QObject::connect(ui->joinButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
    QObject::connect(ui->backButton, SIGNAL(clicked()), this, SLOT(toMainMenu()));
}

PlayMenu::~PlayMenu()
{
    delete ui;
}

void PlayMenu::toMainMenu()
{
    emit changeInterface(MAIN_MENU);
}

void PlayMenu::connectToServer()
{
    JoinDialog dialog;
    int ret = dialog.exec();
    if(ret == QDialog::Accepted)
        emit createClient(dialog.getServerAddress(), dialog.getPseudo());
}

void PlayMenu::hostGame()
{
    bool ok;
    QString pseudo = QInputDialog::getText(this, "Enter your pseudo",
                                          "Pseudo:", QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);
    if(ok)
        emit createServer(pseudo);
}

