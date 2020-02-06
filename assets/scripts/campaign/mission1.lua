--Mission script

local player_ship = sgs_world_get_player()
local roberts_ship = 0
local training_drone = 0

if sgs_get_sector() == 2 then
	roberts_ship = sgs_ship_create(1, -1000.0, 14000.0, 180.0)

	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end

	sgs_set_variable("docking_enabled", "n")

	local playerx, playery = sgs_object_get_position(player_ship)
	local escortx, escorty = sgs_object_get_position(roberts_ship)
	sgs_hud_hide()
	sgs_object_halt(player_ship)
	sgs_cam_begin_vignette_camera(2.0, playerx, playery, escortx, escorty)
	sgs_enqueue_callback(2.0, "mission1_roberts_welcome_dialogue")
end

function mission1_roberts_welcome_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end

	sgs_dialogue_begin(4)
	sgs_enqueue_callback(1.0, "mission1_roberts_dialogue_ended")
end

function mission1_roberts_dialogue_ended()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end

	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
end