--Music
sgs_audio_set_ambient_music(3)
sgs_audio_set_battle_music(4)

--Sector map section
sgs_map_init(50000, 50000)
sgs_map_add_square(0.0, 0.0, 255, 255, 0) --CS Bremerhaven
sgs_map_add_square(5000.0, 0.0, 255, 255, 0) --Whitney Station
sgs_map_add_circle(-5000.0, 200.0, 0, 0, 255) --Callisto Alpha buoy
sgs_map_add_circle(-8000.0, -15000.0, 0, 0, 255) --Ceres Gamma buoy
sgs_map_add_circle(15000.0, 15000.0, 0, 0, 255) --Callisto Gamma buoy
sgs_map_add_zone_circular(1000.0, -15000.0, 5000.0, 128, 128, 128) --Southern astfield
sgs_map_add_zone_circular(-1000.0, 15000.0, 5000.0, 128, 128, 128) --Northern astfield
sgs_map_finalize()

--Staticobjs section
sgs_staticobj_create(1, 0.0, 0.0, -200.0, 0.0, 4096.0, 2048.0)
sgs_staticobj_create(17, 5000.0, 0.0, -200.0, 0.0, 1350.0, 1800.0)

--Astfields
sgs_astfield_create(1, 1000.0, -15000.0, 5000.0)
sgs_astfield_create(4, -1000.0, 15000.0, 5000.0)

--Jump buoys
sgs_jumpbuoy_create(1, -5000.0, 200.0, 1, -10000.0, 4300.0) --Buoy to Callisto Alpha
sgs_jumpbuoy_create(1, 15000.0, 15000.0, 3, -19000.0, 750.0) --Buoy to Callisto Gamma
sgs_jumpbuoy_create(1, -8000.0, -15000.0, 10, 17500.0, 750.0) --Buoy to Ceres Gamma

--Docking buoys
local bremerhaven_buoy = sgs_dockbuoy_create(1, 0.0, 0.0, 1) --CS Bremerhaven
local whitney_buoy = sgs_dockbuoy_create(2, 5000.0, 0.0, 4) --Whitney Station

--Dynamic spawns section
local tsf_patroller_chars = {17, 18, 19}
local patroller1_char = tsf_patroller_chars[sgs_random_int(1, #tsf_patroller_chars)]

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

dofile(sgs_get_asset_path("scripts/campaign/campaign.lua"))