#pragma once

#include "IWorldInstance.hxx"
#include <unordered_map>
#include <vector>
#include "SGLib.hxx"
#include <mutex>

typedef float Attitude;
#define ATTITUDE_NEUTRAL 0.0f
#define ATTITUDE_LOATHING -0.7f
#define ATTITUDE_LOVING 0.7f
#define ATTITUDE_HOSTILE -0.45f
#define ATTITUDE_FRIENDLY 0.45f
#define THRESHOLD_HOSTILE -0.4f
#define THRESHOLD_FRIENDLY 0.4f
#define ATTITUDE_COLD -0.2f
#define ATTITUDE_WARMING 0.2f

class CAttitudeSet
{
public:
	CAttitudeSet()
	{};
	~CAttitudeSet()
	{};

	void initialize(Faction const *pMyFaction)
	{
		this->m_pMyFaction = pMyFaction;
	};

	void set_attitude(InstanceId const iInstance, Attitude const attitude)
	{
		if(iInstance >= this->m_mObjectAttitudes.size())
		{
			this->m_mObjectAttitudes.resize(iInstance + 1);
		}

		this->m_mObjectAttitudes[iInstance] = attitude;
	};

	Attitude get_attitude(InstanceId const iInstance)
	{
		if (iInstance >= this->m_mObjectAttitudes.size())
		{
			this->m_mObjectAttitudes.resize(iInstance + 1);

			this->m_mObjectAttitudes[iInstance] = ATTITUDE_NEUTRAL;

			return ATTITUDE_NEUTRAL;
		}

		return this->m_mObjectAttitudes[iInstance];
	};

	void cleanup_attitudes(void)
	{
		for(InstanceId inst = 0; inst < this->m_mObjectAttitudes.size(); ++inst)
		{
			if(!SG::get_engine()->instance_is_allocated(inst))
			{
				this->m_mObjectAttitudes[inst] = ATTITUDE_NEUTRAL;
			}
		}
	};

	static std::string get_attitude_descriptor(Attitude const attitude)
	{
		if (attitude < ATTITUDE_HOSTILE)
		{
			return "Hostile";
		}
		else if (attitude < ATTITUDE_FRIENDLY)
		{
			return "Friendly";
		}
		else
		{
			return "Neutral";
		}
	};

private:
	Faction const *m_pMyFaction;
	std::vector<Attitude> m_mObjectAttitudes;
};

/*
 * Attitude set object.
 * 
 * TODO: Make the whole attitude system better.
 * It is quite inefficient, with CEquippedObject
 * having get/set functions which just copy
 * around CAttitudeSet objects.
 */
/*class CAttitudeSet
{
public:
	CAttitudeSet()
	{};
	~CAttitudeSet()
	{};

	void initialize(Faction const *pMyFaction)
	{
		this->m_pMyFaction = pMyFaction;
	};

	void set_attitude(InstanceId const iInstance, Attitude const attitude)
	{
		this->m_mObjectAttitudes[iInstance] = attitude;
	};

	Attitude get_attitude(InstanceId const iInstance)
	{
		auto i = this->m_mObjectAttitudes.find(iInstance);

		if(i != this->m_mObjectAttitudes.end())
		{
			return i->second;
		}

		return ATTITUDE_NEUTRAL;
	};

	void cleanup_attitudes(void)
	{
		auto i = this->m_mObjectAttitudes.begin();

		while(i != this->m_mObjectAttitudes.end())
		{
			if(!SG::get_engine()->instance_is_allocated(i->first))
			{
				this->m_mObjectAttitudes.erase(i++);
			}
			else
			{
				++i;
			}
		}
	};

	static std::string get_attitude_descriptor(Attitude const attitude)
	{
		if(attitude < ATTITUDE_HOSTILE)
		{
			return "Hostile";
		}
		else if(attitude < ATTITUDE_FRIENDLY)
		{
			return "Friendly";
		}
		else
		{
			return "Neutral";
		}
	};

private:
	Faction const *m_pMyFaction;
	std::unordered_map<InstanceId, Attitude> m_mObjectAttitudes;
};*/