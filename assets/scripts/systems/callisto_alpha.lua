--Staticobjs section
sgs_staticobj_create(17, 0.0, 0.0, -200.0, 0.0, 1350.0, 1800.0)

--Jump buoys
local beta_buoy = sgs_jumpbuoy_create(1, -10000.0, 4500.0, 2, -5000.0, 0.0) --Buoy to Callisto Beta

--Docking buoys
local spire_buoy = sgs_dockbuoy_create(3, 0.0, 0.0, 2) --Spire

--Dynamic spawns section
local pi_patroller_chars = {10, 11, 12, 13, 14, 15, 22, 23, 24}
local patroller1 = 0

--Jericho (test)
local patroller2 = sgs_ship_create(25, 1000.0, 200.0, 200.0)

function patroller1_spawn()
	local patroller1_char = pi_patroller_chars[sgs_random_int(1, #pi_patroller_chars)]

	local patroller1_x, patroller1_y = sgs_random_float(-11000.0, -9000.0), sgs_random_float(3500.0, 5500.0)

	patroller1 = sgs_ship_create(patroller1_char, patroller1_x, patroller1_y, 200.0)
	sgs_effect_jump_in(patroller1_x, patroller1_y)
end

function patroller1_wander1()
	if not sgs_object_exists(patroller1) then
		patroller1_spawn()
	end
	
	local dist = sgs_object_get_distance_to_object(patroller1, beta_buoy)
	
	local dest_x, dest_y = sgs_random_float(-11000.0, -9000.0), sgs_random_float(3500.0, 5500.0)
	
	sgs_object_goto_directive(patroller1, dest_x, dest_y, 250.0)
	
	if not (dist < 2000.0) then
		sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller1_wander1")
		return
	else
		sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller1_wander2")
	end
end

function patroller1_wander2()
	if not sgs_object_exists(patroller1) then
		patroller1_spawn()
	end
	
	local dist = sgs_object_get_distance_to_object(patroller1, spire_buoy)
	
	local dest_x, dest_y = sgs_random_float(-1000.0, 1000.0), sgs_random_float(-1000.0, 1000.0)
	
	sgs_object_goto_directive(patroller1, dest_x, dest_y, 250.0)
	
	if not (dist < 2000.0) then
		sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller1_wander2")
		return
	else
		sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller1_wander1")
	end
end

patroller1_spawn()
patroller1_wander1()