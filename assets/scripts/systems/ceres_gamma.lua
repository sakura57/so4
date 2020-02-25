--Sector map section
sgs_map_init(50000, 50000)
sgs_map_add_circle(-17500.0, -17500.0, 0, 0, 255) --Mars Gamma buoy
sgs_map_add_circle(-17500.0, 17500.0, 0, 0, 255) --Ceres Beta buoy
sgs_map_add_circle(17500.0, 500.0, 0, 0, 255) --Callisto Beta buoy
sgs_map_add_zone_circular(10000.0, 5000.0, 5000.0, 128, 128, 128)
sgs_map_add_zone_circular(-500.0, -5000.0, 5000.0, 128, 128, 128)
sgs_map_add_zone_circular(-17500.0, -1750.0, 5000.0, 128, 128, 128)
sgs_map_finalize()

--Astfields
sgs_astfield_create(7, 10000.0, 5000.0, 5000.0)
sgs_astfield_create(8, -500.0, -5000.0, 5000.0)
sgs_astfield_create(9, -17500.0, -1750.0, 5000.0)

--Jump buoys
sgs_jumpbuoy_create(5, -17500.0, 17500.0, 9, 15000.0, 15250.0) --Buoy to Ceres Beta
sgs_jumpbuoy_create(1, 17500.0, 500.0, 2, -8000.0, -15250.0) --Buoy to Callisto Beta

--Dynamic spawns section
local legion_patroller_chars = {28, 29, 30}
local patroller1_char = legion_patroller_chars[sgs_random_int(1, #legion_patroller_chars)]

local patroller1_x, patroller1_y = sgs_random_float(-1000.0, 1000.0), sgs_random_float(-1000.0, 1000.0)

local patroller1 = sgs_ship_create(patroller1_char, patroller1_x, patroller1_y, 200.0)

function patroller1_wander()
	if not sgs_object_exists(patroller1) then
		return
	end
	
	local dest_x, dest_y = sgs_random_float(-1000.0, 1000.0), sgs_random_float(-1000.0, 1000.0)
	
	sgs_object_goto_directive(patroller1, dest_x, dest_y, 250.0)
	
	sgs_enqueue_callback(sgs_random_float(10.0, 30.0), "patroller1_wander")
end

patroller1_wander()