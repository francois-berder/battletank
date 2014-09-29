#include "PlayMenu.hpp"
#include "ui_PlayMenu.h"
#include "Menu.hpp"

PlayMenu::PlayMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::PlayMenu)
{
    ui->setupUi(this);

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
