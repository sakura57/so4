--Music
sgs_audio_set_ambient_music(1)
sgs_audio_set_battle_music(2)

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

local player_ship = sgs_world_get_player()

local borderpatrol1 = 0
local borderpatrol2 = 0
local borderpatrol3 = 0

sgs_enqueue_callback(2.0, "test_spawn_border_patrol")

function test_spawn_border_patrol()
	if not sgs_object_exists(player_ship) then
		return
	end
	
	if sgs_get_variable("cg_encounter_border_patrol") == "y" then
		return
	end
	
	local playerx, playery = sgs_object_get_position(player_ship)
	
	borderpatrol1 = sgs_ship_create(33, playerx + 800.0, playery, 180.0)
	borderpatrol2 = sgs_ship_create(34, playerx + 800.0, playery + 350.0, 220.0)
	borderpatrol3 = sgs_ship_create(35, playerx + 800.0, playery - 350.0, 140.0)
	
	sgs_effect_jump_in(playerx + 800.0, playery)
	sgs_effect_jump_in(playerx + 800.0, playery + 350.0)
	sgs_effect_jump_in(playerx + 800.0, playery - 350.0)
	
	sgs_hud_hide()
	sgs_object_halt(player_ship)
	sgs_cam_begin_vignette_camera(2.0, playerx, playery, playerx + 800.0, playery)
	sgs_enqueue_callback(2.0, "encounter_border_patrol_start_dialogue")
end

function encounter_border_patrol_start_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end
	
	if not sgs_object_exists(borderpatrol1) then
		return
	end
	
	sgs_dialogue_begin(49)
	sgs_enqueue_callback(1.0, "encounter_border_patrol_dialogue_ended")
end

function encounter_border_patrol_dialogue_ended()
	if not sgs_object_exists(player_ship) then
		return
	end
	
	if not sgs_object_exists(borderpatrol1) then
		return
	end
	
	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	sgs_set_variable("cg_encounter_border_patrol", "y")
	
	if sgs_get_variable("cg_border_patrol_provoked") == "y" then
		sgs_object_set_attitude(borderpatrol1, player_ship, -0.7)
		sgs_object_set_attitude(borderpatrol2, player_ship, -0.7)
		sgs_object_set_attitude(borderpatrol3, player_ship, -0.7)
	else
		sgs_object_goto_directive(borderpatrol1, -2500.0, 500.0, 500.0)
		sgs_object_goto_directive(borderpatrol2, -2500.0, 850.0, 500.0)
		sgs_object_goto_directive(borderpatrol3, -2500.0, 150.0, 500.0)
	end
end

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

function player_check_crossed_border()
	local player_ship = sgs_world_get_player()
		
	if not sgs_object_exists(player_ship) then
		return
	end
	
	if sgs_get_variable("cg_border_clearance") ~= "y" then
		local playerx, playery = sgs_object_get_position(player_ship)
		
		if playerx > 0.0 then
			sgs_fail_mission("You crossed the border into Europan space.")
		end
	end
	
	sgs_enqueue_callback(5.0, "player_check_crossed_border")
end

patroller1_wander()
patroller2_wander()
player_check_crossed_border()