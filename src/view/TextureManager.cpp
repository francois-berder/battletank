#include <stdexcept>
#include <sstream>

#include "TextureManager.hpp"

TextureManager::TextureManager() :
		m_textures()
{
}

TextureManager& TextureManager::instance()
{
	static TextureManager tm;
	return tm;
}

TexturePtr TextureManager::get(const std::string& name)
{
	auto it = m_textures.find(name);
	if(it != m_textures.end())
		return it->second;

	std::string path = "media/textures/" + name;
	m_textures[name] = load(path);

	return m_textures[name];
}

TexturePtr TextureManager::load(const std::string &path)
{
	sf::Texture *tex = new sf::Texture;
	if(!tex->loadFromFile(path))
	{
		delete tex;
		std::stringstream msg;
		msg << "Could not load texture from file " << path;
		throw std::runtime_error(msg.str());
	}

	return TexturePtr(tex);
}
