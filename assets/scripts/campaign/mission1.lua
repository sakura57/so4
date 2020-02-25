--Mission script

local pirate_characters = {13, 14, 15}
local player_ship = sgs_world_get_player()
local roberts_ship = 0
local training_drone = 0
local dont_stray_from_object = 0
local dont_stray_from_point = 0
local dont_stray_distance = 5000.0

local pirate1_ship = 0
local pirate2_ship = 0

local strayx, strayy = 0.0, 0.0

if sgs_get_sector() == 2 then
	roberts_ship = sgs_ship_create(1, -500.0, 14000.0, 180.0)

	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end

	if sgs_get_variable("mission1_started") ~= "y" then
		dont_stray_from_object = roberts_ship

		sgs_set_variable("docking_enabled", "n")

		--Initialize globals associated with the mission.
		sgs_set_variable("mission1_started", "y") --The mission has been started and/or finished.
		sgs_set_variable("mission1_inprogress", "y") --The mission is in progress, but may not be active.
		sgs_set_variable("mission1_active", "y") --The mission is active.

		--Variables associated with various checkpoints
		sgs_set_variable("mission1_destroyed_drone", "n") --The player has destroyed the training drone (task 1)
		sgs_set_variable("mission1_pirates_killed", "n") --The player has destroyed the pirates in the Roanoke Belt

		local playerx, playery = sgs_object_get_position(player_ship)
		local escortx, escorty = sgs_object_get_position(roberts_ship)
		sgs_hud_hide()
		sgs_object_halt(player_ship)
		sgs_cam_begin_vignette_camera(2.0, playerx, playery, escortx, escorty)
		sgs_enqueue_callback(2.0, "mission1_roberts_welcome_dialogue")
		sgs_enqueue_callback(2.5, "mission_dont_stray_from")
	end

	if sgs_get_variable("mission1_refused") == "y" then
		sgs_enqueue_callback(2.5, "mission1_part2_dispatch")
	end
end

function mission1_roberts_welcome_dialogue()
	if not sgs_object_exists(player_ship) then
		return
	end

	sgs_dialogue_begin(4)
	sgs_enqueue_callback(1.0, "mission1_roberts_dialogue_ended")
end

function mission1_roberts_dialogue_ended()
	if not sgs_object_exists(player_ship) then
		return
	end

	local playerx, playery = sgs_object_get_position(roberts_ship)

	sgs_cam_begin_vignette_camera(2.0, playerx, playery, -2500.0, 14000.0)
	sgs_enqueue_callback(2.0, "mission1_spawn_drone")
end

function mission1_spawn_drone()
	if not sgs_object_exists(player_ship) then
		return
	end

	training_drone = sgs_ship_create(27, -2500.0, 14000.0, 0.0)
	sgs_effect_jump_in(-2500.0, 14000.0)

	sgs_enqueue_callback(3.0, "mission1_chase_cam_drone")
end

function mission1_chase_cam_drone()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(training_drone) then
		return
	end

	sgs_object_set_attitude(training_drone, player_ship, -0.7)
	sgs_cam_begin_chase_camera(training_drone)
	
	sgs_enqueue_callback(2.0, "mission1_drone_return_player_control")
end

function mission1_drone_return_player_control()
	if not sgs_object_exists(player_ship) then
		return
	end

	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	sgs_send_notification("Objective: Destroy the training drone.")

	sgs_enqueue_callback(2.0, "mission1_check_drone_dead")
end

function mission1_check_drone_dead()
	if not sgs_object_exists(player_ship) then
		return
	end

	if sgs_object_exists(training_drone) then
		sgs_enqueue_callback(2.0, "mission1_check_drone_dead")

		return
	end

	sgs_object_halt(player_ship)
	sgs_hud_hide()
	
	sgs_send_notification("Objective complete.")

	sgs_set_variable("mission1_destroyed_drone", "y")

	sgs_enqueue_callback(2.0, "mission1_roberts_continues_talking")
end

function mission1_roberts_continues_talking()
	sgs_dialogue_begin(14)

	sgs_enqueue_callback(1.0, "mission1_roberts_second_conversation_over")
end

function mission1_roberts_second_conversation_over()
	sgs_hud_show()

	dont_stray_from_object = 0

	if sgs_get_variable("mission1_refused") == "y" then
		sgs_set_variable("docking_enabled", "y")
		sgs_set_variable("mission1_active", "n")

		sgs_enqueue_callback(2.5, "mission1_part2_dispatch")
	elseif sgs_get_variable("mission1_phase1") == "y" then
		sgs_set_variable("docking_enabled", "n")
		sgs_set_variable("mission1_active", "y")
		
		sgs_send_notification("Objective: Fly to the Roanoke Metallic Belt.")
		sgs_waypoint_set(1000.0, -13000.0)

		sgs_enqueue_callback(2.5, "mission1_close_in_on_bogeys")
	end
end

function mission1_part2_dispatch()
	if not sgs_object_exists(player_ship) then
		return
	end

	if not sgs_object_exists(roberts_ship) then
		return
	end

	if sgs_object_get_distance_to_object(player_ship, roberts_ship) < 500 then
		sgs_hud_hide()
		sgs_dialogue_begin(22)

		sgs_enqueue_callback(1.0, "mission1_roberts_second_conversation_over")

		return
	end

	sgs_enqueue_callback(2.5, "mission1_part2_dispatch")
end

function mission1_close_in_on_bogeys()
	if not sgs_object_exists(player_ship) then
		return
	end

	if sgs_object_get_distance_to_point(player_ship, 1000.0, -13000.0) < 2000 then
		local playerx, playery = sgs_object_get_position(player_ship)
		
		sgs_waypoint_remove()
		
		sgs_object_halt(player_ship)
		sgs_hud_hide()
		sgs_cam_begin_vignette_camera(3.0, playerx, playery, 1000.0, -13000.0)
		
		sgs_send_notification("Objective complete.")
		
		sgs_enqueue_callback(3.0, "mission1_spawn_bogeys")

		return
	end

	sgs_enqueue_callback(1.0, "mission1_close_in_on_bogeys")
end

function mission1_spawn_bogeys()
	if not sgs_object_exists(player_ship) then
		return
	end

	local character1 = pirate_characters[sgs_random_int(1, #pirate_characters)]
	local character2 = pirate_characters[sgs_random_int(1, #pirate_characters)]
	
	pirate1_ship = sgs_ship_create(character1, 350.0, -12900.0, 90.0)
	pirate2_ship = sgs_ship_create(character2, 1000.0, -13000.0, 90.0)
	
	sgs_object_enable_weapons(pirate1_ship, 0)
	sgs_object_enable_weapons(pirate2_ship, 0)
	
	sgs_effect_jump_in(350.0, -12900.0)
	sgs_effect_jump_in(1000.0, -13000.0)
	
	sgs_cam_begin_chase_camera(pirate2_ship)
	
	sgs_enqueue_callback(3.0, "mission1_bogeys_comm")
end

function mission1_bogeys_comm()
	sgs_send_comm("Scrapper", "Soon, TSF scum... soon there will be a reckoning.")
	sgs_enqueue_callback(5.0, "mission1_bogeys_engage")
	sgs_enqueue_callback(8.0, "mission1_bogeys_return_player_control")
end

function mission1_bogeys_engage()
	if not sgs_object_exists(player_ship) then
		return
	end

	sgs_object_set_attitude(pirate1_ship, player_ship, -0.7)
	sgs_object_set_attitude(pirate2_ship, player_ship, -0.7)
end

function mission1_bogeys_return_player_control()
	if not sgs_object_exists(player_ship) then
		return
	end

	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	sgs_object_enable_weapons(pirate1_ship, 1)
	sgs_object_enable_weapons(pirate2_ship, 1)
	
	sgs_send_notification("Objective: Eliminate the Reavers.")

	sgs_enqueue_callback(2.0, "mission1_check_bogeys_dead")
end

function mission1_check_bogeys_dead()
	if not sgs_object_exists(player_ship) then
		return
	end
	
	if sgs_object_exists(pirate1_ship) or sgs_object_exists(pirate2_ship) then
		sgs_enqueue_callback(2.0, "mission1_check_bogeys_dead")
		
		return
	end
	
	sgs_set_variable("mission1_pirates_killed", "y")
	
	sgs_send_notification("Objective complete.")
	sgs_send_notification("Objective: Return to Roberts.")
	
	local robertsx, robertsy = sgs_object_get_position(roberts_ship)
	
	sgs_waypoint_set(robertsx, robertsy - 100.0)
	
	sgs_enqueue_callback(1.0, "mission1_final_comm_from_roberts")
end

function mission1_final_comm_from_roberts()
	if not sgs_object_exists(player_ship) then
		return
	end
	
	if sgs_object_get_distance_to_object(player_ship, roberts_ship) < 500 then
		sgs_hud_hide()
		sgs_dialogue_begin(23)
		
		sgs_send_notification("Objective complete.")
		
		sgs_waypoint_remove()

		sgs_enqueue_callback(1.0, "mission1_mission_complete")

		return
	end
	
	sgs_enqueue_callback(1.0, "mission1_final_comm_from_roberts")
end

function mission1_mission_complete()
	sgs_set_variable("mission1_inprogress", "n")
	sgs_set_variable("mission1_active", "n")
	sgs_set_variable("mission1_complete", "y")
	sgs_set_variable("docking_enabled", "y")
	
	sgs_cam_begin_chase_camera(player_ship)
	sgs_hud_show()
	
	sgs_player_adjust_money(5000)
	sgs_send_notification("Money: +5000")
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