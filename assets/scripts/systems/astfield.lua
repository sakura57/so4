local focus_x, focus_y, focus_z = 0.0, 0.0, -400
local size_x, size_y = 1000.0, 1000.0
local pop = 5

local asteroids = {2, 3, 4, 5, 6}

for i=1,pop do
	local x, y, z = sgs_random_float(-size_x/2.0, size_x/2.0), sgs_random_float(-size_y/2.0, size_y/2.0), sgs_random_float(-50.0, 0.0)
	local rot = sgs_random_float(-180.0, 180.0)
	local ast = asteroids[sgs_random_int(2, #asteroids)]
	
	sgs_staticobj_create(ast, x + focus_x, y + focus_y, z + focus_z, rot, 512.0, 512.0)
end