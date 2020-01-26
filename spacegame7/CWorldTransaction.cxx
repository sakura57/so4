#include "CWorldTransaction.hxx"
#include <iostream>

CWorldTransaction::CWorldTransaction()
{
	SG::get_world()->begin_world_transaction();
}

CWorldTransaction::~CWorldTransaction()
{
	SG::get_world()->end_world_transaction();
}