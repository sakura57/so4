/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "DebugCommands.hxx"
#include "SGLib.hxx"
#include "CBaseTransitionState.hxx"
#include "CSectorTransitionState.hxx"
#include <sstream>

void populate_debug_commands()
{
	static bool bDebugCommandsPopulated = false;

	if(bDebugCommandsPopulated)
	{
		throw SGException("Duplicate call to populate_debug_commands()");
	}

	IDebugConsole* pDebugConsole = SG::get_debug_console();

	pDebugConsole->console_register_command(
		"help",
		[pDebugConsole](std::vector<std::string> const &args) {
			std::string titleString("");

			titleString.append(GAME_TITLE_STRING);
			titleString.append(" ver. ");
			titleString.append(GAME_VERSION_STRING);

			pDebugConsole->console_write_line(titleString);
			pDebugConsole->console_write_line("Please refer to the documentation for debug console usage and commands.");
		}
	);

	pDebugConsole->console_register_command(
		"warp",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 4;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			SectorId sectorId;
			Vector2f spawnPosition;

			try
			{
				sectorId = std::stoi(args[1]);
				spawnPosition.x = std::stof(args[2]);
				spawnPosition.y = std::stof(args[3]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				pDebugConsole->console_write_line("Warping to sector ID " + sectorId);

				SG::get_game_state_manager()->transition_game_state(new CSectorTransitionState(sectorId, "", spawnPosition));
			}
		}
	);

	pDebugConsole->console_register_command(
		"land",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 2;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			BaseId baseId;

			try
			{
				baseId = std::stoi(args[1]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				pDebugConsole->console_write_line("Landing on base ID " + baseId);

				SG::get_game_state_manager()->transition_game_state(new CBaseTransitionState(baseId));
			}
		}
	);

	pDebugConsole->console_register_command(
		"tele",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 3;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			Vector2f spawnPosition;

			try
			{
				spawnPosition.x = std::stof(args[1]);
				spawnPosition.y = std::stof(args[2]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				IWorld* pWorld = SG::get_world();

				pWorld->begin_world_transaction();

				InstanceId uiPlayer = pWorld->get_player_unchecked();

				if(uiPlayer != INVALID_INSTANCE)
				{
					CShip* pPlayerShip = SG::get_engine()->instance_get_checked<CShip>(uiPlayer);

					if(pPlayerShip != nullptr)
					{
						pPlayerShip->set_position(spawnPosition);
					}
				}

				pWorld->end_world_transaction();
			}
		}
	);

	pDebugConsole->console_register_command(
		"cash",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 2;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			unsigned int cash;

			try
			{
				cash = std::stoi(args[1]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				pDebugConsole->console_write_line("OK");

				SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory()->adjust_money(cash);
			}
		}
	);

	pDebugConsole->console_register_command(
		"metal",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 2;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			unsigned int metal;

			try
			{
				metal = std::stoi(args[1]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				pDebugConsole->console_write_line("OK");

				SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory()->adjust_metal(metal);
			}
		}
	);

	pDebugConsole->console_register_command(
		"give",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 3;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			ArchId arch;
			unsigned int quantity;

			try
			{
				arch = std::stoi(args[1]);
				quantity = std::stoi(args[2]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				pDebugConsole->console_write_line("OK");

				SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory()->add_item(arch, quantity);
			}
		}
	);

	pDebugConsole->console_register_command(
		"script",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 2;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}


			pDebugConsole->console_write_line("OK");

			SG::get_script_engine()->script_enqueue(args[1].c_str());
		}
	);

	pDebugConsole->console_register_command(
		"exp",
		[pDebugConsole](std::vector<std::string> const& args) {
			unsigned int const expectedArguments = 2;

			if(args.size() != expectedArguments)
			{
				pDebugConsole->console_write_line("Expected " + Conversion::uint_to_string(expectedArguments - 1) + " arguments");

				return;
			}

			bool inputValid = true;
			unsigned long expToGain;

			try
			{
				expToGain = std::stoi(args[1]);
			}
			catch(std::invalid_argument & e)
			{
				inputValid = false;
			}

			if(!inputValid)
			{
				pDebugConsole->console_write_line("Invalid argument format");
			}
			else
			{
				pDebugConsole->console_write_line("OK");

				SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->gain_exp(expToGain);
			}
		}
	);

	bDebugCommandsPopulated = true;
}