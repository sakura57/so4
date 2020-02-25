--Mission script

local player_ship = sgs_world_get_player()
local roberts_ship = 0
local jericho_ship = 0
local training_drone = 0
local dont_stray_from_object = 0
local dont_stray_from_point = 0
local dont_stray_distance = 5000.0

local strayx, strayy = 0.0, 0.0

--This section is executed if the player is in Callisto Beta.
if sgs_get_sector() == 2 then
	if not sgs_object_exists(player_ship) then
		return
	end

	if sgs_get_variable("mission2_started") ~= "y" then
		--Do a test to determine if the player is eligible to start the mission.
		--For mission 2: At least 20,000 credits.
		if sgs_player_get_money() < 20000 then
			return
		end
		
		roberts_ship = sgs_ship_create(1, -500.0, 14000.0, 180.0)
		
		sgs_set_variable("docking_enabled", "y")

		--Initialize globals associated with the mission.
		sgs_set_variable("mission2_started", "y") --The mission has been started and/or finished.
		sgs_set_variable("mission2_inprogress", "y") --The mission is in progress, but may not be active.
		sgs_set_variable("mission2_active", "n") --The mission is active.

		--Variables associated with various checkpoints
		sgs_set_variable("mission2_jericho_killed", "n") --The player has killed Jericho (task 1)

		local escortx, escorty = sgs_object_get_position(roberts_ship)
		
		sgs_send_notification("Objective: Meet with Roberts.")
		sgs_waypoint_set(escortx, escorty)
		
		sgs_enqueue_callback(2.0, "mission2_cb_meet_roberts")
	elseif sgs_get_variable("mission2_jericho_killed") == "y" then
		roberts_ship = sgs_ship_create(1, -500.0, 14000.0, 180.0)
	
		local escortx, escorty = sgs_object_get_position(roberts_ship)
		
		sgs_send_notification("Objective: Return to Roberts.")
		sgs_waypoint_set(escortx, escorty)
		
		sgs_enqueue_callback(2.0, "mission2_cb_meet_roberts_victory")
	end
	
	sgs_enqueue_callback(2.5, "mission_dont_stray_from")
--This section is executed if the player is in Callisto Alpha.
elseif sgs_get_sector() == 1 then
	if not sgs_object_exists(player_ship) then
		return
	end

	if sgs_get_variable("mission2_started") == "y" and (sgs_get_variable("mission2_jericho_killed") ~= "y" and sgs_get_variable("mission2_jericho_spared") ~= "y") then
		sgs_set_variable("docking_enabled", "n")
		
		jericho_ship = sgs_ship_create(25, -500.0, 14000.0, 180.0)

		local jerichox, jerichoy = sgs_object_get_position(jericho_ship)
		
		sgs_send_notification("Objective: Find Jericho.")
		sgs_waypoint_set(jerichox, jerichoy)
		
		sgs_enqueue_callback(2.0, "mission2_ca_meet_jericho")
		sgs_enqueue_callback(2.0, "mission2_ca_jericho_dead")
	end
	
	sgs_enqueue_callback(2.5, "mission_dont_stray_from")
end

function mission2_cb_meet_roberts()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end
	
	if sgs_object_get_distance_to_object(player_ship, roberts_ship) > 500.0 then
		sgs_enqueue_callback(2.0, "mission2_cb_meet_roberts")
		return
	end
	
	sgs_waypoint_remove()
	
	local playerx, playery = sgs_object_get_position(player_ship)
	local escortx, escorty = sgs_object_get_position(roberts_ship)
	
	sgs_object_halt(player_ship)
	sgs_hud_hide()
	sgs_cam_begin_vignette_camera(3.0, playerx, playery, escortx, escorty)
	
	sgs_send_notification("Objective complete.")
	
	sgs_enqueue_callback(3.0, "mission2_cb_robert_start_dialogue")
end

function mission2_cb_robert_start_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end
	
	sgs_dialogue_begin(28)
	
	sgs_enqueue_callback(1.0, "mission2_cb_robert_end_dialogue")
end

function mission2_cb_robert_end_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end
	
	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	sgs_send_notification("Objective: Travel to Callisto Alpha.")
	
	sgs_waypoint_set(-5000.0, 200.0)
	
	--nothing else to do, CA side of the script takes care of the rest
end

function mission2_ca_meet_jericho()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(jericho_ship) then
		return
	end
	
	if sgs_object_get_distance_to_object(player_ship, jericho_ship) > 1500.0 then
		sgs_enqueue_callback(2.0, "mission2_ca_meet_jericho")
		return
	end
	
	sgs_waypoint_remove()
	
	local playerx, playery = sgs_object_get_position(player_ship)
	local jerichox, jerichoy = sgs_object_get_position(jericho_ship)
	
	sgs_object_halt(player_ship)
	sgs_hud_hide()
	sgs_cam_begin_vignette_camera(3.0, playerx, playery, jerichox, jerichoy)
	
	sgs_send_notification("Objective complete.")
	
	sgs_enqueue_callback(3.0, "mission2_ca_jericho_start_dialogue")
end

function mission2_ca_jericho_dead()
	if not sgs_object_exists(player_ship) then
		return
	end

	if sgs_object_exists(jericho_ship) then
		sgs_enqueue_callback(3.0, "mission2_ca_jericho_dead")
		return
	end
	
	sgs_set_variable("mission2_jericho_killed", "y")
	sgs_set_variable("mission2_jericho_spared", "n")

	sgs_send_notification("Objective complete.")
	sgs_send_notification("Objective: Return to Roberts.")
	
	sgs_set_variable("docking_enabled", "y")
	
	sgs_waypoint_set(-10000.0, 4500.0)
end

function mission2_ca_jericho_start_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(jericho_ship) then
		return
	end
	
	sgs_dialogue_begin(37)
	
	sgs_enqueue_callback(1.0, "mission2_ca_jericho_end_dialogue")
end

function mission2_ca_jericho_end_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(jericho_ship) then
		return
	end
	
	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	if sgs_get_variable("mission2_jericho_provoked") == "y" then
		sgs_object_set_attitude(jericho_ship, player_ship, -0.7)
	end
end

function mission2_cb_meet_roberts_victory()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end
	
	if sgs_object_get_distance_to_object(player_ship, roberts_ship) > 500.0 then
		sgs_enqueue_callback(2.0, "mission2_cb_meet_roberts_victory")
		return
	end
	
	sgs_waypoint_remove()
	
	local playerx, playery = sgs_object_get_position(player_ship)
	local escortx, escorty = sgs_object_get_position(roberts_ship)
	
	sgs_object_halt(player_ship)
	sgs_hud_hide()
	sgs_cam_begin_vignette_camera(3.0, playerx, playery, escortx, escorty)
	
	sgs_send_notification("Objective complete.")
	
	sgs_enqueue_callback(3.0, "mission2_cb_robert_start_dialogue_victory")
end

function mission2_cb_robert_start_dialogue_victory()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end
	
	sgs_dialogue_begin(46)
	
	sgs_enqueue_callback(1.0, "mission2_cb_robert_end_dialogue_victory")
end

function mission2_cb_robert_end_dialogue_victory()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end
	
	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	mission2_mission_complete()
end

function mission2_mission_complete()
	sgs_set_variable("mission2_inprogress", "n")
	sgs_set_variable("mission2_active", "n")
	sgs_set_variable("mission2_complete", "y")
	sgs_set_variable("docking_enabled", "y")
	
	sgs_player_adjust_money(15000)
	sgs_send_notification("Money: +15000")
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