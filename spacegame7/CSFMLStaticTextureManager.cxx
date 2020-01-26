#include "SFMLIncludes.hxx"
#include "CSFMLStaticTextureManager.hxx"
#include "SGLib.hxx"

void CSFMLStaticTextureManager::load_texture(StaticTextureId const texId, char const *szTextureFile)
{
	if(texId >= MAXIMUM_STATIC_TEXTURES)
	{
		throw SGException("CSFMLStaticTextureManager reached maximum upper bound on allowed number of static textures");
	}

	if(this->m_vecTextures.size() <= texId)
	{
		this->m_vecTextures.resize(texId + 1);
	}

	std::string szFullTexturePath = SG::get_game_data_manager()->get_full_data_file_path(szTextureFile);

	this->m_vecTextures[texId].loadFromFile(szFullTexturePath);
}

void CSFMLStaticTextureManager::render_texture(StaticTextureId texId, StaticTextureRenderParameters const &texRenderParms)
{
	if(this->m_vecTextures.size() <= texId)
	{
		throw SGException("CSFMLStaticTextureManager::render_texture was given an invalid static texture ID (texture not loaded)");
	}

	sf::Texture &sfTex = this->m_vecTextures[texId];

	sf::Texture::bind(&sfTex);

	glPushMatrix();

	glTranslatef(texRenderParms.flPositionX, texRenderParms.flPositionY, -2500.0f);
	glRotatef(texRenderParms.flRotation, 0.0f, 0.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-texRenderParms.flWidth / 2.0f, -texRenderParms.flHeight / 2.0f, texRenderParms.flPositionZ); //3072
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-texRenderParms.flWidth / 2.0f, texRenderParms.flHeight / 2.0f, texRenderParms.flPositionZ);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(texRenderParms.flWidth / 2.0f, texRenderParms.flHeight / 2.0f, texRenderParms.flPositionZ);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(texRenderParms.flWidth / 2.0f, -texRenderParms.flHeight / 2.0f, texRenderParms.flPositionZ);
	glEnd();

	glPopMatrix();
}

void *CSFMLStaticTextureManager::get_texture_handle(StaticTextureId const iTextureId)
{
	return (void*)this->m_vecTextures[iTextureId].getNativeHandle();
}