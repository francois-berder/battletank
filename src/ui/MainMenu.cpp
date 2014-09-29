#include "MainMenu.hpp"
#include "ui_MainMenu.h"
#include "Menu.hpp"

MainMenu::MainMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    QObject::connect(ui->playButton, SIGNAL(clicked()), this, SLOT(toPlayMenu()));
    QObject::connect(ui->quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::toPlayMenu()
{
    emit changeInterface(PLAY_MENU);
}
