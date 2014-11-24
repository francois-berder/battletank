#ifndef STATMENU_HPP
#define STATMENU_HPP

#include <QWidget>

namespace Ui
{
    class StatMenu;
}

class StatMenu : public QWidget
{
    Q_OBJECT

    public :

        explicit StatMenu(QWidget *parent = 0);
        virtual ~StatMenu();

    signals :

        void changeInterface(int);

    private slots :

        void backToMainMenu();

    private :

        Ui::StatMenu *ui;
};

#endif // STATMENU_HPP
