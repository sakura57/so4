--Random mission script
local math = require "math"

local player = sgs_world_get_player()
local characters = {17, 18, 19}
local character = characters[sgs_random_int(1, #characters)]

local comms_greetings = {
"Traitorous scum.\nYou will pay the ultimate price.",
"The TSF does not look kindly on traitors.",
"You've robbed your last freighter, pirate scum.",
"The TSF does not forgive, the TSF does not forget.",
"I've got a visual on the target. Moving to engage.",
"HQ, I have the target on radar. Engaging.",
"I have the target in my sights, missile lock acquired.",
"Capacitors hot, moving in to engage.",
"HQ, one bogey on radar, coming in hot.\nMoving to engage.",
"You know that this isn't going to end well for you, right?",
"If it isn't the traitor himself.\nPrepare to die.",
"Soon, you will be no more.\nI am the storm, maggot."
}

local comms_reinforcements = {
"Moving in to assist. Target lock engaged.",
"You won't touch the boss. I'll see to that.",
"This ends here and now.",
"You had your chance, but it's too late now.",
"Boss, you've got reinforcements.",
"I've got a visual. Moving to assist.",
"Target acquired. Moving to assist."
}

local comms_personas = {
", who's been a thorn in our side for quite some time.",
", a patrol leader who needs to be put down.",
", whose squadron has been causing us problems lately.",
", an individual we can't afford to leave at large.",
", the leader of a local TSF contingent.",
", whose squadron needs to be taught a lesson.",
", a defector who needs to be eliminated immediately.",
". We need this individual dealt with.",
", a TSF captain who needs to be wiped off the map.",
", whose little squad has overstepped its bounds.",
", who we've put up with for far too long.",
", whose little patrol gang must come to an end.",
", whose existence we are no longer willing to tolerate.",
", whose existence we've grown quite tired of."
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
	sgs_send_comm("Spire", "This is Reaver HQ.\nLaunch sequence complete.")
	sgs_send_comm("Spire", "Your target is " .. target_name .. comms_personas[persona])
	sgs_send_comm("Spire", "We're uploading their last known coordinates.\nReaver HQ out.")
	sgs_enqueue_callback(20.0, "mission_upload_coords")
	sgs_enqueue_callback(1.0, "mission_await_player")
end

function mission_upload_coords()
	sgs_waypoint_set(tarx, tary)
end

function mission_await_player()
	if not sgs_object_exists(player) or not sgs_object_exists(target_ship) then
		return
	end
	
	local dist = sgs_object_get_distance_to_object(player, target_ship)
	
	if dist < 1000.0 then
		sgs_send_comm(target_name, comms_greetings[greeting])
		sgs_object_set_attitude(target_ship, player, -0.7)
		sgs_enqueue_callback(1.0, "mission_targets_dead")
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

function mission_targets_dead()
	if not sgs_object_exists(player) then
		return
	end

	if sgs_object_exists(target_ship) then
		sgs_enqueue_callback(1.0, "mission_targets_dead")
		return
	end

	sgs_send_comm("Spire", "This is Reaver HQ.\nMission successful.")
	sgs_send_comm("Spire", "Transferring the reward to your account now.")
	sgs_send_comm("Spire", "Excellent work, pilot\nHQ out.")

	sgs_enqueue_callback(16.0, "mission_transfer_reward")
end

function mission_transfer_reward()
	sgs_player_adjust_money(5000)
	sgs_send_notification("Money: +5000")
	sgs_waypoint_set(0, 0)
end