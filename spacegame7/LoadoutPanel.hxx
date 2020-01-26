#pragma once

#include <sstream>

#include "InterfacePanel.hxx"

#include "CEquippedObject.hxx"
#include "CWorldTransaction.hxx"

class LoadoutPanel : public InterfacePanel
{
public:
	LoadoutPanel(InstanceId const iObjectId)
		: m_iObjectId(iObjectId)
	{};
	virtual ~LoadoutPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		CWorldTransaction wTransaction;

		if(!SG::get_engine()->instance_is_allocated(this->m_iObjectId))
		{
			return;
		}

		IWorldInstance *pInstance = SG::get_engine()->instance_get(this->m_iObjectId);

		if(pInstance == nullptr)
		{
			return;
		}

		ImGui::Begin("Equipment", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

		//pInstance->instance_acquired();

		//is our linked object a CEquippedObject?
		if(pInstance->instance_get_flags() & CEquippedObject::InstanceFlag)
		{
			CEquippedObject *pEquippedObject = static_cast<CEquippedObject*>(pInstance);

			std::vector<InstanceId> vObjectChildren = pEquippedObject->get_children();

			//now, walk each child of the equipped object (its equipment instances)
			for(InstanceId instanceId : vObjectChildren)
			{
				if(!SG::get_engine()->instance_is_allocated(instanceId))
				{
					continue;
				}

				CEquipInstance *pChildEquip = SG::get_engine()->instance_get_checked<CEquipInstance>(instanceId);

				if(pChildEquip == nullptr)
				{
					continue;
				}

				//pChildEquip->instance_acquired();

				Archetype const *pChildArch = pChildEquip->get_archetype();
				EquipArch const *pChildEquipArch = reinterpret_cast<EquipArch const*>(pChildArch);

				bool bChildEnabled = pChildEquip->is_alive();

				//the color will be either green or yellow depending on if the piece of
				//equipment is enabled
				ImVec4 vColor = bChildEnabled ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

				ImGui::PushID(instanceId);
				ImGui::PushStyleColor(ImGuiCol_Text, vColor);
				if(ImGui::Selectable(pChildEquipArch->szEquipName.c_str()))
				{
					pChildEquip->alive_set(!bChildEnabled);
				}
				ImGui::PopStyleColor(1);
				ImGui::PopID();

				//if the user hovers over a piece of equipment, display a tooltip
				//with the item's description and statistics
				if(ImGui::IsItemHovered())
				{
					LoadoutPanel::do_item_tooltip(pChildEquipArch);
				}

				//pChildEquip->instance_released();
			}
		}

		//pInstance->instance_released();

		ImGui::End();
	};

	static void do_item_tooltip(EquipArch const *pEquipArch)
	{
		ImGui::BeginTooltip();

		ImGui::Text(pEquipArch->szEquipName.c_str());

		ImGui::Separator();

		switch(pEquipArch->uiArchType)
		{
			case ARCH_WEAPON:
			{
				WeaponArch const *pWeapArch = reinterpret_cast<WeaponArch const*>(pEquipArch);
				ImGui::Text("Weapon");
				ImGui::Text(pEquipArch->szEquipDescription.c_str());
				ImGui::Text("Hull DPS:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->get_hull_dps()).c_str());
				ImGui::Text("Shield DPS:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->get_shield_dps()).c_str());
				ImGui::Text("Range:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->get_range()).c_str());
				break;
			}
			case ARCH_MISSILE:
			{
				MissileArch const *pWeapArch = reinterpret_cast<MissileArch const*>(pEquipArch);
				ImGui::Text("Missile");
				ImGui::Text(pEquipArch->szEquipDescription.c_str());
				ImGui::Text("Hull DMG:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->flHullDamage).c_str());
				ImGui::Text("Shield DMG:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->flShieldDamage).c_str());
				ImGui::Text("Accel.:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->flMotorAcceleration).c_str());
				ImGui::Text("Seek Time:");
				ImGui::SameLine();
				ImGui::Text(Conversion::float_to_string(pWeapArch->flMotorLifetime).c_str());
				//ImGui::Text("Range:");
				//ImGui::SameLine();
				//ImGui::Text(Conversion::float_to_string(pWeapArch->get_range()).c_str());
				break;
			}
			default:
			{

			}
		}
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
};