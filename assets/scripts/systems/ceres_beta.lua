--Music
sgs_audio_set_ambient_music(1)
sgs_audio_set_battle_music(2)

sgs_map_init(50000, 50000)
sgs_map_add_square(0.0, 0.0, 255, 255, 0) --Freeport
sgs_map_add_circle(-15000.0, -15000.0, 0, 0, 255) --Ceres Alpha buoy
sgs_map_add_circle(15000.0, 15000.0, 0, 0, 255) --Ceres Gamma buoy
sgs_map_add_zone_circular(12500.0, -12500.0, 7500.0, 128, 128, 128) --Southern astfield
sgs_map_add_zone_circular(-14000.0, 14000.0, 5000.0, 128, 128, 128) --Northern astfield
sgs_map_finalize()

--Astfields
sgs_astfield_create(5, 12500.0, -12500.0, 7500.0)
sgs_astfield_create(3, -14000.0, 14000.0, 5000.0)

--Jump buoys
sgs_jumpbuoy_create(5, -15000.0, -15000.0, 8, 0.0, 250.0) --Buoy to Ceres Alpha
sgs_jumpbuoy_create(5, 15000.0, 15000.0, 10, -17500.0, 17750.0) --Buoy to Ceres Gamma

--Staticobjs
sgs_staticobj_create(17, 0.0, 0.0, -200.0, 0.0, 1350.0, 1800.0)

--Docking buoys
local freeport_buoy = sgs_dockbuoy_create(5, 0.0, 0.0, 5) --Whitney Station