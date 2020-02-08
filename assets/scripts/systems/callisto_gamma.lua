--Sector map section
sgs_map_init(50000, 50000)
sgs_map_add_circle(-19000.0, 500.0, 0, 0, 255) --Callisto Beta buoy
sgs_map_add_circle(19000.0, -500.0, 0, 0, 255) --Ganymede Gamma buoy
sgs_map_finalize()

--Jump buoys
sgs_jumpbuoy_create(1, -19000.0, 500.0, 2, 15000.0, 15250.0) --Buoy to Callisto Beta
sgs_jumpbuoy_create(4, 19000.0, -500.0, 6, -15000.0, -15250.0) --Buoy to Ganymede Gamma

--Dynamic spawns section
local tsf_patroller_chars = {17, 18, 19}
local europan_patroller_chars = {31, 32}

local patroller1_char = tsf_patroller_chars[sgs_random_int(1, #tsf_patroller_chars)]

local patroller1_x, patroller1_y = sgs_random_float(-3000.0, -1000.0), sgs_random_float(-5000.0, 5000.0)

local patroller1 = sgs_ship_create(patroller1_char, patroller1_x, patroller1_y, 200.0)

local patroller2_char = europan_patroller_chars[sgs_random_int(1, #europan_patroller_chars)]

local patroller2_x, patroller2_y = sgs_random_float(1000.0, 3000.0), sgs_random_float(-5000.0, 5000.0)

local patroller2 = sgs_ship_create(patroller2_char, patroller2_x, patroller2_y, 200.0)

function patroller1_wander()
	if not sgs_object_exists(patroller1) then
		return
	end
	
	local dest_x, dest_y = sgs_random_float(-3000.0, -1000.0), sgs_random_float(-5000.0, 5000.0)
	
	sgs_object_goto_directive(patroller1, dest_x, dest_y, 250.0)
	
	sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller1_wander")
end

function patroller2_wander()
	if not sgs_object_exists(patroller2) then
		return
	end
	
	local dest_x, dest_y = sgs_random_float(1000.0, 3000.0), sgs_random_float(-5000.0, 5000.0)
	
	sgs_object_goto_directive(patroller2, dest_x, dest_y, 250.0)
	
	sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller2_wander")
end

patroller1_wander()
patroller2_wander()