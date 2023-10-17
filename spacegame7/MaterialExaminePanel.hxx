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
#pragma once

#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"

class MaterialExaminePanel : public InterfacePanel
{
public:
	MaterialExaminePanel(std::string szMaterialSerial)
		: m_szMaterialSerial(szMaterialSerial)
	{
		if(++this->m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}

		this->build_id_text_block();
		this->build_attribs_text_block();
	};
	virtual ~MaterialExaminePanel()
	{
		--this->m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	std::string m_szMaterialSerial;
	std::string m_szIdText;
	std::string m_szMatAttribsText;
	static int m_iPanelInstances;
	bool m_bPanelActive;

	void build_id_text_block(void);
	void build_attribs_text_block(void);
};