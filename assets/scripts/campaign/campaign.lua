--Campaign meta script
--Determine which mission the player
--is on and execute the correct script.

if sgs_get_variable("mission1_started") ~= "y" or sgs_get_variable("mission1_active") == "y" then
	dofile(sgs_get_asset_path("scripts/campaign/mission1.lua"))
end