#pragma once

#include "Util.hxx"
#include "IEntityInventory.hxx"
#include "Loadout.hxx"
#include "ISpell.hxx"
#include "CStatBlock.hxx"
#include "CSkill.hxx"
#include "IMaterialBank.hxx"

typedef unsigned int CharacterEntityId;

/*
 * Interface describing a character.
 */
interface ICharacterEntity
{
	/*
	 * get the inventory corresponding to this entity
	 */
	virtual IEntityInventory *get_inventory(void) = 0;

	virtual IMaterialBank* get_material_bank(void) = 0;

	virtual SpellSet get_known_spells(void) = 0;
	virtual SpellSet get_mapped_spells(void) = 0;

	virtual void set_mapped_spells(SpellSet const &) = 0;
	virtual void learn_spell(SpellId const) = 0;
	virtual void reset_spells(void) = 0;

	virtual void cast_spell(int const, InstanceId const, InstanceId const) = 0;
	virtual bool can_cast_spell(int const) = 0;
	virtual float get_spell_cooldown_time_remaining(int const) = 0;

	virtual Loadout get_loadout(void) = 0;

	virtual void demount_item(int const) = 0;
	virtual void mount_item(int const, ArchId const) = 0;

	virtual void set_loadout(Loadout const &) = 0;

	virtual void tick(float const) = 0;

	virtual float get_mana(void) = 0;
	virtual void set_mana(float const) = 0;
	virtual float get_max_mana(void) = 0;
	virtual int get_level(void) = 0;
	virtual void gain_exp(unsigned long int const) = 0;
	virtual unsigned long get_exp_to_next_level(void) = 0;
	virtual unsigned long get_current_exp(void) = 0;
	virtual void set_level(int const) = 0;

	virtual void set_name(std::string const &) = 0;
	virtual std::string get_name() = 0;

	virtual CStatBlock *get_stat_block(void) = 0;

	virtual unsigned int get_ship(void) = 0;
	virtual void set_ship(unsigned int const) = 0;

	virtual bool has_skill(SkillId const) = 0;
	virtual bool grant_skill(SkillId const) = 0;
	virtual bool give_skill(SkillId const) = 0;
	virtual unsigned int get_skill_points_available(void) = 0;
	virtual void set_free_skill_points(unsigned int const) = 0;
	virtual void get_skills(std::vector<SkillId>&) = 0;
	virtual void clear_skills(void) = 0;

	virtual float get_stat(Stat const) = 0;
	virtual void set_stats(CStatBlock const&) = 0;
};