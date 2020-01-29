--This script is always executed by the game.
--Register any important callbacks.

function cb1()
	local x, y = sgs_object_get_position(1)
	io.write(string.format("pos: %.4f, %.4f\n", x, y))
	sgs_enqueue_callback(2, "cb1")
	sgs_enqueue_callback(1, "cb2")
end

function cb2()
	local x, y = sgs_object_get_velocity(1)
	io.write(string.format("vel: %.4f %.4f\n", x, y))
end

io.write(string.format("Main script started\nRunning %s\n", _VERSION))
sgs_enqueue_callback(0.2, "cb1")