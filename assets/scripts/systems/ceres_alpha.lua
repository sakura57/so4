sgs_map_init(50000, 50000)
sgs_map_add_circle(0.0, 0.0, 0, 0, 255) --Ceres Beta buoy
sgs_map_add_zone_circular(12500.0, 12500.0, 5000.0, 128, 128, 128)
sgs_map_add_zone_circular(12500.0, -12500.0, 5000.0, 128, 128, 128)
sgs_map_add_zone_circular(-14000.0, 14000.0, 5000.0, 128, 128, 128)
sgs_map_finalize()

--Jump buoys
sgs_jumpbuoy_create(5, 0.0, 0.0, 9, -15000.0, -15250.0) --Buoy to Ceres Beta

--Astfields
sgs_astfield_create(10, 12500.0, 12500.0, 5000.0)
sgs_astfield_create(11, 12500.0, -12500.0, 5000.0)
sgs_astfield_create(12, -14000.0, 14000.0, 5000.0)