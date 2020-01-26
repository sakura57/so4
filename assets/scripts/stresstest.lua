--This script is always executed by the game.
--Register any important callbacks.

local pirate_chars = {10, 11, 12, 13, 14, 15, 16}
local tsf_chars = {17, 18, 19}

local pirates = {}
local tsf = {}

local num_pirates = 10
local num_tsf = 10

sgs_enqueue_callback(5.0, "spawn")
local freighter = sgs_ship_create(20, 200, 200, 180)

function spawn()
	for i=1,num_pirates,1 do
		local x, y = sgs_random_float(-1000.0, 1000.0), sgs_random_float(-1000.0, 1000.0)
		local r = sgs_random_float(-180.0, 180.0)
		pirates[i] = sgs_ship_create(pirate_chars[sgs_random_int(1, #pirate_chars)], x, y, r)
	end

	for i=1,num_tsf,1 do
		local x, y = sgs_random_float(-1000.0, 1000.0), sgs_random_float(-1000.0, 1000.0)
		local r = sgs_random_float(-180.0, 180.0)
		tsf[i] = sgs_ship_create(tsf_chars[sgs_random_int(1, #tsf_chars)], x, y, r)
	end

	for i=1,num_pirates,1 do
		for j=1,num_tsf,1 do
			sgs_object_set_attitude(pirates[i], tsf[j], -0.7)
			sgs_object_set_attitude(tsf[j], pirates[i], -0.7)
		end
	end
end