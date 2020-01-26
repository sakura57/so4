#pragma once

#include "Defs.hxx"
#include <vector>
#include <string>
#include "MaterialType.hxx"
#include "Character.hxx"

typedef unsigned int AstfieldId;

class CAstfield
{
public:
	struct CreationParameters
	{
		std::string szName;
		std::string szDescription;
		MaterialType matType;
		float flDropProbability;
		unsigned int uiMinDropQuantity;
		unsigned int uiMaxDropQuantity;
		float flPirateAttackChancePerTick;
		float flRadius;
		std::vector<CharacterId> vPirateEncounters;
	};

	CAstfield();
	CAstfield(CreationParameters const &);
	CAstfield(std::string const&, std::string const&, MaterialType const, float const, unsigned int const, unsigned int const, float const, std::vector<CharacterId> const &, float const);
	~CAstfield();

	std::string get_name(void) const;
	std::string get_desc(void) const;
	MaterialType get_mat_type(void) const;
	float get_drop_probability(void) const;
	unsigned int get_min_drop_quantity(void) const;
	unsigned int get_max_drop_quantity(void) const;
	float get_pirate_attack_chance_per_tick(void) const;
	float get_radius(void) const;
	std::vector<CharacterId> get_pirate_encounters(void) const;

private:
	std::string m_szName;
	std::string m_szDescription;
	MaterialType m_matType;
	float m_flDropProbability;
	unsigned int m_uiMinDropQuantity;
	unsigned int m_uiMaxDropQuantity;
	float m_flPirateAttackChancePerTick;
	float m_flRadius;
	std::vector<CharacterId> m_vPirateEncounters;
};