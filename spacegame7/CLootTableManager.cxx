#include "CLootTableManager.hxx"
#include "SGLib.hxx"

CLootTableManager::CLootTableManager()
{
	
}

CLootTableManager::~CLootTableManager()
{
	
}

void CLootTableManager::roll_drops(DropTableId const iDropTableId, QuantityMap &drops, unsigned int &uiMetal)
{
	if(this->m_vDropTables.size() <= iDropTableId)
	{
		throw SGException("Attempted to roll drops for a nonexistent drop table");
	}

	DropTable &dropTable = this->m_vDropTables[iDropTableId];

	DropProbabilities::iterator i;
	DropProbabilities &dropProbs = dropTable.dropProbs;

	for(i = dropProbs.begin(); i != dropProbs.end(); ++i)
	{
		ArchId archId = i->first;
		float flChance = i->second;

		/*
		 * Generate a random float between 0 and 1.
		 * If the roll falls within 0 and the drop chance,
		 * then increment the item quantity in the drops list.
		 */
		std::uniform_real_distribution<float> dist_roll(0.0f, 1.0f);
		float flRoll = dist_roll(SG::get_random());

		if(flRoll <= flChance)
		{
			++drops[archId];
		}
	}

	/*
	 * Determine metal with a simple random uint between the min metal
	 * and max metal.
	 */
	std::uniform_int_distribution<unsigned int> dist_metal(dropTable.dropMetalMin, dropTable.dropMetalMax);
	uiMetal = dist_metal(SG::get_random());
}

void CLootTableManager::create_drop_table(DropTableId const iDropTableId)
{
	if(this->m_vDropTables.size() <= iDropTableId)
	{
		this->m_vDropTables.resize(iDropTableId + 1);
	}
}

void CLootTableManager::add_drop(DropTableId const iDropTableId, ArchId const iArch, float const flChance)
{
	if(this->m_vDropTables.size() <= iDropTableId)
	{
		throw SGException("Attempted to add drop to nonexistent loot table");
	}

	this->m_vDropTables[iDropTableId].dropProbs[iArch] = flChance;
}

void CLootTableManager::set_metal_chance(DropTableId const iDropTableId, unsigned int const uiMetalMin, unsigned int const uiMetalMax)
{
	if(this->m_vDropTables.size() <= iDropTableId)
	{
		throw SGException("Attempted to set min/max metal for nonexistent loot table");
	}

	this->m_vDropTables[iDropTableId].dropMetalMin = uiMetalMin;
	this->m_vDropTables[iDropTableId].dropMetalMax = uiMetalMax;
}