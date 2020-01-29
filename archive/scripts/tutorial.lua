--This script is always executed by the game.
--Register any important callbacks.

--spawn Roberts' ship
local roberts_ship = sgs_ship_create(1, 100.0, 100.0, 20.0)
local pirate_ship = sgs_ship_create(2, 4600.0, 2000.0, -40.0)
sgs_object_set_attitude(roberts_ship, sgs_world_get_player(), 0.7)
local attacker1 = -1
local attacker2 = -1
local ally1 = -1
local attackers_left = 0
sgs_enqueue_callback(1.0, "tutorial_init")
--sgs_enqueue_callback(1.0, "tutorial4_spawn_attackers")

function tutorial_init()
	sgs_enqueue_callback(1.0, "tutorial1_comm")
	sgs_enqueue_callback(1.5, "tutorial_roberts_died")
	sgs_enqueue_callback(1.0, "tutorial_pirate_died")
end

function tutorial_roberts_died()
	if not sgs_object_exists(roberts_ship) then
		sgs_send_comm("CS Bremerhaven", "Bremerhaven to Delta 5.\nWe lost Roberts on radar.\nIs he... no...")
		sgs_send_comm("CS Bremerhaven", "Return to base at once.\nBremerhaven out.")
	else
		sgs_enqueue_callback(1.0, "tutorial_roberts_died")
	end
end

function tutorial_pirate_died()
	if not sgs_object_exists(pirate_ship) then
		tutorial3_comm()
	else
		sgs_enqueue_callback(1.0, "tutorial_pirate_died")
	end
end

function tutorial1_comm()
	sgs_send_comm("CS Bremerhaven", "This is the Bremerhaven.\nLaunch sequence complete.\nGood luck out there, Delta 5!")
	sgs_send_comm("Roberts", "Alright, let's get a move on.\nCommand said the unknown contact was to the east.")
	sgs_send_comm("Roberts", "Follow me.\nDon't shoot unless I give the order, but keep your guns hot.")

	--one comm takes 7.5 seconds
	sgs_enqueue_callback(15.0, "tutorial1_move_to_pirate")
end

function tutorial1_move_to_pirate()
	sgs_object_goto_directive(roberts_ship, 4600.0, 1300.0, 500.0)
	sgs_enqueue_callback(22.0, "tutorial2_comm")
end

function tutorial2_comm()
	sgs_send_comm("Roberts", "Unidentified vessel, this is TSF Patrol Delta 5.\nIdentify yourself.")
	sgs_send_comm("Unknown Ship", "Terrans, you'll never understand.\nYour system is crumbling...\nand your time is up.")
	sgs_send_comm("Roberts", "He's warming up his guns!\nWe have no other choice, engage!")
	sgs_enqueue_callback(20.5, "tutorial2_pirate_engages")
end

function tutorial2_pirate_engages()
	sgs_object_set_attitude(roberts_ship, pirate_ship, -0.7)
	sgs_object_set_attitude(pirate_ship, roberts_ship, -0.7)
	sgs_object_set_attitude(pirate_ship, sgs_world_get_player(), -0.7)
	sgs_object_set_target(sgs_world_get_player(), pirate_ship)
end

function tutorial3_comm()
	sgs_send_comm("Roberts", "That was a close one.\nNice work.\nLet's head back to the Bremerhaven.")
	sgs_enqueue_callback(7.5, "tutorial3_back_to_base")
end

function tutorial3_back_to_base()
	sgs_object_goto_directive(roberts_ship, 100.0, 0.0, 500.0)
	sgs_enqueue_callback(22.0, "tutorial4_comm")
end

function tutorial4_comm()
	sgs_send_comm("CS Bremerhaven", "Bremerhaven to Delta 5.\nWhat happened out there?\nWhat's your status?")
	sgs_send_comm("Roberts", "We're fine.\nThe ship attacked us.\nWe had to shoot him down.")
	sgs_send_comm("CS Bremerhaven", "Copy that Delta 5.\nYou did what you had to do.\nBremerhaven out.")
	sgs_send_comm("Roberts", "Damned right.\nAlright kid, bring your ship in for...")
	sgs_send_comm("CS Bremerhaven", "Delta 5, we see two new fighter contacts closing fast from the east.")
	sgs_send_comm("Roberts", "Damn it, there's more of them.\nGuns hot, prepare to defend.")
	sgs_enqueue_callback(36.0, "tutorial4_spawn_attackers")
end

function tutorial4_spawn_attackers()
	attacker1 = sgs_ship_create(6, 3800.0, 2900.0, 180.0)
	attacker2 = sgs_ship_create(3, 4000.0, 2100.0, 180.0)
	ally1 = sgs_ship_create(4, -1800.0, 2100.0, -20.0)
	
	attackers_left = 2
	
	sgs_object_goto_directive(attacker1, 100.0, 0.0, 500.0)
	sgs_object_goto_directive(attacker2, 100.0, 0.0, 500.0)
	sgs_object_goto_directive(ally1, 100.0, 0.0, 500.0)
	
	sgs_object_set_attitude(ally1, sgs_world_get_player(), 0.7)
	
	sgs_object_set_attitude(roberts_ship, attacker1, -0.7)
	sgs_object_set_attitude(attacker1, roberts_ship, -0.7)
	sgs_object_set_attitude(ally1, attacker1, -0.7)
	sgs_object_set_attitude(attacker1, ally1, -0.7)
	sgs_object_set_attitude(attacker1, sgs_world_get_player(), -0.7)
	
	sgs_object_set_attitude(roberts_ship, attacker2, -0.7)
	sgs_object_set_attitude(attacker2, roberts_ship, -0.7)
	sgs_object_set_attitude(ally1, attacker2, -0.7)
	sgs_object_set_attitude(attacker2, ally1, -0.7)
	sgs_object_set_attitude(attacker2, sgs_world_get_player(), -0.7)
	
	sgs_enqueue_callback(12.0, "tutorial4_attackers")
	sgs_enqueue_callback(7.0, "tutorial4_check_end")
	sgs_enqueue_callback(3.0, "tutorial4_attacker1_dead")
	sgs_enqueue_callback(5.0, "tutorial4_attacker2_dead")
end

function tutorial4_attackers()
	sgs_send_comm("Saber Pilot", "This is the end for you, Terrans.")
end

function tutorial4_attacker1_dead()
	if not sgs_object_exists(attacker1) then
		attackers_left = attackers_left - 1
		--sgs_send_comm("Roberts", "One down!")
	else
		sgs_enqueue_callback(3.0, "tutorial4_attacker1_dead")
	end
end

function tutorial4_attacker2_dead()
	if not sgs_object_exists(attacker2) then
		attackers_left = attackers_left - 1
		--sgs_send_comm("Roberts", "One ship cooked!")
	else
		sgs_enqueue_callback(5.0, "tutorial4_attacker2_dead")
	end
end

function tutorial4_check_end()
	if attackers_left == 0 then
		sgs_send_comm("Roberts", "That's the last of them.\nGood shooting, kid.")
		sgs_object_goto_directive(roberts_ship, 100.0, 0.0, 500.0)
		sgs_object_goto_directive(ally1, -100.0, 50.0, 500.0)
	else
		sgs_enqueue_callback(7.0, "tutorial4_check_end")
	end
end

io.write(string.format("Main script started\nRunning %s\n", _VERSION))