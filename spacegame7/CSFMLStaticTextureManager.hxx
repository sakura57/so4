#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "IStaticTextureManager.hxx"

class CSFMLStaticTextureManager : public IStaticTextureManager
{
public:
	CSFMLStaticTextureManager() {};
	~CSFMLStaticTextureManager() {};

	virtual void load_texture(StaticTextureId const, char const *) final;
	virtual void render_texture(StaticTextureId const, StaticTextureRenderParameters const &) final;
	virtual void *get_texture_handle(StaticTextureId const) final;

private:
	std::vector<sf::Texture> m_vecTextures;
};