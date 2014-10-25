#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <QMap>
#include <QString>

class GameData
{
    public :

        GameData();

        void addPlayer(const QString &pseudo);
        void removePlayer(const QString &pseudo);
        void setPlayerReadiness(const QString &pseudo, const bool isReady);
        QMap<QString, bool> getPlayers() const;
        unsigned int getNbPlayers() const;
        bool containsPlayer(QString pseudo) const;

        void appendText(const QString &text);
        QString getText() const;

        void clean();

    private :

        QMap<QString, bool> m_players;
        QString m_text;
};

#endif // GAMEDATA_HPP
