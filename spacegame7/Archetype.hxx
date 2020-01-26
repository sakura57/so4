#pragma once

#include <vector>
#include "IWorldObject.hxx"

typedef unsigned int ArchId;
typedef unsigned int ArchType;

#define ARCH_NULL 0U
#define ARCH_SHIP 1U
#define ARCH_EQUIP 2U
#define ARCH_WEAPON 3U
#define ARCH_MISSILE 4U

/*
 * Base Archetype struct, comprising just arch
 * id and type. No two Archetypes can have the same
 * id.
 * 
 * For simplicity, member access is public, but ALL usages
 * of Archetype structs outside of CGameDataManager
 * should be CONST. DO NOT MODIFY DATA OUTSIDE OF CGameDataManager.
 */
struct Archetype
{
	enum
	{
		Type = ARCH_NULL
	};

	ArchId uiArchId;
	ArchType uiArchType;
	int iValue;

	Archetype()
	{
		this->uiArchType = Type;
	};

	virtual std::string get_name(void) const = 0;
};

struct ShipArch : public Archetype
{
	enum
	{
		Type = ARCH_SHIP
	};

	AppearanceId uiShipAppearance;
	AppearanceId uiShieldAppearance;
	std::string szShipName;
	std::string szShipShortName;
	std::string szShipDescription;
	float flMass;
	float flRotationalInertia;
	float flMaxHitPoints;
	float flEngineForce;
	float flEngineTorque;
	float flMaxShieldPoints;
	std::vector<Vector2f> hardpoints;
	float flShieldRegenTime;
	float flShieldRegenRate;
	float flShieldRegenPercent;
	float flCapacitorEnergy;
	float flCapacitorRegenRate;

	ShipArch()
	{
		this->uiArchType = Type;
	};

	virtual std::string get_name(void) const final
	{
		return this->szShipName;
	};
};

struct EquipArch : public Archetype
{
	enum
	{
		Type = ARCH_EQUIP
	};

	std::string szEquipName;
	std::string szEquipDescription;

	EquipArch()
	{
		this->uiArchType = Type;
	};

	virtual std::string get_name(void) const final
	{
		return this->szEquipName;
	};
};

struct WeaponArch : public EquipArch
{
	enum
	{
		Type = ARCH_WEAPON
	};

	AppearanceId uiProjectileAppearance;
	float flShieldDamage;
	float flHullDamage;
	float flMuzzleVelocity;
	float flProjectileLifetime;
	float flRefireDelay;
	SoundId uiFireSound;
	float flEnergyCost;

	WeaponArch()
	{
		this->uiArchType = Type;
	};

	float get_hull_dps(void) const
	{
		return flHullDamage * (1.0f / flRefireDelay);
	};

	float get_shield_dps(void) const
	{
		return flShieldDamage * (1.0f / flRefireDelay);
	};

	float get_range(void) const
	{
		return flProjectileLifetime * flMuzzleVelocity;
	};
};

struct MissileArch : public WeaponArch
{
	enum
	{
		Type = ARCH_MISSILE
	};

	float flMotorLifetime;
	float flMotorDelay;
	float flMotorSound;
	float flMotorAcceleration;
	SoundId uiMotorSound;
	AnimationId uiMotorParticle;
	AnimationId uiMunitionExplosion;

	MissileArch()
	{
		this->uiArchType = Type;
	};
};