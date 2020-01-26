#include "CShip.hxx"
#include "CProjectile.hxx"
#include "CShield.hxx"
#include "CEngineFlame.hxx"
#include "CInSpaceState.hxx"

#include <random>

CShip::CShip(void)
{
	CEquippedObject::CEquippedObject();
}

CShip::~CShip(void)
{
	//nothing to do
}

void CShip::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		IPhysicalObject::InstanceFlag |
		CObject::InstanceFlag |
		CEquippedObject::InstanceFlag |
		CShip::InstanceFlag |
		IRadarVisible::InstanceFlag |
		ILiving::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

void CShip::instance_destroy(InstanceId const iInstanceId)
{
	CEquippedObject::instance_destroy(iInstanceId);
}

void CShip::initialize(CreationParameters const &creationParameters)
{
	if(creationParameters.pArch == nullptr)
	{
		throw SGException("Cannot create CShip with null arch pointer");
	}

	if(creationParameters.flHitPoints <= 0.0f)
	{
		throw SGException("Cannot create CShip with initial hit points <= 0");
	}

	this->m_pShipArch = creationParameters.pArch;
	this->m_flHitPoints = creationParameters.flHitPoints;
	this->m_flShieldPoints = creationParameters.flShieldPoints;
	this->m_szRadarClass = creationParameters.szRadarClass;
	this->m_flThrottle = 0.0f;
	this->m_flSpinThrottle = 0.0f;
	this->m_flStrafeThrottle = 0.0f;
	this->m_bSmokeEmitted = false;
	this->m_flSmokeTimer = -1.0f;
	this->m_iTargetId = INVALID_INSTANCE;
	this->m_flShieldRegenTimeRemaining = -1.0f;
	this->m_bIsPlayer = creationParameters.bIsPlayer;
	this->m_uiInvulnerable = creationParameters.uiInvulnerable;
	this->m_bIsJumping = false;
	this->m_uiDropTable = creationParameters.uiDropTable;

	this->m_szName = creationParameters.szName;

	/*
	 * create our engine flame effect object
	 */
	CEngineFlame *pEngFlame;
	CInstanceFactory::create<CEngineFlame>(this->m_iEngineFlameInstanceId, pEngFlame);

	CEngineFlame::CreationParameters engineParms;
	engineParms.iParentInstanceId = this->m_iInstanceId;
	engineParms.uiAppearanceId = 10u;

	pEngFlame->initialize(engineParms);

	SG::get_world()->instance_enqueue(pEngFlame);

	/*
	* create our shield effect object
	*/
	CShield *pShield;
	CInstanceFactory::create<CShield>(this->m_iShieldInstanceId, pShield);

	CShield::CreationParameters shieldParms;
	shieldParms.iParentInstanceId = this->m_iInstanceId;
	shieldParms.uiAppearanceId = this->m_pShipArch->uiShieldAppearance;

	pShield->initialize(shieldParms);

	SG::get_world()->instance_enqueue(pShield);

	CEquippedObject::initialize(creationParameters);
}

float CShip::get_hit_pts(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flHitPoints;
}

void CShip::set_hit_pts(float const flHitPoints)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flHitPoints = flHitPoints;
}

float CShip::get_shield_pts(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flShieldPoints;
}

void CShip::set_shield_pts(float const flShieldPoints)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flShieldPoints = flShieldPoints;
}

float CShip::get_relative_health(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flHitPoints / this->m_pShipArch->flMaxHitPoints;
}

void CShip::set_relative_health(float const flRelHitPoints)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flHitPoints =  flRelHitPoints * this->m_pShipArch->flMaxHitPoints;
}

ShipArch const* CShip::get_ship_arch(void) const
{
	return this->m_pShipArch;
}

void CShip::alive_tick(float const flDelta)
{
	this->m_mFieldAccess.lock();

	if(this->m_flShieldPoints < 0.0f)
	{
		if(this->m_flShieldRegenTimeRemaining < 0.0f)
		{
			this->m_flShieldPoints = this->m_pShipArch->flShieldRegenPercent * this->m_pShipArch->flMaxShieldPoints;
		}
		else
		{
			this->m_flShieldRegenTimeRemaining -= flDelta;
		}
	}
	else
	{
		//begin shield delta with the typical regen rate
		float flShieldDelta = flDelta * this->m_pShipArch->flShieldRegenRate;

		//apply shield drain from status effects
		flShieldDelta -= flDelta * this->get_shield_drain_percent() * this->m_pShipArch->flMaxShieldPoints;
		this->m_flShieldPoints += flShieldDelta;
		this->m_flShieldPoints = clamp(this->m_flShieldPoints, 0, this->m_pShipArch->flMaxShieldPoints);
	}

	//apply health drain from status effects
	{
		float flHullDelta = -flDelta * this->get_hull_drain_percent() * this->m_pShipArch->flMaxHitPoints;

		this->m_flHitPoints += flHullDelta;

		//could run a HP=0 then kill check here,
		//but status effects are incapable of killing
		//outright

		this->m_flHitPoints = clamp(this->m_flHitPoints, 0.1f, this->m_pShipArch->flMaxHitPoints);
	}

	if(this->m_flHitPoints < (0.5f * this->m_pShipArch->flMaxHitPoints))
	{
		Vector2f vEnginePosition = this->m_vPosition + Matrix2f::rotation_matrix(this->m_flRotation) * Vector2f(-48.0f, 0.0f);

		if (this->m_flSmokeTimer < 0.0f)
		{
			std::uniform_real_distribution<float> dist_rot(-180.0f, 180.0f);
			float flRotation = dist_rot(SG::get_random());

			SG::get_particle_manager()->add_particle(5, vEnginePosition, Vector2f(0.0f, 0.0f), flRotation, 0.0f);

			this->m_flSmokeTimer = 0.05f;
		}
		else
		{
			this->m_flSmokeTimer -= flDelta;
		}

		if(!this->m_bSmokeEmitted)
		{
			std::uniform_real_distribution<float> dist_rot(-180.0f, 180.0f);
			float flRotation = dist_rot(SG::get_random());

			SG::get_particle_manager()->add_particle(4, vEnginePosition, this->m_vVelocity, flRotation, 0.0f);

			SG::get_audio_manager()->play_sound_spatialized(4, this->m_vPosition, 10.0f, 100.0f);

			this->m_bSmokeEmitted = true;
		}
	}

	this->m_mFieldAccess.unlock();

	CEquippedObject::alive_tick(flDelta);
}

float CShip::get_throttle(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flThrottle;
}

void CShip::set_throttle(float const flThrottle)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	float flNewThrottle = clamp(flThrottle, -1.0f, 1.0f);

	this->m_flThrottle = flNewThrottle;
}

void CShip::set_throttle_unclamped(float const flThrottle)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flThrottle = flThrottle;
}

float CShip::get_strafe_throttle(void)
{
	return this->m_flStrafeThrottle;
}

void CShip::set_strafe_throttle(float const flStrafeThrottle)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flStrafeThrottle = flStrafeThrottle;
}

float CShip::get_spin_throttle(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flSpinThrottle;
}

void CShip::set_spin_throttle(float const flSpinThrottle)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flSpinThrottle = flSpinThrottle;
}

void CShip::physics_tick(float const flDelta)
{
	this->m_mFieldAccess.lock();

	if (this->m_bAlive)
	{
		//we will set our acceleration and angular acceleration based on our throttle values.
		Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(this->m_flRotation);
		Vector2f vVelUnit = this->m_vVelocity.normalize();
		Vector2f vAccelUnit = mRotationMatrix * Vector2f(-this->m_flThrottle, 0.0f);
		float flDeltaRatio = clamp(acos(vVelUnit.dot(vAccelUnit)), 0.4f, 2.8f);

		float flThrottleMultiplier = this->get_engine_thrust_multiplier() * 2.0f / ((0.025f * flDeltaRatio * (this->m_vVelocity.magnitude())) + 2.0f);
		float flSpinThrottleMultiplier = this->get_engine_thrust_multiplier() * 2.0f / ((0.01f * abs(this->m_flAngularVelocity)) + 1.0f);

		Vector2f flStrafeFactor = this->get_engine_thrust_multiplier() * (this->m_flStrafeThrottle * 0.5f) * Vector2f(0.0f, this->m_pShipArch->flEngineForce);

		Vector2f vNewAcceleration =
		flThrottleMultiplier * (Vector2f(
			this->m_flThrottle
				* this->m_pShipArch->flEngineForce,
			0.0f
		) + flStrafeFactor)
		/ this->m_pShipArch->flMass;

		float flNewAngularAccel = this->m_flSpinThrottle * this->m_pShipArch->flEngineTorque / this->m_pShipArch->flRotationalInertia * flSpinThrottleMultiplier;

		this->m_flAngularAcceleration = flNewAngularAccel;
		this->m_vAcceleration = mRotationMatrix * vNewAcceleration;
	}

	this->m_mFieldAccess.unlock();

	//invoke the base physics_tick
	CObject::physics_tick(flDelta);
}

void CShip::collision_callback(IWorldObject *pOtherObject)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bMarkedForDeletion || !this->m_bAlive)
	{
		return;
	}

	//did we just collide with a projectile?
	if(pOtherObject->instance_get_flags() & CProjectile::InstanceFlag)
	{
		CProjectile *pProjectile = static_cast<CProjectile*>(pOtherObject);

		if(!pProjectile->get_dealt_damage() && pProjectile->get_parent_instance() != this->m_iInstanceId)
		{
			WeaponArch const *pArch = pProjectile->get_weapon_arch();

			//first apply the damage multiplier, which is usually 1.0 but for projectiles
			//originating from the player, will depend on their stat modifiers
			float flFinalHullDamage = pArch->flHullDamage * pProjectile->get_damage_multiplier();
			float flFinalShieldDamage = pArch->flShieldDamage * pProjectile->get_damage_multiplier();

			this->take_damage(flFinalHullDamage, flFinalShieldDamage);

			pProjectile->set_dealt_damage();

			if(!this->m_uiInvulnerable && this->m_flHitPoints < 0.0f)
			{
				InstanceId otherId = pProjectile->get_parent_instance();

				if(this->m_bIsPlayer)
				{
					CInSpaceState *pGameState = static_cast<CInSpaceState *>(SG::get_game_state_manager()->get_game_state());

					if(pGameState != nullptr)
					{
						DeathCause deathCause;

						if(!pProjectile->is_missile())
						{
							deathCause = DeathCause::GUNFIRE;
						}
						else
						{
							deathCause = DeathCause::MISSILE;
						}

						pGameState->build_death_string(otherId, deathCause);
					}
				}

				/*
				 * This section determines if the final blow was delivered by the player,
				 * in which case we should award loot.
				 */
				CShip *pAttackingShip = SG::get_engine()->instance_get_checked<CShip>(otherId);

				if(pAttackingShip != nullptr && pAttackingShip->is_player())
				{
					unsigned int uiMetalAward;
					QuantityMap qItemsAward;

					ILootTableManager *pLootTableManager = SG::get_loot_table_manager();

					pLootTableManager->roll_drops(this->m_uiDropTable, qItemsAward, uiMetalAward);

					ICharacterEntity *pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();
					IEntityInventory *pPlayerInventory = pPlayerEntity->get_inventory();
					pPlayerInventory->add_items(qItemsAward);

					/*
					 * Apply the player's luck stat to determine how much additional
					 * metal they can loot.
					 */
					float flMetalMultiplier = 1.0f;
					flMetalMultiplier += pPlayerEntity->get_stat(Stat::LUCK) * 0.01f;

					unsigned int uiFinalMetalAward = (unsigned int)(flMetalMultiplier * uiMetalAward);

					pPlayerInventory->adjust_metal(uiFinalMetalAward);

					unsigned long ulExpValue = this->m_ulExpValue;

					//Send a text notification to the game state, so we can provide
					//UI feedback that the player has gained metal and items
					std::stringstream ss;

					if(ulExpValue)
					{
						ss << "+" << ulExpValue << " EXP";
					}

					if(uiMetalAward)
					{
						ss << std::endl << "+" << uiMetalAward << " Metal";
					}

					for(QuantityMap::iterator i = qItemsAward.begin(); i != qItemsAward.end(); ++i)
					{
						ss << std::endl << "Item: " << SG::get_game_data_manager()->get_arch<Archetype>(i->first)->get_name();
					}

					std::string szNotification(ss.str());
					SG::get_game_state_manager()->get_game_state()->state_send_notification(szNotification);

					//Audio feedback, coin drop sound
					SG::get_audio_manager()->play_sound(13);


					//Add the actual EXP after the kill
					pPlayerEntity->gain_exp(ulExpValue);
				}

				this->ship_destroy();
			}
			else
			{
				InstanceId otherId = pProjectile->get_parent_instance();

				float flCurrentAttitude = this->m_attitudeSet.get_attitude(otherId);
				float flNewAttitude = flCurrentAttitude - (pArch->flHullDamage + pArch->flShieldDamage) / (ATTITUDE_DAMAGE_TOLERANCE);

				this->m_attitudeSet.set_attitude(otherId, clamp(flNewAttitude, ATTITUDE_LOATHING, ATTITUDE_LOVING));
			}
		}
	}
}

/*
 * triggered internally when the ship reaches zero hull
 */
void CShip::ship_destroy()
{
	if(!this->m_bAlive)
	{
		return;
	}

	IWorldInstance *pEngInstance = SG::get_engine()->instance_get(this->m_iEngineFlameInstanceId);
	IWorldInstance *pShieldInstance = SG::get_engine()->instance_get(this->m_iShieldInstanceId);
	
	pEngInstance->mark_for_deletion();
	pShieldInstance->mark_for_deletion();

	this->m_iEngineFlameInstanceId = INVALID_INSTANCE;
	this->m_iShieldInstanceId = INVALID_INSTANCE;

	this->m_bAlive = false;

	//notify the EquippedObject of deletion,
	//so child equipment can be destroyed
	this->notify_deletion();

	Matrix2f mRotation = Matrix2f::rotation_matrix(this->m_flRotation);

	Vector2f vVelocityEstimate = this->estimate_velocity();

	//std::uniform_real_distribution<float> dist_rot(0.0f, 0.2f);
	//std::uniform_real_distribution<float> dist_velx(-0.5f, 0.5f);
	//std::uniform_real_distribution<float> dist_vely(8.0f, 14.0f);

	//std::mt19937 &random = SG::get_random();

	//Vector2f vFinalVelocityPiece1 = vVelocityEstimate + (mRotation * Vector2f(dist_velx(random), dist_vely(random)));
	//Vector2f vFinalVelocityPiece2 = vVelocityEstimate + (mRotation * Vector2f(dist_velx(random),  -dist_vely(random)));

	//SG::get_particle_manager()->add_particle(2, this->m_vPosition, vFinalVelocityPiece1, this->m_flRotation, dist_rot(random) - 0.1f);
	//SG::get_particle_manager()->add_particle(3, this->m_vPosition, vFinalVelocityPiece2, this->m_flRotation, dist_rot(random) - 0.1f);
	SG::get_particle_manager()->add_particle(1, this->m_vPosition, vVelocityEstimate, this->m_flRotation, 0.0f);

	SG::get_audio_manager()->play_sound_spatialized(4, this->m_vPosition, 1.0f, 500.0f);

	this->m_bMarkedForDeletion = true;

	this->listeners_send({ Notification::Type::DEPENDENT_DESTROYED });
	this->listeners_clear();
}

void CShip::inflict_damage(float const flHullDamage, float const flShieldDamage)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->take_damage(flHullDamage, flShieldDamage);
}

void CShip::take_damage(float const flHullDamage, float const flShieldDamage)
{
	if(this->m_flShieldPoints > 0.0f)
	{
		CShield *pShield = static_cast<CShield*>(this->get_shield());

		if(pShield)
		{
			pShield->register_hit();
		}

		if(this->m_uiInvulnerable != 2)
		{
			this->m_flShieldPoints -= flShieldDamage * this->get_shield_damage_multiplier();
		}

		if(this->m_flShieldPoints < 0.0f)
		{
			this->m_flShieldRegenTimeRemaining = this->m_pShipArch->flShieldRegenTime;
		}
	}
	else
	{
		if(this->m_uiInvulnerable != 2)
		{
			this->m_flHitPoints -= flHullDamage * this->get_hull_damage_multiplier();
		}

		if(this->m_uiInvulnerable == 1)
		{
			float flThreshold = 0.1f;

			if(this->m_flHitPoints < flThreshold)
			{
				this->m_flHitPoints = flThreshold;
			}
		}
	}

	if (!this->m_uiInvulnerable && this->m_flHitPoints < 0.0f)
	{
		if (this->m_bIsPlayer)
		{
			CInSpaceState* pGameState = static_cast<CInSpaceState*>(SG::get_game_state_manager()->get_game_state());

			if (pGameState != nullptr)
			{
				DeathCause deathCause = DeathCause::OTHER;

				pGameState->build_death_string(INVALID_INSTANCE, deathCause);
			}

			this->ship_destroy();
		}
	}
}

void CShip::flick_shields(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	CShield* pShield = static_cast<CShield*>(this->get_shield());

	if (pShield)
	{
		pShield->register_hit();
	}
}