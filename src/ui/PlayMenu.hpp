#ifndef PLAYMENU_HPP
#define PLAYMENU_HPP

#include <QWidget>

namespace Ui
{
    class PlayMenu;
}

class PlayMenu : public QWidget
{
    Q_OBJECT

    public :

        explicit PlayMenu(QWidget *parent = 0);
        virtual ~PlayMenu();

    signals :

        void changeInterface(int);
        void createClient(QString, QString);
        void createServer(QString);

    private slots :

        void toMainMenu();
        void connectToServer();
        void hostGame();

    private :

        Ui::PlayMenu *ui;
};

#endif // PLAYMENU_HPP
