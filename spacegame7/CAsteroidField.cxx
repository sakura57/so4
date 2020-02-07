#include "CAsteroidField.hxx"
#include "CShip.hxx"
#include "CAsteroidMineable.hxx"

#define MAX_ASTEROID_STATIC_SPARSITY 75
#define MAX_DYNAMIC_ASTEROIDS_SPARSITY 1000
#define MAX_DYNAMIC_ASTEROIDS_LIMIT_UPPER 20
#define MAX_DYNAMIC_ASTEROIDS_LIMIT_LOWER 2
#define DYNAMIC_ASTEROID_UPDATE_TIMER 5.0f

CAsteroidField::CAsteroidField(void)
{
	this->m_bInitialized = false;
}

CAsteroidField::~CAsteroidField(void)
{
	//we have nothing to free
}

void CAsteroidField::initialize(CAsteroidField::CreationParameters const& parms)
{
	this->m_uiAstfieldInstanceId = parms.uiAstfieldInstanceId;
	this->m_uiAppearanceId = parms.uiAppearanceId;
	this->m_flRadius = parms.flRadius;
	this->m_matType = parms.matType;
	this->m_uiDropMinQuantity = parms.uiDropMinQuantity;
	this->m_uiDropMaxQuantity = parms.uiDropMaxQuantity;
	this->m_flDropProbability = parms.flDropProbability;
	this->m_flPirateAttackChancePerTick = parms.flPirateAttackChancePerTick;
	this->m_vPirateEncounterCharacters = parms.vPirateEncounterCharacters;
	this->m_vPosition = parms.vPosition;

	unsigned int uiTentativeDynasts = (unsigned int)(this->m_flRadius / MAX_DYNAMIC_ASTEROIDS_SPARSITY);
	this->m_flDynastsTimer = -1.0f;

	if(uiTentativeDynasts > MAX_DYNAMIC_ASTEROIDS_LIMIT_UPPER)
	{
		uiTentativeDynasts = MAX_DYNAMIC_ASTEROIDS_LIMIT_UPPER;
	}
	else if(uiTentativeDynasts < MAX_DYNAMIC_ASTEROIDS_LIMIT_LOWER)
	{
		uiTentativeDynasts = MAX_DYNAMIC_ASTEROIDS_LIMIT_LOWER;
	}

	this->m_uiMaxDynamicAsteroids = uiTentativeDynasts;

	IMaterial* pMaterial = SG::get_material_manager()->pull_random_material_from_pool_hazy_associative(this->m_uiAstfieldInstanceId, this->m_matType);
	pMaterial->increment_reference_count();

	this->m_szMaterialSerial = pMaterial->get_serial_number();

	this->m_bPlayerPresent = false;

	this->spawn_asteroid_statics();

	CPhantomObject::initialize(parms);
}

void CAsteroidField::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		ILiving::InstanceFlag |
		CPhantomObject::InstanceFlag |
		CAsteroidField::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

void CAsteroidField::instance_destroy(InstanceId const iInstanceId)
{
	//remember to decrement ref count for our material
	IMaterial* pMaterial = SG::get_material_manager()->get_material_from_serial(this->m_szMaterialSerial);

	if(pMaterial)
	{
		pMaterial->decrement_reference_count();
	}

	CPhantomObject::instance_destroy(iInstanceId);
}

Vector2f CAsteroidField::get_position(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vPosition;
}

void CAsteroidField::set_position(Vector2f const& newPosition)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vPosition = newPosition;
}

float CAsteroidField::get_rotation(void)
{
	return 0.0f;
}

void CAsteroidField::set_rotation(float const newRotation)
{
	//do nothing
}

AppearanceId CAsteroidField::get_appearance()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return APPEARANCE_NONE;
}

void CAsteroidField::set_appearance(AppearanceId const newAppearance)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiAppearanceId = newAppearance;
}

void CAsteroidField::alive_tick(float const fDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	InstanceId uiPlayerId = SG::get_world()->get_player_unchecked();

	CShip* pPlayerShip = SG::get_engine()->instance_get_checked<CShip>(uiPlayerId);

	if(pPlayerShip != nullptr)
	{
		if(pPlayerShip->get_position().distance(this->m_vPosition) < this->m_flRadius)
		{
			if(!this->m_bPlayerPresent)
			{
				this->m_bPlayerPresent = true;
			}
		}
		else
		{
			if(this->m_bPlayerPresent)
			{
				this->m_bPlayerPresent = false;
			}
		}
	}

	if(this->m_bPlayerPresent)
	{
		this->update_dynamic_asteroids(fDelta);
	}
}

void CAsteroidField::alive_set(bool const)
{
	//do nothing; shield objects are always alive
}

void CAsteroidField::update_dynamic_asteroids(float const flDelta)
{
	this->m_flDynastsTimer -= flDelta;

	if(this->m_flDynastsTimer < 0.0f)
	{
		//time for an update

		std::vector<InstanceId> lUpdatedChildren;

		for(InstanceId childId : this->m_lChildIds)
		{
			if(SG::get_engine()->instance_is_allocated(childId))
			{
				lUpdatedChildren.push_back(childId);
			}
		}

		unsigned int childrenToCreate = this->m_uiMaxDynamicAsteroids - lUpdatedChildren.size();

		for(unsigned int i = 0; i < childrenToCreate; ++i)
		{
			lUpdatedChildren.push_back(this->create_randomized_dynast());
		}

		this->m_lChildIds = lUpdatedChildren;

		this->m_flDynastsTimer = DYNAMIC_ASTEROID_UPDATE_TIMER;
	}
}

void CAsteroidField::spawn_asteroid_statics(void)
{
	std::uniform_real_distribution<float> radDist(0.0f, this->m_flRadius);
	std::uniform_real_distribution<float> thetaDist(-180.0f, 180.0f);
	std::uniform_real_distribution<float> depthDist(-300.0f, 300.0f);

	unsigned int numAsteroids = (unsigned int)(this->m_flRadius / MAX_ASTEROID_STATIC_SPARSITY);

	std::vector<StaticTextureId> static const asteroidAppearances = 
	{
		2, 3, 4
	};

	Vector2f static const asteroidImageSize(512.0f, 512.0f);

	std::uniform_int_distribution<unsigned int> appearanceDist(0, asteroidAppearances.size() - 1);

	IWorld* pWorld = SG::get_world();

	for(size_t i = 0; i < numAsteroids; ++i)
	{
		Vector2f prerotationPosition(radDist(SG::get_random()), 0.0f);
		Matrix2f rotationMatrix = Matrix2f::rotation_matrix(thetaDist(SG::get_random()));
		Vector2f relativePosition = rotationMatrix * prerotationPosition;
		Vector2f absolutePosition = this->m_vPosition + relativePosition;
		float flDepth = depthDist(SG::get_random());
		float flRotation = thetaDist(SG::get_random());

		StaticTextureId appearanceId = asteroidAppearances[appearanceDist(SG::get_random())];

		pWorld->create_static(appearanceId, absolutePosition.x, absolutePosition.y, flDepth, flRotation, asteroidImageSize);
	}
}

InstanceId CAsteroidField::create_randomized_dynast(void)
{
	std::uniform_real_distribution<float> radDist(0.0f, this->m_flRadius);
	std::uniform_real_distribution<float> thetaDist(-180.0f, 180.0f);
	std::uniform_real_distribution<float> angVelDist(-1.0f, 1.0f);

	Vector2f prerotationPosition(radDist(SG::get_random()), 0.0f);
	Matrix2f rotationMatrix = Matrix2f::rotation_matrix(thetaDist(SG::get_random()));
	Vector2f relativePosition = rotationMatrix * prerotationPosition;
	Vector2f absolutePosition = this->m_vPosition + relativePosition;
	float flRotation = thetaDist(SG::get_random());
	float flAngularVelocity = angVelDist(SG::get_random());

	InstanceId astId;
	CAsteroidMineable* ast;
	CInstanceFactory::create<CAsteroidMineable>(astId, ast);

	CAsteroidMineable::CreationParameters parms;
	parms.vPosition = absolutePosition;
	parms.vVelocity = Vector2f(0.0f, 0.0f);
	parms.flRotation = flRotation;
	parms.flAngularVelocity = flAngularVelocity;
	parms.uiAppearanceId = 23U;
	parms.flRadius = 64.0f;
	parms.szName = "Asteroid";
	parms.pFaction = nullptr;
	parms.pPilot = nullptr;
	parms.bActivated = true;
	parms.szRadarClass = "Asteroid";
	parms.flCapacitorEnergy = 0.0f;
	parms.flCapacitorRegenRate = 0.0f;
	parms.ulExpValue = 0UL;
	parms.pParentEntity = nullptr;
	parms.pStartingLoadout = { };

	parms.flDropProbability = this->m_flDropProbability;
	parms.uiMinQuantity = this->m_uiDropMinQuantity;
	parms.uiMaxQuantity = this->m_uiDropMaxQuantity;
	parms.szMaterialSerial = this->m_szMaterialSerial;
	parms.uiAstfieldId = this->m_uiAstfieldInstanceId;

	ast->initialize(parms);

	SG::get_world()->instance_enqueue(ast);

	return astId;
}