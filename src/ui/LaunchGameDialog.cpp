#include "LaunchGameDialog.hpp"
#include "ui_LaunchGameDialog.h"


LaunchGameDialog::LaunchGameDialog(QWidget *parent):
QDialog(parent),
ui(new Ui::LaunchGameDialog)
{
    ui->setupUi(this);
    startTimer(1000);

    QObject::connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(reject()));
}

LaunchGameDialog::~LaunchGameDialog()
{
    delete ui;
}

void LaunchGameDialog::timerEvent(QTimerEvent *event)
{
    int secondsLeft = ui->secondsLeftLabel->text().toInt();
    if(secondsLeft == 0)
        accept();
    else
    {
        --secondsLeft;
        ui->secondsLeftLabel->setText(QString::number(secondsLeft));
    }
}
