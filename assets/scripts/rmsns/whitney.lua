--Random mission script
local math = require "math"

local player = sgs_world_get_player()
local characters = {10, 11, 12}
local character = characters[sgs_random_int(1, #characters)]
local freighter_character = 20

local comms_ambush = {
"Power down and prepare to be boarded.\nYour cargo is mine.",
"Stand down and surrender your cargo.",
"Jettison your cargo, and I may consider sparing you...",
"I see you've got an escort. No matter.",
"Don't make things difficult for yourself. Jettison it all!",
"My my, what do we have here?\nSurrender now, or die.",
"Watch in awe, as I turn you both into space dust!"
}

local comms_ambush_response = {
"Incoming, incoming! Ah, damn it, just my luck.",
"Shit, pirates always gotta ruin a good run.",
"Take him out, take him out! I'm too young to die!",
"Here I was thinking we'd make the checkpoint in one piece...",
"Holy flaming fuck on a stick! Pirates!",
"Damn it all to hell! We've got incoming, deploy hardpoints!",
"Like hell if I'll die to a grimy pirate like you!\nPowering up weapons.",
"We're all in now, friend. Get those guns online."
}

local comms_personas_first = {
"Well you're a sight for sore eyes.\nThe pirates don't let up in this damn sector.",
"Greetings. HQ radioed in just now, I guess you're our escort.",
"I told them no way I'm flying to the checkpoint alone, now they finally listen.",
"I got my own escort today? Damn, HQ doesn't want me dead after all!",
"Thank God for escorts.\nJovian sectors give me the shivers.",
"Is it true, I mean the pirates in this sector? This is my first run through here."
}

local comms_personas_second = {
"Alright, enough chit-chat. Time to hit the road.",
"All set? Let's head out, cargo ain't gonna deliver itself.",
"Uploading the checkpoint coordinates now.\nStay close, and keep your guns hot.",
"Time to get going, don't want to get any angry messages from the commissioner this time.",
"Chop chop now, checkpoint's waiting."
}

local comms_thanks = {
"Phew, that was a close one.\nI thought I was a goner for sure!",
"Nothing like a pirate ambush to get the blood pumping!",
"I mark one bogey as sunk.\nDamn fine shooting, pilot!",
"Scratch one bandit.\nNice shooting, you saved my ass!",
"Thank God you were here!\nIf you weren't, I...",
"Damn pirates. This is why I hate this sector...",
"Target destroyed! Thanks, I owe you one!",
"Take that, you sorry son of a bitch! Yee-haw!"
}

local comms_enroute = {
"Now, let's get back to the route.\nThe checkpoint awaits!",
"Alright, enough dilly-dallying. Back to the route!\n",
"Now that that's over, we can get back to the run.\nUploading the coordinates.",
"Damn, I'm still sweating!\nNext checkpoint, let's go!"
}

local comms_madeit = {
"We made it!\nAnd, miraculously, in one piece!\nI can't thank you enough, pilot.",
"Here we are, the checkpoint.\nMany thanks, pilot. I can take it from here.",
"Alright, I've got it from here.\nThanks again for saving my skin!",
"Another day, another checkpoint.\nAlright, I've got it from here.",
"I'll take it from here.\nNice work today, pilot.",
"Here it is. Alright, until next time\nFind me in the Whitney bar sometime, first round's on me!"
}

local over_condition = 0

local ambush = sgs_random_int(1, #comms_ambush)
local response = sgs_random_int(1, #comms_ambush_response)
local persona1 = sgs_random_int(1, #comms_personas_first)
local persona2 = sgs_random_int(1, #comms_personas_second)
local thanks = sgs_random_int(1, #comms_thanks)
local enroute = sgs_random_int(1, #comms_enroute)
local madeit = sgs_random_int(1, #comms_madeit)

local check1x, check1y = 15000, 10000
local check2x, check2y = 27500, 10000

local freighter = sgs_ship_create(freighter_character, 5000.0, 200.0, 90.0)
local freighter_name = sgs_ship_get_name(freighter)
local pirate = 0
local pirate_name = ""
sgs_enqueue_callback(1, "mission_init")

function mission_init()
	sgs_object_set_target(player, freighter)

	sgs_send_comm(freighter_name, comms_personas_first[persona1])
	sgs_send_comm(freighter_name, comms_personas_second[persona2])
	sgs_object_set_attitude(freighter, player, 0.7)
	sgs_enqueue_callback(1, "mission_failed_angered_freighter")
	sgs_enqueue_callback(1, "mission_failed_freighter_died")
	sgs_enqueue_callback(1, "mission_failed_freighter_abandoned")
	sgs_enqueue_callback(18, "mission_freighter_move_to_first_checkpoint")
end

function mission_failed_angered_freighter()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	if sgs_object_get_attitude(freighter, player) > -0.45 then
		sgs_enqueue_callback(1, "mission_failed_angered_freighter")

		return
	end

	sgs_send_comm("Whitney Station", "Mission failed.\nYou were supposed to protect the convoy, not shoot it!")
	sgs_object_set_attitude(freighter, player, -0.7)

	over_condition = 1
end

function mission_failed_freighter_abandoned()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	if not (sgs_object_get_distance_to_object(freighter, player) > 5000) then
		sgs_enqueue_callback(1, "mission_failed_freighter_abandoned")

		return
	end

	sgs_send_comm("Whitney Station", "Mission failed.\nWhere are you going? You're supposed to escort the convoy!")

	over_condition = 1
end

function mission_failed_freighter_died()
	if not sgs_object_exists(player) or over_condition == 1 then
		return
	end

	if sgs_object_exists(freighter) then
		sgs_enqueue_callback(1, "mission_failed_freighter_died")

		return
	end

	sgs_send_comm("Whitney Station", "Mission failed.\nWe've lost contact with the convoy. I repeat, the convoy is lost.")
	sgs_object_set_attitude(freighter, player, -0.7)

	over_condition = 1
end

function mission_freighter_move_to_first_checkpoint()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	sgs_object_goto_directive(freighter, check1x, check1y, 500.0)
	sgs_waypoint_set(check1x, check1y)

	sgs_enqueue_callback(1, "mission_freighter_arrive_first_checkpoint")
end

function mission_freighter_arrive_first_checkpoint()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	if not (sgs_object_get_distance_to_point(freighter, check1x, check1y) < 1500) then
		sgs_enqueue_callback(1, "mission_freighter_arrive_first_checkpoint")

		return
	end

	sgs_object_chill_directive(freighter)

	pirate = sgs_ship_create(character, check1x + 1000, check1y, 180.0)
	pirate_name = sgs_ship_get_name(pirate)

	sgs_object_set_attitude(pirate, player, -0.7)
	sgs_object_set_attitude(pirate, freighter, -0.7)
	sgs_object_set_attitude(freighter, pirate, -0.7)

	sgs_send_comm(pirate_name, comms_ambush[ambush])
	sgs_send_comm(freighter_name, comms_ambush_response[response])
	
	sgs_waypoint_remove()

	sgs_enqueue_callback(1, "mission_ambush_dead")
end

function mission_ambush_dead()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	if sgs_object_exists(pirate) then
		sgs_enqueue_callback(1, "mission_ambush_dead")
		return
	end

	local freighterx, freightery = sgs_object_get_position(freighter)

	sgs_waypoint_set(freighterx, freightery)

	sgs_send_comm(freighter_name, comms_thanks[thanks])
	sgs_send_comm(freighter_name, comms_enroute[enroute])
	sgs_enqueue_callback(18, "mission_freighter_move_to_second_checkpoint")
end

function mission_freighter_move_to_second_checkpoint()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	sgs_object_goto_directive(freighter, check2x, check2y, 500.0)
	sgs_waypoint_set(check2x, check2y)

	sgs_enqueue_callback(1, "mission_freighter_arrive_second_checkpoint")
end

function mission_freighter_arrive_second_checkpoint()
	if not sgs_object_exists(player) or not sgs_object_exists(freighter) or over_condition == 1 then
		return
	end

	if not (sgs_object_get_distance_to_point(freighter, check2x, check2y) < 1500) then
		sgs_enqueue_callback(1, "mission_freighter_arrive_second_checkpoint")

		return
	end

	over_condition = 1

	sgs_waypoint_remove()

	sgs_object_chill_directive(freighter)

	sgs_send_comm(freighter_name, comms_madeit[madeit])

	sgs_enqueue_callback(8, "mission_transfer_reward")
end

function mission_transfer_reward()
	sgs_send_comm("Whitney Station", "All primary objectives completed\nThank you, pilot.")
	sgs_send_comm("Whitney Station", "Transferring your reward now.\nWhitney Control out.")
	sgs_waypoint_set(5000, 0)
	sgs_enqueue_callback(8, "mission_actually_give_money")
end

function mission_actually_give_money()
	sgs_player_adjust_money(10000)
	sgs_send_notification("Money: +10000")
end