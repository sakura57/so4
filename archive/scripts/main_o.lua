--Random mission script
local math = require "math"

local player = sgs_world_get_player()
local characters = {10, 11, 12, 13, 14, 15, 16}
local character = characters[sgs_random_int(1, #characters)]

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

local comms_reinforcements = {
"If you think you can take out our boss, you're mistaken.",
"You should never have come here.\nThe boss is off limits.",
"The boss is off limits.\nPrepare to get wiped off the map.",
"You're not taking out the boss.\nThis is the end for you.",
"What do we have here?\nTSF scum think they can take out our boss?",
"You might as well kill your engines, your ship is getting shredded no matter what.",
"When will the TSF learn?\nThis won't end well for you."
}

local comms_personas = {
", who's been a thorn in our side for quite some time.",
", a pirate ace who needs to be put down.",
", whose lackeys have been causing us problems lately.",
", an individual we can't afford to leave at large.",
", the leader of a local Reavers contingent.",
", whose gang needs to be taught a lesson.",
", a defector who needs to be eliminated immediately.",
". We need this individual dealt with.",
", a Reaver leader who needs to be wiped off the map.",
", whose little gang has overstepped its bounds.",
", who we've put up with for far too long.",
", whose little raider gang must come to an end.",
", whose existence we are no longer willing to tolerate.",
", whose gang we've grown quite tired of."
}

local greeting = sgs_random_int(1, #comms_greetings)
local persona = sgs_random_int(1, #comms_personas)

local xpre, ypre = sgs_random_float(-1.0, 1.0), sgs_random_float(-1.0, 1.0)
local radius = math.sqrt(xpre * xpre + ypre * ypre)
local kx, ky = xpre / radius, ypre / radius
local dist = 15000 + sgs_random_float(-2000, 2000)
local tarx, tary = dist * kx, dist * ky

local target_ship = sgs_ship_create(character, tarx, tary, 180.0)
local target_name = sgs_ship_get_name(target_ship)
sgs_enqueue_callback(1, "mission_init")

function mission_init()
	sgs_send_comm("CS Bremerhaven", "This is the Bremerhaven.\nLaunch sequence complete.")
	sgs_send_comm("CS Bremerhaven", "Your target is " .. target_name .. comms_personas[persona])
	sgs_send_comm("CS Bremerhaven", "We're uploading their last known coordinates.\nBremerhaven out.")
	sgs_enqueue_callback(20.0, "mission_upload_coords")
	sgs_enqueue_callback(1.0, "mission_await_player")
end

function mission_upload_coords()
	sgs_waypoint_set(tarx, tary)
end

function mission_await_player()
	if not sgs_object_exists(player) then
		return
	end
	
	local dist = sgs_object_get_distance_to_object(player, target_ship)
	
	if dist < 1000.0 then
		sgs_send_comm(target_name, comms_greetings[greeting])
		sgs_object_set_attitude(target_ship, player, -0.7)
		sgs_enqueue_callback(10.0, "mission_reinforcements")
		sgs_waypoint_remove()
	else
		sgs_enqueue_callback(1.0, "mission_await_player")
	end
end

function mission_reinforcements()
	if not sgs_object_exists(player) then
		return
	end
	
	local character2 = characters[sgs_random_int(1, #characters)]
	local x, y = sgs_object_get_position(player)
	local vx, vy = sgs_object_get_velocity(player)
	local m = math.sqrt(vx * vx + vy * vy)
	local px, py = (vx / m) * 2000 + x, (vy / m) * 2000 + y
	
	local target2_ship = sgs_ship_create(character2, px, py, 180.0)
	local reinforcement_name = sgs_ship_get_name(target2_ship)
	sgs_send_comm(reinforcement_name, comms_reinforcements[sgs_random_int(1, #comms_reinforcements)])
	sgs_object_set_attitude(target2_ship, player, -0.7)
end