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
#include "AbilityRepertoirePanel.hxx"
#include "SGLib.hxx"
#include "AbilityToolbar.hxx"

int AbilityRepertoirePanel::m_iPanelInstances = 0;

void AbilityRepertoirePanel::render_panel(float const flDelta)
{
	ImGui::Begin("Known Abilities");

	std::vector<SpellId> vKnownSpells = this->m_pCharEntity->get_known_spells();

	ImGui::TextWrapped("To assign an ability to the hotbar, simply drag and drop the ability to any hotbar slot.");

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth(), 200.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	for(int i = 0; i < vKnownSpells.size(); ++i)
	{
		SpellId const iSpellId = vKnownSpells[i];

		if(iSpellId == 0)
		{
			continue;
		}

		ISpell *pSpell = SG::get_spell_manager()->get_spell(iSpellId);

		if(pSpell == nullptr)
		{
			continue;
		}

		void *pStaticTexture = SG::get_static_texture_manager()->get_texture_handle(pSpell->get_icon_texture());

		ImGui::Image(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

		if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			struct AbilityDrop drop;
			drop.iSpellId = iSpellId;
			drop.iSpellSlot = -1;

			ImGui::SetDragDropPayload("SPELLID", reinterpret_cast<void const *>(&drop), sizeof(struct AbilityDrop), ImGuiCond_Once);

			ImGui::Image(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			ImGui::EndDragDropSource();
		}

		if(ImGui::IsItemHovered())
		{
			AbilityToolbar::do_ability_tooltip(pSpell);
		}

		int itemsPerRow = (int)((ImGui::GetWindowContentRegionWidth() - 32.0f) / 32.0f);

		if(itemsPerRow > 0 && i && i % itemsPerRow == 0)
		{
			ImGui::NewLine();
		}
		else
		{
			ImGui::SameLine();
		}
	}

	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	if(ImGui::Button("Clear Hotbar"))
	{
		this->m_pCharEntity->set_mapped_spells({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
	}

	ImGui::SameLine();

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}