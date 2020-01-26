#pragma once

#include <vector>
#include "ILootTableManager.hxx"

typedef std::unordered_map<ArchId, float> DropProbabilities;

struct DropTable
{
	unsigned int dropMetalMin, dropMetalMax;
	DropProbabilities dropProbs;
};

class CLootTableManager : public ILootTableManager
{
public:
	CLootTableManager();
	~CLootTableManager();

	virtual void roll_drops(DropTableId const, QuantityMap &, unsigned int &);
	virtual void create_drop_table(DropTableId const);
	virtual void add_drop(DropTableId const, ArchId const, float const);
	virtual void set_metal_chance(DropTableId const, unsigned int const, unsigned int const);
private:
	std::vector<DropTable> m_vDropTables;
};