#pragma once

#include <sstream>

#include "ShipStatusPanel.hxx"
#include "InterfacePanel.hxx"

#include "CShip.hxx"
#include "CWorldTransaction.hxx"

class TargetPanel : public InterfacePanel
{
public:
	TargetPanel(InstanceId const iObjectId)
		: m_iObjectId(iObjectId)
	{};
	virtual ~TargetPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		CWorldTransaction wTransaction;

		if (!SG::get_engine()->instance_is_allocated(this->m_iObjectId))
		{
			return;
		}

		CShip *pShip = SG::get_engine()->instance_get_checked<CShip>(this->m_iObjectId);

		if (pShip == nullptr)
		{
			return;
		}

		InstanceId iObjectTarget = pShip->get_target();

		if (iObjectTarget != INVALID_INSTANCE && SG::get_engine()->instance_is_allocated(iObjectTarget))
		{
			CEquippedObject *pChildEquip = SG::get_engine()->instance_get_checked<CEquippedObject>(iObjectTarget);

			if(pChildEquip != nullptr)
			{

				bool bChildEnabled = pChildEquip->is_alive();

				if (bChildEnabled)
				{
					ImGui::Begin("Target", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);

					IRadarVisible *pRadarVisible = pChildEquip->get_radar();

					if(pRadarVisible)
					{
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), pRadarVisible->get_radar_name().c_str());
					}

					CShip *pTgtShip = SG::get_engine()->instance_get_checked<CShip>(iObjectTarget);

					ShipArch const *pShipArch = nullptr;

					ImGui::Text("Type:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), pChildEquip->get_object_name().c_str());

					if(pTgtShip)
					{
						pShipArch = pTgtShip->get_ship_arch();

						ImGui::Text("Ship:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), pShipArch->szShipShortName.c_str());
					}

					ImGui::Separator();

					if(pTgtShip)
					{
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), pTgtShip->get_name().c_str());
					}

					Faction const *pFaction = pChildEquip->get_faction();

					if(pFaction == nullptr)
					{
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Unaffiliated");
					}
					else
					{
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), pFaction->name.c_str());
					}

					ImGui::Text("Attitude:");
					ImGui::SameLine();

					Attitude attitude = pChildEquip->get_attitude_set().get_attitude(this->m_iObjectId);
					ImVec4 attColor;
					std::string szAttitude;

					if(attitude <= THRESHOLD_HOSTILE)
					{
						attColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
						szAttitude = "HOSTILE";
					}
					else if(attitude >= THRESHOLD_FRIENDLY)
					{
						attColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
						szAttitude = "FRIENDLY";
					}
					else
					{
						attColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
						szAttitude = "NEUTRAL";
					}

					ImGui::TextColored(attColor, szAttitude.c_str());

					ICharacterEntity *pEntity = pChildEquip->get_parent_entity();

					if(pEntity != nullptr)
					{
						ImGui::Separator();

						ImGui::TextColored(ImVec4(0.0f, 0.6f, 0.8f, 1.0f), "Elite Lvl %d", pEntity->get_level());

						float flMind = pEntity->get_mana();
						float flMaxMind = pEntity->get_max_mana();

						std::string szMind = Conversion::float_to_string(flMind);
						std::string szMaxMind = Conversion::float_to_string(flMaxMind);

						ImVec4 mindGaugeColor(1.0f - (flMind / flMaxMind), 0.5f * flMind / flMaxMind, 1.0f * flMind / flMaxMind, 1.0f);
						std::string szMindText = (flMind > 0.0f) ? (szMind + "/" + szMaxMind) : "Exh.";

						ImGui::Text("Mind:");
						ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, mindGaugeColor);
						ImGui::Text(szMindText.c_str());
						ImGui::PopStyleColor(1);
					}

					ImGui::Separator();

					if(pTgtShip)
					{
						ShipStatusPanel::do_indicators_simple(
							pTgtShip->get_hit_pts(), pShipArch->flMaxHitPoints,
							pTgtShip->get_shield_pts(), pShipArch->flMaxShieldPoints
						);
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
						ImGui::Text("Immortal Object");
						ImGui::PopStyleColor(1);
					}

					ShipStatusPanel::do_status_effects(pChildEquip);

					ImGui::End();
				}
			}
		}
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
};