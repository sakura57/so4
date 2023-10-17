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

#include "SGLib.hxx"
#include "CShip.hxx"
#include "ICharacterEntity.hxx"
#include "CWorldTransaction.hxx"

class ShipStatusPanel : public InterfacePanel
{
public:
	ShipStatusPanel(InstanceId const iObjectId)
		: m_iObjectId(iObjectId)
	{
		this->m_pEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();
	};
	virtual ~ShipStatusPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		CWorldTransaction wTransaction;

		if (!SG::get_engine()->instance_is_allocated(this->m_iObjectId))
		{
			return;
		}

		IWorldInstance *pInstance = SG::get_engine()->instance_get(this->m_iObjectId);

		if (pInstance == nullptr)
		{
			return;
		}

		ImGui::Begin("Systems", nullptr, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

		//is our linked object a CShip?
		if (pInstance->instance_get_flags() & CShip::InstanceFlag)
		{
			CShip *pShip = static_cast<CShip*>(pInstance);
			ShipArch const *pShipArch = pShip->get_ship_arch();
			

			float flHealth = pShip->get_hit_pts();
			float flShields = pShip->get_shield_pts();

			float flMaxHealth = pShipArch->flMaxHitPoints;
			float flMaxShields = pShipArch->flMaxShieldPoints;

			float flCapacitors = pShip->get_capacitor_energy();
			float flMaxCapacitors = pShipArch->flCapacitorEnergy;

			float flMind = this->m_pEntity->get_mana();
			float flMaxMind = this->m_pEntity->get_max_mana();

			std::string szMind = Conversion::float_to_string(flMind);
			std::string szMaxMind = Conversion::float_to_string(flMaxMind);

			ImVec4 mindGaugeColor(1.0f - (flMind / flMaxMind), 0.5f * flMind / flMaxMind, 1.0f * flMind / flMaxMind, 1.0f);
			std::string szMindText = (flMind > 0.0f) ? (szMind + "/" + szMaxMind) : "Exh.";

			ImGui::Text("Mind:");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, mindGaugeColor);
			ImGui::Text(szMindText.c_str());
			ImGui::PopStyleColor(1);
			//ImGui::Text(pShipArch->szShipShortName.c_str());

			ImGui::Separator();
			ShipStatusPanel::do_indicators(flHealth, flMaxHealth, flShields, flMaxShields, flCapacitors, flMaxCapacitors);

			ICharacterEntity *pCharEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

			//ImVec4 energyGaugeColor((flCapacitors / flMaxCapacitors), 0.5f * (flCapacitors / flMaxCapacitors), 1.0f - flCapacitors / flMaxCapacitors, 1.0f);

			ShipStatusPanel::do_status_effects(pShip);
		}

		ImGui::End();
	};

	static void do_status_effects(CEquippedObject * pEqObj)
	{
		std::list<IStatusEffect *> lStatusEffects = pEqObj->get_status_effects();
		for(IStatusEffect *pStatusEffect : lStatusEffects)
		{
			StaticTextureId iconTexId = pStatusEffect->get_icon_texture();
			void * pTextureHandle = SG::get_static_texture_manager()->get_texture_handle(iconTexId);
			ImGui::Image(pTextureHandle, ImVec2(16.0f, 16.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

			if(ImGui::IsItemHovered())
			{
				ShipStatusPanel::do_status_tooltip(pStatusEffect);
			}

			ImGui::SameLine();
		}
	};

	static void do_indicators(float const flHealth, float const flMaxHealth, float const flShields, float const flMaxShields, float const flCapacitors, float const flMaxCapacitors)
	{
		ImVec4 hullGaugeColor(1.0f - flHealth / flMaxHealth, (flHealth / flMaxHealth), 0.0f, 1.0f);
		ImVec4 shieldsGaugeColor(1.0f - flShields / flMaxShields, (flShields / flMaxShields), 0.0f, 1.0f);
		ImVec4 energyGaugeColor(1.0f, (flCapacitors / flMaxCapacitors), 0.0f, 1.0f);

		std::string szHull = Conversion::float_to_string(flHealth);
		std::string szMaxHull = Conversion::float_to_string(flMaxHealth);
		std::string szShields = Conversion::float_to_string(flShields);
		std::string szMaxShields = Conversion::float_to_string(flMaxShields);
		std::string szCapacitors = Conversion::float_to_string(flCapacitors);
		std::string szMaxCapacitors = Conversion::float_to_string(flMaxCapacitors);

		std::string szHullText = (flHealth > 0.0f) ? (szHull + "/" + szMaxHull) : "Dead";
		std::string szShieldsText = (flShields > 0.0f) ? (szShields + "/" + szMaxShields) : "Down";
		std::string szCapacitorsText = (flCapacitors > 0.0f) ? (szCapacitors + "/" + szMaxCapacitors) : "Down";

		ImGui::Text("Hull:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, hullGaugeColor);
		ImGui::Text(szHullText.c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text("Shields:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, shieldsGaugeColor);
		ImGui::Text(szShieldsText.c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text("Energy:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, energyGaugeColor);
		ImGui::Text(szCapacitorsText.c_str());
		ImGui::PopStyleColor(1);
	};

	static void do_indicators_simple(float const flHealth, float const flMaxHealth, float const flShields, float const flMaxShields)
	{
		ImVec4 hullGaugeColor(1.0f - flHealth / flMaxHealth, (flHealth / flMaxHealth), 0.0f, 1.0f);
		ImVec4 shieldsGaugeColor(1.0f - flShields / flMaxShields, (flShields / flMaxShields), 0.0f, 1.0f);

		std::string szHull = Conversion::float_to_string(flHealth);
		std::string szMaxHull = Conversion::float_to_string(flMaxHealth);
		std::string szShields = Conversion::float_to_string(flShields);
		std::string szMaxShields = Conversion::float_to_string(flMaxShields);

		std::string szHullText = (flHealth > 0.0f) ? (szHull + "/" + szMaxHull) : "Dead";
		std::string szShieldsText = (flShields > 0.0f) ? (szShields + "/" + szMaxShields) : "Down";

		ImGui::Text("Hull:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, hullGaugeColor);
		ImGui::Text(szHullText.c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text("Shields:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, shieldsGaugeColor);
		ImGui::Text(szShieldsText.c_str());
		ImGui::PopStyleColor(1);
	};

	static void do_status_tooltip(IStatusEffect *pStatusEffect)
	{
		ImGui::BeginTooltip();

		ImGui::Text(pStatusEffect->get_name().c_str());

		ImGui::Separator();

		ImGui::TextUnformatted(pStatusEffect->get_desc().c_str());
		
		ImGui::EndTooltip();
	};

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	InstanceId const m_iObjectId;
	ICharacterEntity *m_pEntity;
};