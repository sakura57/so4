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
#include "CAstfield.hxx"
#include <unordered_map>
#include <string>

#define BUFFER_SIZE 512

typedef std::unordered_map<std::string, unsigned int> MaterialQuantityMap;

class AsteroidProspectPanel : public InterfacePanel
{
public:
	AsteroidProspectPanel(AstfieldId uiAstfieldId)
		: m_uiAstfieldId(uiAstfieldId), m_szDepositSerial("")
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
		this->build_desc_text_block();
	};
	virtual ~AsteroidProspectPanel()
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
	AstfieldId m_uiAstfieldId;
	std::string m_szIdText;
	std::string m_szDescText;
	std::string m_szDepositSerial;
	static int m_iPanelInstances;
	bool m_bPanelActive;
	char m_szDescBuffer[BUFFER_SIZE];

	MaterialQuantityMap m_matQuantityMap;

	void build_id_text_block(void);
	void build_desc_text_block(void);

	static void do_mat_list(MaterialQuantityMap&);
};