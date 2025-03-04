#include "GameData.hpp"

GameData::GameData():
m_players(),
m_text()
{

}

void GameData::addPlayer(const QString &pseudo)
{
    m_players[pseudo] = false;
}

void GameData::removePlayer(const QString &pseudo)
{
    m_players.remove(pseudo);
}

void GameData::setPlayerReadiness(const QString &pseudo, const bool isReady)
{
    m_players[pseudo] = isReady;
}

QMap<QString, bool> GameData::getPlayers() const
{
    return m_players;
}

unsigned int GameData::getNbPlayers() const
{
    return m_players.size();
}

bool GameData::containsPlayer(QString pseudo) const
{
    return m_players.contains(pseudo);
}

void GameData::appendText(const QString &text)
{
    if(!m_text.isEmpty())
        m_text += "<br>";

    m_text += text;
}

QString GameData::getText() const
{
    return m_text;
}

void GameData::clean()
{
    m_players.clear();
    m_text.clear();
}
