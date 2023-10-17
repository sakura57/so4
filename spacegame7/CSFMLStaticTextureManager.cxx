/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
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