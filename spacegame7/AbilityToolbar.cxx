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
#include "AbilityToolbar.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"

void AbilityToolbar::render_panel(float const flDelta)
{
	if(!SG::get_engine()->instance_is_allocated(this->m_iInstanceId))
	{
		return;
	}

	ImGui::Begin("Abilities", nullptr, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize);

	ImGui::Columns(10, "abil1", false);

	std::vector<SpellId> vSpells = this->m_pCharEntity->get_mapped_spells();

	for(int i = 1; i <= 10; ++i)
	{
		bool bSpellSlotHasSpell = false;

		void *pStaticTexture = nullptr;

		int iSlot = i % 10;
		SpellId iSpellId = 0;

		if(iSlot < vSpells.size() && (iSpellId = vSpells[iSlot]))
		{
			bSpellSlotHasSpell = true;

			ISpell *pSpell = SG::get_spell_manager()->get_spell(iSpellId);

			pStaticTexture = SG::get_static_texture_manager()->get_texture_handle(pSpell->get_icon_texture());

			if(this->m_pCharEntity->can_cast_spell(iSlot))
			{
				ImGui::Image(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.77f, 0.77f, 1.0f));

				/*if(ImGui::ImageButton(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f)))
				{
					InstanceId iPlayerInstance = SG::get_world()->get_player();
					InstanceId iPlayerTarget = INVALID_INSTANCE;

					CShip *pShip = SG::get_engine()->instance_get_checked<CShip>(iPlayerInstance);

					if(pShip)
					{
						iPlayerTarget = pShip->get_target();
					}

					if(iPlayerTarget != INVALID_INSTANCE)
					{
						if(this->m_pCharEntity->can_cast_spell(iSlot))
						{
							this->m_pCharEntity->cast_spell(iSlot, iPlayerInstance, iPlayerTarget);
						}
					}
				}*/
			}
			else
			{
				float flTimeRemaining = this->m_pCharEntity->get_spell_cooldown_time_remaining(iSlot);
				float flTimeRemainingRatio = 1.0f - flTimeRemaining / pSpell->get_cooldown();

				ImVec4 tintCol(0.6f, flTimeRemainingRatio * 0.6f, flTimeRemainingRatio * 0.6f, 1.0f);

				ImGui::Image(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), tintCol, ImVec4(0.55f, 0.45f, 0.45f, 1.0f));
			}

			if(ImGui::IsItemHovered())
			{
				AbilityToolbar::do_ability_tooltip(pSpell);
			}
		}
		else
		{
			pStaticTexture = SG::get_static_texture_manager()->get_texture_handle(14);

			ImGui::Image(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		}

		if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			struct AbilityDrop drop;
			drop.iSpellId = iSpellId;
			drop.iSpellSlot = iSlot;

			ImGui::SetDragDropPayload("SPELLID", reinterpret_cast<void const *>(&drop), sizeof(struct AbilityDrop), ImGuiCond_Once);

			ImGui::Image(pStaticTexture, ImVec2(32.0f, 32.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			ImGui::EndDragDropSource();
		}

		if(ImGui::BeginDragDropTarget())
		{
			ImGuiPayload const *pPayload = ImGui::AcceptDragDropPayload("SPELLID");

			if(pPayload && pPayload->DataSize == sizeof(struct AbilityDrop))
			{
				AbilityDrop const *drop = reinterpret_cast<AbilityDrop const *>(pPayload->Data);

				SpellSet spellSet = this->m_pCharEntity->get_mapped_spells();

				SpellId iOldSpell;

				if(drop->iSpellSlot >= 0)
				{
					iOldSpell = spellSet[iSlot];
				}
				else
				{
					iOldSpell = 0;
				}

				spellSet[iSlot] = drop->iSpellId;

				if(drop->iSpellSlot >= 0)
				{
					spellSet[drop->iSpellSlot] = iOldSpell;
				}

				this->m_pCharEntity->set_mapped_spells(spellSet);
			}
		}

		ImGui::Text("  %d", i % 10);

		ImGui::NextColumn();
	}

	ImGui::End();
}

void AbilityToolbar::do_ability_tooltip(ISpell *pSpell)
{
	ImGui::BeginTooltip();

	ImGui::TextUnformatted(pSpell->get_name().c_str());
	ImGui::Separator();
	ImGui::TextUnformatted(pSpell->get_desc().c_str());
	ImGui::Text("MP Cost: %0.0f", pSpell->get_mana_cost());
	ImGui::Text("Cooldown: %0.0f s", pSpell->get_cooldown());

	ImGui::EndTooltip();
};