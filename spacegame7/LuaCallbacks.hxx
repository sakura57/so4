#pragma once

#include "SGLib.hxx"

extern "C"
{
	namespace sgs
	{
		static int enqueue_callback(lua_State *);
		static int ship_create(lua_State *);
		static int staticobj_create(lua_State *);
		static int jumpbuoy_create(lua_State *);
		static int dockbuoy_create(lua_State *);
		static int astfield_create(lua_State *);
		static int object_get_position(lua_State *);
		static int object_get_velocity(lua_State *);
		static int object_get_acceleration(lua_State *);
		static int object_get_rotation(lua_State *);
		static int object_get_angular_velocity(lua_State *);
		static int object_get_angular_acceleration(lua_State *);
		static int object_goto_directive(lua_State *);
		static int object_chill_directive(lua_State *);
		static int object_set_target(lua_State *);
		static int proximity_callback(lua_State *);
		static int effect_jump_in(lua_State *);

		static int variable_set(lua_State *);
		static int variable_get(lua_State *);

		static int object_get_distance_to_object(lua_State *);
		static int object_get_distance_to_point(lua_State *);
		static int object_get_name(lua_State *);
		static int random_float(lua_State *);
		static int random_int(lua_State *);

		static int send_comm(lua_State *);

		static int object_exists(lua_State *);
		static int object_set_attitude(lua_State *);
		static int object_get_attitude(lua_State*);

		static int instant_dock(lua_State *);
		static int instant_jump(lua_State*);

		static int world_get_player(lua_State *);
		static int player_get_level(lua_State*);

		static int waypoint_set(lua_State *);
		static int waypoint_remove(lua_State *);

		static int player_adjust_money(lua_State*);
		static int player_get_money(lua_State*);

		static int map_init(lua_State*);
		static int map_finalize(lua_State*);
		static int map_add_square(lua_State*);
		static int map_add_circle(lua_State*);
		static int map_add_zone_circular(lua_State*);
		static int map_add_zone_rectangular(lua_State*);
		
		static int send_notification(lua_State*);

		static int cam_begin_chase_camera(lua_State*);
		static int cam_begin_vignette_camera(lua_State*);
		static int hud_hide(lua_State*);
		static int hud_show(lua_State*);
		static int object_halt(lua_State*);
		static int object_enable_weapons(lua_State*);

		static int object_set_invulnerable(lua_State*);

		static int dialogue_begin(lua_State *);
		static int get_asset_path(lua_State*);

		static int get_sector(lua_State*);
		static int fail_mission(lua_State*);

		static int audio_set_ambient_music(lua_State*);
		static int audio_set_battle_music(lua_State*);
		static int audio_play_music(lua_State*);
		static int audio_stop_music(lua_State*);
	}
}

namespace sgs
{
	void register_callbacks(void);
	IWorldObject *worldobject_from_id(lua_State *, InstanceId const);
}