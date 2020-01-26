local math = require "math"

local comms_greetings = {
"You're all the TSF sent?\nWarm up your guns, kid.",
"I'll turn that hunk of junk into space dust.\nLet's dance.",
"I hope this doesn't hurt too much.\nNothing personal.",
"I've shot down dozens of your kind before.\nTime for one more.",
"Hold onto your seat...\nyou're about to eat a whole lot of lead.",
"I see this is what it's come to.\nWell, let's make this quick.",
"You're no match for me, and you know it.\nLet's do this.",
"This is the end of the line for you.\nSay your prayers.",
"This is what happens when you mess with the Reavers.",
"You know that this isn't going to end well for you, right?",
"Understand that these are your final moments.\nSavor them.",
"More TSF scum?\nI'll turn you into a ball of fire just like the others."
}

local kills = 0

--Dynamic spawns section
local pi_attacker_chars_weak = {13, 14, 15}
local pi_attacker_chars_medium = {10, 11, 12}
local pi_attacker_chars_strong = {22, 23, 24}
local attacker = 0

function attacker_spawn()
	local player = sgs_world_get_player()

	if not sgs_object_exists(player) then
		return
	end

	local attacker_char = 13

	local player_level = sgs_player_get_level()

	if player_level < 2 then
		attacker_char = pi_attacker_chars_weak[sgs_random_int(1, #pi_attacker_chars_weak)]
	elseif player_level < 4 then
		attacker_char = pi_attacker_chars_medium[sgs_random_int(1, #pi_attacker_chars_medium)]
	else
		attacker_char = pi_attacker_chars_strong[sgs_random_int(1, #pi_attacker_chars_strong)]
	end

	local attacker_inbound_angle = sgs_random_float(-180.0, 180.0)
	local attacker_distance = sgs_random_float(300.0, 500.0)

	local player_x, player_y = sgs_object_get_position(player)

	local attacker_x_rel, attacker_y_rel = attacker_distance * math.cos(attacker_inbound_angle), attacker_distance * math.sin(attacker_inbound_angle)
	local attacker_x, attacker_y = player_x + attacker_x_rel, player_y + attacker_y_rel

	attacker = sgs_ship_create(attacker_char, attacker_x, attacker_y, attacker_inbound_angle + 180.0)
	sgs_object_set_attitude(attacker, player, -0.7)
	
	sgs_effect_jump_in(attacker_x, attacker_y)

	local greeting = sgs_random_int(1, #comms_greetings)

	sgs_send_comm(sgs_ship_get_name(attacker), comms_greetings[greeting])

	sgs_enqueue_callback(1.0, "attacker_check_dead")
end

function player_dock_deepspace_base()
	sgs_instant_dock(3)
end

function attacker_check_dead()
	if not sgs_object_exists(attacker) then
		if kills < 5 then
			kills = kills + 1
			sgs_send_comm("Arena", "Target eliminated.\nAnother target inbound!")
			sgs_enqueue_callback(10.0, "attacker_spawn")
		else
			sgs_send_comm("Arena", "That's enough for now.\nCome on in for a rearm and refuel!")
			sgs_enqueue_callback(10.0, "player_dock_deepspace_base")
		end
	else
		sgs_enqueue_callback(1.0, "attacker_check_dead")
	end
end

sgs_send_comm("Arena", "Welcome, pilot!\nBlast as many Reavers as you can!")
sgs_enqueue_callback(10.0, "attacker_spawn")