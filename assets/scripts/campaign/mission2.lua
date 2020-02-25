--Mission script

local player_ship = sgs_world_get_player()
local roberts_ship = 0
local jericho_ship = 0
local training_drone = 0
local dont_stray_from_object = 0
local dont_stray_from_point = 0
local dont_stray_distance = 5000.0

local strayx, strayy = 0.0, 0.0

if sgs_get_variable("mission2_started") ~= "y" then
	--test money
end

if sgs_get_sector() == 2 then
	roberts_ship = sgs_ship_create(1, -500.0, 14000.0, 180.0)

	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end

	if sgs_get_variable("mission2_started") ~= "y" then
		sgs_set_variable("docking_enabled", "y")

		--Initialize globals associated with the mission.
		sgs_set_variable("mission2_started", "y") --The mission has been started and/or finished.
		sgs_set_variable("mission2_inprogress", "y") --The mission is in progress, but may not be active.
		sgs_set_variable("mission2_active", "n") --The mission is active.

		--Variables associated with various checkpoints
		sgs_set_variable("mission2_jericho_killed", "n") --The player has killed Jericho (task 1)

		local escortx, escorty = sgs_object_get_position(roberts_ship)
		
		sgs_enqueue_callback(2.0, "mission2_roberts_announce_comm")
		sgs_enqueue_callback(2.5, "mission_dont_stray_from")
	end

	if sgs_get_variable("mission1_refused") == "y" then
		sgs_enqueue_callback(2.5, "mission1_part2_dispatch")
	end
end

if sgs_get_sector() == 1 then
	jericho_ship = sgs_ship_create(25, -500.0, 14000.0, 180.0)

	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(jericho_ship) then
		return
	end

	if sgs_get_variable("mission2_started") == "y" and sgs_get_variable("mission2_jericho_killed") ~= "y" then
		sgs_set_variable("docking_enabled", "y")

		--Initialize globals associated with the mission.
		sgs_set_variable("mission2_started", "y") --The mission has been started and/or finished.
		sgs_set_variable("mission2_inprogress", "y") --The mission is in progress, but may not be active.
		sgs_set_variable("mission2_active", "n") --The mission is active.

		--Variables associated with various checkpoints
		sgs_set_variable("mission2_killed_jericho", "n") --The player has killed Jericho (task 1)

		local jerichox, jerichoy = sgs_object_get_position(jericho_ship)
		
		sgs_enqueue_callback(2.5, "mission_dont_stray_from")
	end

	if sgs_get_variable("mission1_refused") == "y" then
		sgs_enqueue_callback(2.5, "mission1_part2_dispatch")
	end
end

function mission2_roberts_announce_comm()
	if not sgs_object_exists(player_ship) then
		return
	end

	sgs_
end

function mission1_mission_complete()
	sgs_set_variable("mission2_inprogress", "n")
	sgs_set_variable("mission2_active", "n")
	sgs_set_variable("mission2_complete", "y")
	sgs_set_variable("docking_enabled", "y")
	
	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	sgs_player_adjust_money(10000)
	sgs_send_notification("Money: +10000")
end

function mission_dont_stray_from()
	if not sgs_object_exists(player_ship) then
		return
	end

	if dont_stray_from_object ~= 0 then
		if sgs_object_exists(dont_stray_from_object) then
			if sgs_object_get_distance_to_object(player_ship, dont_stray_from_object) > dont_stray_distance then
				mission_failed_strayed()
				return
			end
		end
	end

	if dont_stray_from_point ~= 0 then
		if sgs_object_get_distance_to_point(player_ship, strayx, strayy) > dont_stray_distance then
			mission_failed_strayed()
			return
		end
	end

	sgs_enqueue_callback(2.5, "mission_dont_stray_from")
end

function mission_failed_strayed()
	sgs_fail_mission("You strayed too far from the mission area.")
end