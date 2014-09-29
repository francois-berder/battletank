#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <QWidget>

namespace Ui
{
    class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

    public :

        explicit MainMenu(QWidget *parent = 0);
        virtual ~MainMenu();

    signals :

        void changeInterface(int);

    private slots :

        void toPlayMenu();

    private :

        Ui::MainMenu *ui;
};

#endif // MAINMENU_HPP
