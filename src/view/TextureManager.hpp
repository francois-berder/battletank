#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

typedef std::shared_ptr<sf::Texture> TexturePtr;

class TextureManager
{
	public:

		TextureManager(const TextureManager& tm) = delete;
		TextureManager& operator=(const TextureManager& tm) = delete;

		static TextureManager& instance();

		TexturePtr get(const std::string& name);

	private:

		TextureManager();

		TexturePtr load(const std::string &path);

		std::map<std::string, TexturePtr> m_textures;
};

#endif /* __TEXTURE_MANAGER_H__ */

