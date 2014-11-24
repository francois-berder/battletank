#include "StatMenu.hpp"
#include "ui_StatMenu.h"
#include "Menu.hpp"

StatMenu::StatMenu(QWidget *parent):
QWidget(parent),
ui(new Ui::StatMenu)
{
    ui->setupUi(this);

    QObject::connect(ui->goBackButton, SIGNAL(clicked()), this, SLOT(backToMainMenu()));
}

StatMenu::~StatMenu()
{
    delete ui;
}

void StatMenu::backToMainMenu()
{
    emit changeInterface(MAIN_MENU);
}
