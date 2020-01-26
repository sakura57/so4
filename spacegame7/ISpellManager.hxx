#pragma once

#include "Defs.hxx"

#include "ISpell.hxx"

interface ISpellManager
{
	virtual void add_spell(SpellId const, ISpell *) = 0;
	virtual ISpell *get_spell(SpellId const) = 0;
};