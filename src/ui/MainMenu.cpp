#include "MainMenu.hpp"
#include "ui_MainMenu.h"

MainMenu::MainMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    QObject::connect(ui->quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}

MainMenu::~MainMenu()
{
    delete ui;
}
