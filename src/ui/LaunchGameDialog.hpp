#ifndef LAUNCHGAMEDIALOG_HPP
#define LAUNCHGAMEDIALOG_HPP

#include <QDialog>
#include <QWidget>

namespace Ui
{
    class LaunchGameDialog;
}

class LaunchGameDialog : public QDialog
{
    Q_OBJECT

    public :

        explicit LaunchGameDialog(QWidget *parent = 0);
        virtual ~LaunchGameDialog();

    protected:

        void timerEvent(QTimerEvent *event);

    private :

        Ui::LaunchGameDialog *ui;
};

#endif // LAUNCHGAMEDIALOG_HPP
