local europan_ship1 = sgs_ship_create(5, 4000.0, -250.0, 125.0)

sgs_enqueue_callback(15.0, "europans_comm")

function europans_comm()
	sgs_send_comm("Europan Ship", "What's a Terran doing out here?\nAh, doesn't matter.\nPrepare to die.")
	sgs_enqueue_callback(7.5, "europans_attack")
end

function europans_attack()
	sgs_object_set_attitude(europan_ship1, sgs_world_get_player(), -0.7)
end