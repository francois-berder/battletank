#include "PlayMenu.hpp"
#include "ui_PlayMenu.h"
#include "Menu.hpp"
#include "JoinDialog.hpp"

PlayMenu::PlayMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::PlayMenu)
{
    ui->setupUi(this);

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
    {
        // goto chat menu
    }
}
