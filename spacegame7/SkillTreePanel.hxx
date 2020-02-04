#pragma once

#include "ICharacterEntity.hxx"
#include "InterfacePanel.hxx"
#include "SGLib.hxx"
#include "CSkillTree.hxx"

class SkillTreePanel : public InterfacePanel
{
public:
	SkillTreePanel(ICharacterEntity* pEntity)
		: m_pEntity(pEntity), m_bRepaintTree(true)
	{
		if(++SkillTreePanel::m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}

		CGameDataManager* pGDM = SG::get_game_data_manager();
		CSkillDataManager* pSDM = pGDM->get_skill_data_manager();

		if (pSDM->get_skill_trees() > 0)
		{
			//TODO: make this less shit

			this->m_pSelectedSkillTree = pSDM->get_skill_tree(1);
		}
	};
	virtual ~SkillTreePanel()
	{
		--SkillTreePanel::m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	ICharacterEntity* m_pEntity;
	bool m_bPanelActive;
	static int m_iPanelInstances;
	CSkillTree* m_pSelectedSkillTree;
	std::vector<std::vector<CSkill*>> m_vCurrentTreeSkills;
	CSkill* m_pCurrentLegendarySkill;
	bool m_bRepaintTree;

	void render_skill_button(CSkill*, bool, bool, bool);
	void do_skill_tooltip(CSkill*, bool, bool, bool);
};