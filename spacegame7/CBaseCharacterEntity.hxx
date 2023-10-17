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

#include "Loadout.hxx"
#include "ICharacterEntity.hxx"
#include "ISpell.hxx"
#include "CStatBlock.hxx"
#include "IMaterialBank.hxx"
#include <unordered_map>

class CBaseCharacterEntity : public ICharacterEntity
{
public:
	CBaseCharacterEntity(Loadout const *, std::string const &, bool const);
	virtual ~CBaseCharacterEntity();

	/*
	* get the inventory corresponding to this entity
	*/
	virtual IEntityInventory *get_inventory(void);

	virtual IMaterialBank* get_material_bank(void);

	virtual Loadout get_loadout(void);

	virtual void demount_item(int const) final;
	virtual void mount_item(int const, ArchId const) final;

	virtual void set_loadout(Loadout const &) final;

	virtual unsigned int get_ship(void) final;
	virtual void set_ship(unsigned int const) final;

	virtual SpellSet get_known_spells(void) final;
	virtual SpellSet get_mapped_spells(void) final;

	virtual void set_mapped_spells(SpellSet const &) final;
	virtual void learn_spell(SpellId const) final;
	virtual void reset_spells(void) final;

	virtual void cast_spell(int const, InstanceId const, InstanceId const) final;
	virtual bool can_cast_spell(int const) final;
	virtual float get_spell_cooldown_time_remaining(int const) final;

	virtual void tick(float const) final;

	virtual float get_mana(void) final;
	virtual void set_mana(float const) final;
	virtual float get_max_mana(void) final;
	virtual int get_level(void) final;
	virtual void set_level(int const) final;
	virtual void gain_exp(unsigned long int const) final;
	virtual unsigned long get_exp_to_next_level(void) final;
	virtual unsigned long get_current_exp(void) final;

	virtual void set_name(std::string const &) final;
	virtual std::string get_name(void) final;

	virtual CStatBlock *get_stat_block(void) final;

	virtual bool has_skill(SkillId const) final;
	virtual bool grant_skill(SkillId const) final;
	virtual bool give_skill(SkillId const) final;
	virtual unsigned int get_skill_points_available(void) final;
	virtual void set_free_skill_points(unsigned int const) final;

	virtual float get_stat(Stat const) final;
	virtual void set_stats(CStatBlock const&) final;

	virtual void get_skills(std::vector<SkillId> &) final;
	virtual void clear_skills(void) final;

protected:
	Spinlock m_mFieldAccess;
	Loadout m_lLoadout;
	bool m_bPlayer;
	std::string m_szName;
	SpellSet m_vKnownSpells;
	SpellSet m_vMappedSpells;
	std::unordered_map<SpellId, float> m_vSpellCooldown;
	float m_flMana;
	float m_flMaxMana;
	float m_flManaRegen;
	CStatBlock *m_pStatBlock;
	unsigned int m_uiShipId;
	unsigned int m_uiFreeSkillPoints;

	unsigned long int m_ulCurrentExp;
	unsigned long int m_ulExpToNextLevel;
	int m_iLevel;

	std::vector<SkillId> m_vSkills;

	static unsigned long int get_exp_from_level(int const);
	static float get_mana_at_level(int const);
private:
	void update_mana(void);
};