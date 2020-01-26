#pragma once

#include <unordered_map>

enum class Stat : unsigned int
{
	INVALID,
	EVASION,
	HULL_INTEGRITY,
	SHIELD_KNOWLEDGE,
	LASER_PROFICIENCY,
	PLASMA_PROFICIENCY,
	KINETIC_PROFICIENCY,
	MISSILE_PROFICIENCY,
	NEGOTIATION,
	HACKING,
	LUCK
};

class CStatBlock
{
public:
	CStatBlock();
	~CStatBlock();

	void set_stat(Stat const, float const);
	float get_stat(Stat const) const;
	void format_stat_text(std::string &);
	void format_stat_data(std::string&);

	void merge(CStatBlock const *);
	bool empty(void);

	static Stat get_stat_from_name(std::string const &);
	static std::string get_stat_long_name(Stat const);
	static std::string get_stat_short_name(Stat const);
private:
	std::unordered_map<Stat, float> m_mStatMap;
};