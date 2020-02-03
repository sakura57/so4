#include "SkillTreePanel.hxx"
#include "SGLib.hxx"
#include "ISpell.hxx"
#include <iostream>

bool SkillTreePanel::m_bPanelExists = false;

void SkillTreePanel::render_panel(float const flDelta)
{
	if (!this->m_bPanelExists)
	{
		return;
	}

	if (this->m_pEntity == nullptr)
	{
		return;
	}

	CGameDataManager *pGDM = SG::get_game_data_manager();
	CSkillDataManager* pSDM = pGDM->get_skill_data_manager();

	ImGui::Begin("Skills");

	std::string szName = this->m_pEntity->get_name();
	int iCurrentLevel = this->m_pEntity->get_level();
	unsigned int uiFreeSkillPoints = this->m_pEntity->get_skill_points_available();

	ImGui::Text("Name: %s", szName.c_str());

	ImGui::Text("Elite Pilot, Level %d", iCurrentLevel);

	ImGui::Text("Free skill points: %u", uiFreeSkillPoints);

	ImVec2 windowSize = ImGui::GetWindowSize();

	int iNumSkillTrees = pSDM->get_skill_trees();

	for (int i = 0; i < iNumSkillTrees; ++i)
	{
		CSkillTree* pSkillTree = pSDM->get_skill_tree(i);

		if (i == 0) ImGui::NewLine();
		else if (i != 0) ImGui::SameLine();

		if (pSkillTree != nullptr)
		{
			std::string skillTreeName;
			pSkillTree->get_name(skillTreeName);

			if(ImGui::Button(skillTreeName.c_str(), ImVec2(windowSize.x / iNumSkillTrees, 20.0f)))
			{
				this->m_pSelectedSkillTree = pSkillTree;
				this->m_bRepaintTree = true;
			}
		}
	}

	/*
	 * we would like to essentially cache the entire tree
	 * so that we aren't polling the SDM every frame when
	 * IMGUI redraws
	 */
	if (this->m_bRepaintTree == true)
	{
		CSkillTree* pSkillTree = this->m_pSelectedSkillTree;

		std::vector<SkillLineId> lineIds;
		std::vector<CSkillLine*> skillLines;

		pSkillTree->get_skill_lines(lineIds);

		for (int i = 0; i < lineIds.size(); ++i)
		{
			skillLines.push_back(pSDM->get_skill_line(lineIds[i]));
		}

		this->m_vCurrentTreeSkills.clear();

		for (int i = 0; i < skillLines.size(); ++i)
		{
			std::vector<SkillId> lineSkillIds;
			std::vector<CSkill*> lineSkills;

			skillLines[i]->get_skills(lineSkillIds);

			for (SkillId skillId : lineSkillIds)
			{
				lineSkills.push_back(pSDM->get_skill(skillId));
			}

			this->m_vCurrentTreeSkills.push_back(lineSkills);
		}

		SkillId uiFinalSkillId;
		pSkillTree->get_final_skill(uiFinalSkillId);

		this->m_pCurrentLegendarySkill = pSDM->get_skill(uiFinalSkillId);

		this->m_bRepaintTree = false;
	}

	ImGui::Separator();
	ImGui::Columns(this->m_vCurrentTreeSkills.size(), "tree", true);

	int totalSkillsInTree = 0;
	int skillsAcquiredInTree = 0;

	for (int i = 0; i < this->m_vCurrentTreeSkills.size(); ++i)
	{
		int skillsAcquiredInLine = 0;

		for (int j = 0; j < this->m_vCurrentTreeSkills[i].size(); ++j)
		{
			++totalSkillsInTree;

			bool bAcquired = this->m_pEntity->has_skill(this->m_vCurrentTreeSkills[i][j]->get_id());
			bool bCanAcquire = !bAcquired && ((skillsAcquiredInLine >= j) && (uiFreeSkillPoints > 0U));

			if(bAcquired)
			{
				++skillsAcquiredInLine;
			}

			render_skill_button(this->m_vCurrentTreeSkills[i][j], false, bAcquired, bCanAcquire);
		}

		skillsAcquiredInTree += skillsAcquiredInLine;

		ImGui::NextColumn();
	}

	ImGui::Columns();

	if (this->m_pCurrentLegendarySkill != nullptr)
	{
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(windowSize.x - (windowSize.x / 2.0f) - 32.0f, cursorPos.y));

		bool bHasLegendarySkill = this->m_pEntity->has_skill(this->m_pCurrentLegendarySkill->get_id());
		bool bCanAcquireLegendarySkill = !bHasLegendarySkill && (totalSkillsInTree == skillsAcquiredInTree) && (uiFreeSkillPoints > 0U);

		render_skill_button(this->m_pCurrentLegendarySkill, true, bHasLegendarySkill, bCanAcquireLegendarySkill);
	}

	ImGui::Separator();

	if (ImGui::Button("Done"))
	{
		this->m_bPanelExists = false;
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool SkillTreePanel::panel_active(void)
{
	return this->m_bPanelActive;
}

void SkillTreePanel::render_skill_button(CSkill* pSkill, bool bLegendary, bool bHasSkill, bool bCanAcquireSkill)
{
	void* pStaticTexture = SG::get_static_texture_manager()->get_texture_handle(pSkill->get_icon_id());

	ImVec4 buttonColor;

	if (bHasSkill)
	{
		buttonColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (bCanAcquireSkill)
	{
		buttonColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		buttonColor = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
	}

	if (ImGui::ImageButton(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), buttonColor))
	{
		if (!bHasSkill && bCanAcquireSkill)
		{
			this->m_pEntity->grant_skill(pSkill->get_id());
		}
	}

	if (ImGui::IsItemHovered())
	{
		do_skill_tooltip(pSkill, bLegendary, bHasSkill, bCanAcquireSkill);
	}
}

void SkillTreePanel::do_skill_tooltip(CSkill* pSkill, bool bLegendarySkill, bool bHasSkill, bool bCanAcquireSkill)
{
	std::string temp;
	std::vector<SpellId> spellsGranted;

	ImGui::BeginTooltip();

	pSkill->get_name(temp);
	ImGui::TextUnformatted(temp.c_str());

	if (bLegendarySkill)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Legendary Skill");
	}
	else
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Skill");
	}

	CStatBlock statBlock;

	pSkill->get_stat_block(statBlock);

	if (!statBlock.empty())
	{
		ImGui::Separator();

		statBlock.format_stat_text(temp);
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), temp.c_str());
	}

	pSkill->get_spells(spellsGranted);

	if (spellsGranted.size() > 0)
	{
		ImGui::Separator();

		ImGui::TextUnformatted("Grants:");

		for (int i = 0; i < spellsGranted.size(); ++i)
		{
			ISpell* pSpell = SG::get_spell_manager()->get_spell(spellsGranted[i]);

			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), " %s", pSpell->get_name().c_str());
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), " %s", pSpell->get_desc().c_str());
			ImGui::TextUnformatted("\n");
		}
	}

	if (bHasSkill)
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "You have this skill.");
	}
	else if (bCanAcquireSkill)
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Click to acquire this skill.\nThis will cost 1 skill point.");
	}
	else
	{
		ImGui::TextColored(ImVec4(1.0f, 0.25f, 0.0f, 1.0f), "You may not acquire this skill.\nYou require some free skill points,\nor do not meet the prerequisites.");
	}

	ImGui::EndTooltip();
};