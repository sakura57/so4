#include "IMaterialClass.hxx"
#include <unordered_map>
#include <algorithm>

std::string IMaterialClass::get_material_type_name(MaterialType const matType)
{
	std::unordered_map<MaterialType, std::string> static const typeNameMap = {
		{ MaterialType::INVALID, "Invalid" },
		{ MaterialType::UNKNOWN, "Unknown" },
		{ MaterialType::MANUFACTURED, "Manufactured" },
		{ MaterialType::METAL, "Metallic" },
		{ MaterialType::INORGANIC, "Inorganic" },
		{ MaterialType::ORGANIC, "Organic" }
	};

	auto nameStringI = typeNameMap.find(matType);

	if (nameStringI != typeNameMap.end())
	{
		return nameStringI->second;
	}

	return "Invalid";
}

std::string IMaterialClass::get_material_type_nick(MaterialType const matType)
{
	std::unordered_map<MaterialType, std::string> static const typeNameMap = {
		{ MaterialType::INVALID, "INVALID" },
		{ MaterialType::UNKNOWN, "UNKNOWN" },
		{ MaterialType::MANUFACTURED, "MANUFACTURED" },
		{ MaterialType::METAL, "METALLIC" },
		{ MaterialType::INORGANIC, "INORGANIC" },
		{ MaterialType::ORGANIC, "ORGANIC" }
	};

	auto nameStringI = typeNameMap.find(matType);

	if(nameStringI != typeNameMap.end())
	{
		return nameStringI->second;
	}

	return "INVALID";
}

MaterialType IMaterialClass::get_material_type_from_nick(std::string const& matType)
{
	std::unordered_map<std::string, MaterialType> static const typeNameMap = {
		{ "INVALID", MaterialType::INVALID },
		{ "UNKNOWN", MaterialType::UNKNOWN },
		{ "MANUFACTURED", MaterialType::MANUFACTURED },
		{ "METALLIC", MaterialType::METAL },
		{ "INORGANIC", MaterialType::INORGANIC },
		{ "ORGANIC", MaterialType::ORGANIC }
	};

	auto classId = typeNameMap.find(matType);

	if (classId != typeNameMap.end())
	{
		return classId->second;
	}

	return MaterialType::INVALID;
}

std::string IMaterialClass::get_material_attrib_name(MaterialAttribute const matType)
{
	std::unordered_map<MaterialAttribute, std::string> static const attribNameMap = {
		{ MaterialAttribute::INVALID, "Invalid" },
		{ MaterialAttribute::INTEGRITY, "Integrity" },
		{ MaterialAttribute::DENSITY, "Density" },
		{ MaterialAttribute::THERMAL_CONDUCTIVITY, "Thermal Conductivity" },
		{ MaterialAttribute::ELECTRICAL_CONDUCTIVITY, "Electrical Conductivity" },
		{ MaterialAttribute::MALLEABILITY, "Malleability" },
		{ MaterialAttribute::SHOCK_RESISTANCE, "Shock Resistance" }
	};

	auto nameStringI = attribNameMap.find(matType);

	if(nameStringI != attribNameMap.end())
	{
		return nameStringI->second;
	}

	return "Invalid";
}

std::string IMaterialClass::get_material_attrib_nick(MaterialAttribute const matType)
{
	std::unordered_map<MaterialAttribute, std::string> static const attribNameMap = {
		{ MaterialAttribute::INVALID, "INVALID" },
		{ MaterialAttribute::INTEGRITY, "INTEGRITY"},
		{ MaterialAttribute::DENSITY, "DENSITY" },
		{ MaterialAttribute::THERMAL_CONDUCTIVITY, "THERMAL_CONDUCTIVITY" },
		{ MaterialAttribute::ELECTRICAL_CONDUCTIVITY, "ELECTRICAL_CONDUCTIVITY" },
		{ MaterialAttribute::MALLEABILITY, "MALLEABILITY" },
		{ MaterialAttribute::SHOCK_RESISTANCE, "SHOCK_RESISTANCE" }
	};

	auto nameStringI = attribNameMap.find(matType);

	if(nameStringI != attribNameMap.end())
	{
		return nameStringI->second;
	}

	return "INVALID";
}

MaterialAttribute IMaterialClass::get_material_attrib_from_nick(std::string const& matType)
{
	std::unordered_map<std::string, MaterialAttribute> static const attribNameMap = {
		{ "INVALID", MaterialAttribute::INVALID },
		{ "INTEGRITY", MaterialAttribute::INTEGRITY},
		{ "DENSITY", MaterialAttribute::DENSITY },
		{ "THERMAL_CONDUCTIVITY", MaterialAttribute::THERMAL_CONDUCTIVITY },
		{ "ELECTRICAL_CONDUCTIVITY", MaterialAttribute::ELECTRICAL_CONDUCTIVITY },
		{ "MALLEABILITY", MaterialAttribute::MALLEABILITY },
		{ "SHOCK_RESISTANCE", MaterialAttribute::SHOCK_RESISTANCE }
	};

	auto classId = attribNameMap.find(matType);

	if(classId != attribNameMap.end())
	{
		return classId->second;
	}

	return MaterialAttribute::INVALID;
}