#ifndef JOINDIALOG_HPP
#define JOINDIALOG_HPP

#include <QDialog>
#include <QWidget>

namespace Ui
{
    class JoinDialog;
}

class JoinDialog : public QDialog
{
    Q_OBJECT

    public :

        explicit JoinDialog(QWidget *parent = 0);
        virtual ~JoinDialog();

    public slots :

        virtual void accept();

    private :

        Ui::JoinDialog *ui;
};

#endif // JOINDIALOG_HPP
