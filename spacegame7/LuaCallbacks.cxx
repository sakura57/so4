#include <lua.hpp>
#include "LuaCallbacks.hxx"
#include <iostream>
#include "CEquippedObject.hxx"
#include "CAttitudeSet.hxx"
#include "CShip.hxx"
#include "CAIController.hxx"
#include "IGameState.hxx"
#include "CInSpaceState.hxx"
#include "CStaticObject.hxx"
#include "CSectorJumpBuoy.hxx"
#include "CLandingBuoy.hxx"
#include "CSectorTransitionState.hxx"
#include "CBaseTransitionState.hxx"
#include "CAsteroidField.hxx"
#include "CChaseCamera.hxx"
#include "CVignetteCamera.hxx"

extern "C"
{
	/*
	 * Callback for enqueue_callback.
	 * 
	 * Instructs the script engine to invoke a callback after
	 * a certain amount of time.
	 */
	static int sgs::enqueue_callback(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isstring(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		lua_Number const flTime = lua_tonumber(L, 1);
		char const * szFunction = lua_tostring(L, 2);

		IScriptEngine *pScriptEngine = SG::get_script_engine();
		try
		{
			pScriptEngine->script_enqueue_function((float)flTime, szFunction);
		}
		catch(SGException e)
		{
			lua_pushstring(L, ("script_enqueue_function threw: " + e.get_exception()).c_str());
			lua_error(L);
		}

		return 0;
	}

	/*
	* Callback for proximity_callback.
	*
	* Instructs the script engine to invoke a callback when
	* an object reaches a certain area.
	*/
	static int sgs::proximity_callback(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 5)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) ||!lua_isstring(L, 5))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = lua_tointeger(L, 1);
		Vector2f const vPosition = Vector2f(lua_tonumber(L, 2), lua_tonumber(L, 3));
		float const flRadius = lua_tonumber(L, 4);
		char const * szFunction = lua_tostring(L, 5);

		IScriptEngine *pScriptEngine = SG::get_script_engine();
		try
		{
			//TBD
			//pScriptEngine->script_register_proximity_callback();
		}
		catch (SGException e)
		{
			lua_pushstring(L, ("script_register_proximity_callback threw: " + e.get_exception()).c_str());
			lua_error(L);
		}

		return 0;
	}

	/*
	 * Callback for object_get_position.
	 */
	static int sgs::object_get_position(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();
		
		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);
		Vector2f vPosition = pObject->get_position();

		SG::get_world()->end_world_transaction();

		lua_pushnumber(L, (lua_Number)vPosition.x);
		lua_pushnumber(L, (lua_Number)vPosition.y);

		return 2;
	}

	/*
	 * Callback for object_get_velocity.
	 */
	static int sgs::object_get_velocity(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if(pObject->instance_get_flags() & IPhysicalObject::InstanceFlag)
		{
			IPhysicalObject * pPhyObject = pObject->get_physical();

			Vector2f vVelocity = pPhyObject->get_velocity();

			SG::get_world()->end_world_transaction();

			lua_pushnumber(L, (lua_Number)vVelocity.x);
			lua_pushnumber(L, (lua_Number)vVelocity.y);

			return 2;
		}
		else
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "instance is not a world object");
			lua_error(L);
		}

		//to eliminate the warning
		return -1;
	}

	/*
	 * Callback for object_get_acceleration.
     */
	static int sgs::object_get_acceleration(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if (pObject->instance_get_flags() & IPhysicalObject::InstanceFlag)
		{
			IPhysicalObject * pPhyObject = pObject->get_physical();

			Vector2f vAcceleration = pPhyObject->get_acceleration();

			SG::get_world()->end_world_transaction();

			lua_pushnumber(L, (lua_Number)vAcceleration.x);
			lua_pushnumber(L, (lua_Number)vAcceleration.y);

			return 2;
		}
		else
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "instance is not a world object");
			lua_error(L);
		}

		//to eliminate the warning
		return -1;
	}

	/*
	 * Callback for object_get_angular_velocity.
	 */
	static int sgs::object_get_angular_velocity(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if(pObject->instance_get_flags() & IPhysicalObject::InstanceFlag)
		{
			IPhysicalObject * pPhyObject = pObject->get_physical();

			float flAngularVelocity = pPhyObject->get_angular_velocity();

			SG::get_world()->end_world_transaction();

			lua_pushnumber(L, (lua_Number)flAngularVelocity);

			return 1;
		}
		else
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "instance is not a world object");
			lua_error(L);
		}

		//to eliminate the warning
		return -1;
	}

	/*
	 * Callback for object_get_angular_acceleration.
	 */
	static int sgs::object_get_angular_acceleration(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if(pObject->instance_get_flags() & IPhysicalObject::InstanceFlag)
		{
			IPhysicalObject * pPhyObject = dynamic_cast<IPhysicalObject *>(pObject);

			float flAngularAccel = pPhyObject->get_angular_velocity();

			SG::get_world()->end_world_transaction();

			lua_pushnumber(L, (lua_Number)flAngularAccel);

			return 1;
		}
		else
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "instance is not a world object");
			lua_error(L);
		}

		//to eliminate the warning
		return -1;
	}

	/*
	 * Callback for object_get_rotation.
	 */
	static int sgs::object_get_rotation(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);
		float flRotation = pObject->get_rotation();

		SG::get_world()->end_world_transaction();

		lua_pushnumber(L, (lua_Number)flRotation);

		return 1;
	}

	/*
	* Send a comm to the player.
	* First argument is the sender name,
	* second argument is the text.
	*/
	static int sgs::send_comm(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		char const * szSender = lua_tostring(L, 1);
		char const * szMessage = lua_tostring(L, 2);

		CCommsManager *pCommsManager = SG::get_comms_manager();
		try
		{
			pCommsManager->send_comm(szSender, szMessage);
		}
		catch (SGException e)
		{
			lua_pushstring(L, ("send_comm threw: " + e.get_exception()).c_str());
			lua_error(L);
		}

		return 0;
	}

	/*
	* Callback for object_exists.
	*/
	static int sgs::object_exists(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		lua_pushboolean(L, (int)SG::get_engine()->instance_is_allocated(iInstanceId));

		return 1;
	}

	/*
	* Callback for ship_create
	*/
	static int sgs::ship_create(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 4)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Character const *pCharacter = SG::get_game_data_manager()->get_character((CharacterId const)lua_tointeger(L, 1));

		if(!pCharacter)
		{
			lua_pushstring(L, "invalid character ID");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		float flRotation = (float)lua_tonumber(L, 4);

		InstanceId newInstance = SG::get_world()->instantiate_character(pCharacter, vPosition, flRotation);

		lua_pushinteger(L, (lua_Integer)newInstance);

		return 1;
	}

	/*
	* Callback for dockbuoy_create
	*/
	static int sgs::dockbuoy_create(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 4)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isinteger(L, 4))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		FactionId iOwningFaction = (FactionId)lua_tointeger(L, 1);
		BaseId iTargetBase = (SectorId)lua_tointeger(L, 4);
		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));

		InstanceId newInstance;
		{
			CLandingBuoy *buoy;
			CInstanceFactory::create<CLandingBuoy>(newInstance, buoy);

			CLandingBuoy::CreationParameters parms;
			parms.vPosition = vPosition;
			parms.vVelocity = Vector2f(0.0f, 0.0f);
			parms.flAngularVelocity = 0.0f;
			parms.flRotation = 0.0f;
			parms.uiAppearanceId = 20U;
			parms.flRadius = 64.0f;
			parms.pStartingLoadout = {};
			parms.szName = "Docking Buoy";
			parms.pFaction = SG::get_game_data_manager()->get_faction(iOwningFaction);
			parms.pPilot = nullptr;
			parms.bActivated = true;
			parms.szRadarClass = "Docking Buoy";
			parms.iTargetBase = iTargetBase;
			parms.ulExpValue = 100UL;
			parms.pParentEntity = nullptr;
			buoy->initialize(parms);

			SG::get_world()->instance_add(buoy);
		}

		lua_pushinteger(L, (lua_Integer)newInstance);

		return 1;
	}

	/*
	* Callback for jumpbuoy_create
	*/
	static int sgs::jumpbuoy_create(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 6)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isinteger(L, 4) || !lua_isnumber(L, 5) || !lua_isnumber(L, 6))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		FactionId iOwningFaction = (FactionId)lua_tointeger(L, 1);
		SectorId iDestinationSector = (SectorId)lua_tointeger(L, 4);
		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		Vector2f vDestinationPosition = Vector2f((float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6));

		InstanceId newInstance;
		{
			CSectorJumpBuoy *buoy;
			CInstanceFactory::create<CSectorJumpBuoy>(newInstance, buoy);

			CSectorJumpBuoy::CreationParameters parms;
			parms.vPosition = vPosition;
			parms.vVelocity = Vector2f(0.0f, 0.0f);
			parms.flRotation = 0.0f;
			parms.flAngularVelocity = 0.0f;
			parms.uiAppearanceId = 20U;
			parms.flRadius = 64.0f;
			parms.pStartingLoadout = {};
			parms.szName = "Jump Buoy";
			parms.pFaction = SG::get_game_data_manager()->get_faction(iOwningFaction);
			parms.pPilot = nullptr;
			parms.bActivated = true;
			parms.szRadarClass = "Jump Buoy";
			parms.iDestinationSector = iDestinationSector;
			parms.vDestinationPosition = vDestinationPosition;
			parms.ulExpValue = 100UL;
			parms.pParentEntity = nullptr;
			buoy->initialize(parms);

			SG::get_world()->instance_add(buoy);
		}

		lua_pushinteger(L, (lua_Integer)newInstance);

		return 1;
	}

	/*
	* Callback for staticobj_create
	*/
	static int sgs::staticobj_create(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 7)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 5) || !lua_isnumber(L, 6) || !lua_isnumber(L, 7))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		StaticTextureId iTextureId = (StaticTextureId)lua_tointeger(L, 1);
		float flPositionX = (float)lua_tonumber(L, 2);
		float flPositionY = (float)lua_tonumber(L, 3);
		float flPositionZ = (float)lua_tonumber(L, 4);
		float flRotation = (float)lua_tonumber(L, 5);
		Vector2f vBounds((float)lua_tonumber(L, 6), (float)lua_tonumber(L, 7));

		if(!iTextureId)
		{
			lua_pushstring(L, "invalid static texture ID");
			lua_error(L);
		}

		SG::get_world()->create_static(iTextureId, flPositionX, flPositionY, flPositionZ, flRotation, vBounds);

		return 0;
	}

	/*
	* Callback for object_goto_directive
	*/
	static int sgs::object_goto_directive(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 4)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if (!(pObject->instance_get_flags() & CEquippedObject::InstanceFlag))
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object is not a CEquippedObject");
			lua_error(L);
		}

		CEquippedObject *pEqObj = static_cast<CEquippedObject *>(pObject);

		InstanceId iAIController = pEqObj->get_ai_controller();

		if (iAIController == INVALID_INSTANCE)
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object has no AI controller");
			lua_error(L);
		}

		IWorldInstance *pInstance = nullptr;

		try
		{
			pInstance = SG::get_engine()->instance_get(iAIController);
		}
		catch (SGException e)
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object's AI controller is invalid");
			lua_error(L);
		}

		CAIController *pAIController = nullptr;

		if (pInstance->instance_get_flags() & CAIController::InstanceFlag)
		{
			pAIController = static_cast<CAIController*>(pInstance);
		}
		else
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "object's AI controller is not an AI controller");
			lua_error(L);
		}

		//now we finally have the AI controller...
		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		float flStopRadius = (float)lua_tonumber(L, 4);

		pAIController->goto_directive(vPosition);
		
		SG::get_world()->end_world_transaction();
			
		return 0;
	}

	/*
	* Callback for object_chill_directive
	*/
	static int sgs::object_chill_directive(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject* pObject = sgs::worldobject_from_id(L, iInstanceId);

		if (!(pObject->instance_get_flags() & CEquippedObject::InstanceFlag))
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object is not a CEquippedObject");
			lua_error(L);
		}

		CEquippedObject* pEqObj = static_cast<CEquippedObject*>(pObject);

		InstanceId iAIController = pEqObj->get_ai_controller();

		if (iAIController == INVALID_INSTANCE)
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object has no AI controller");
			lua_error(L);
		}

		IWorldInstance* pInstance = nullptr;

		try
		{
			pInstance = SG::get_engine()->instance_get(iAIController);
		}
		catch (SGException e)
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object's AI controller is invalid");
			lua_error(L);
		}

		CAIController* pAIController = nullptr;

		if (pInstance->instance_get_flags() & CAIController::InstanceFlag)
		{
			pAIController = static_cast<CAIController*>(pInstance);
		}
		else
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "object's AI controller is not an AI controller");
			lua_error(L);
		}

		//now we finally have the AI controller...
		pAIController->chill_directive();

		SG::get_world()->end_world_transaction();

		return 0;
	}

	/*
	* Callback for object_set_target
	*/
	static int sgs::object_set_target(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isinteger(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if (!(pObject->instance_get_flags() & CShip::InstanceFlag))
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object is not a CShip");
			lua_error(L);
		}

		CShip *pShip = static_cast<CShip *>(pObject);

		pShip->set_target((InstanceId const)lua_tointeger(L, 2));

		if (iInstanceId == SG::get_world()->get_player_unchecked())
		{
			SG::get_audio_manager()->play_sound(30U);
		}
		
		SG::get_world()->end_world_transaction();

		return 0;
	}

	/*
	* Callback for object_set_attitude
	*/
	static int sgs::object_set_attitude(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 3)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isinteger(L, 2) || !lua_isnumber(L, 3))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstance1Id = (InstanceId const)lua_tointeger(L, 1);
		InstanceId const iInstance2Id = (InstanceId const)lua_tointeger(L, 2);
		Attitude flAttitude = (float)lua_tonumber(L, 3);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject1 = sgs::worldobject_from_id(L, iInstance1Id);
		
		if(!(pObject1->instance_get_flags() & CEquippedObject::InstanceFlag))
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object is not a CEquippedObject");
			lua_error(L);
		}

		CEquippedObject *pEqObj1 = static_cast<CEquippedObject *>(pObject1);

		CAttitudeSet attitudeSet = pEqObj1->get_attitude_set();
		attitudeSet.set_attitude(iInstance2Id, flAttitude);
		pEqObj1->set_attitude_set(attitudeSet);

		SG::get_world()->end_world_transaction();

		return 0;
	}

	/*
	* Callback for object_get_attitude
	*/
	static int sgs::object_get_attitude(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isinteger(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstance1Id = (InstanceId const)lua_tointeger(L, 1);
		InstanceId const iInstance2Id = (InstanceId const)lua_tointeger(L, 2);
		Attitude flAttitude;

		SG::get_world()->begin_world_transaction();

		IWorldObject* pObject1 = sgs::worldobject_from_id(L, iInstance1Id);

		if (!(pObject1->instance_get_flags() & CEquippedObject::InstanceFlag))
		{
			SG::get_world()->end_world_transaction();
			lua_pushstring(L, "object is not a CEquippedObject");
			lua_error(L);
		}

		CEquippedObject* pEqObj1 = static_cast<CEquippedObject*>(pObject1);

		CAttitudeSet attitudeSet = pEqObj1->get_attitude_set();
		flAttitude = attitudeSet.get_attitude(iInstance2Id);

		SG::get_world()->end_world_transaction();

		lua_pushnumber(L, (lua_Number)flAttitude);

		return 1;
	}

	/*
	* Callback for instant_dock
	*/
	static int sgs::instant_dock(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iBaseId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_game_state_manager()->transition_game_state(new CBaseTransitionState(iBaseId));

		return 0;
	}

	/*
	* Callback for instant_jump
	*/
	static int sgs::instant_jump(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 3)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iSystemId = (InstanceId const)lua_tointeger(L, 1);
		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));

		SG::get_game_state_manager()->transition_game_state(new CSectorTransitionState(iSystemId, "", vPosition));

		return 0;
	}

	/*
	* Get the player instance ID
	*/
	static int sgs::world_get_player(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		lua_pushinteger(L, (int)SG::get_world()->get_player());

		return 1;
	}

	/*
	* Get the player level
	*/
	static int sgs::player_get_level(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		lua_pushinteger(L, (int)SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_level());

		return 1;
	}

	/*
	* Callback for object_get_distance_to_point
	*/
	static int sgs::object_get_distance_to_point(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 3)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);
		Vector2f vObjPosition = pObject->get_position();

		SG::get_world()->end_world_transaction();

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));

		float flDistance = vObjPosition.distance(vPosition);

		lua_pushnumber(L, (lua_Number)flDistance);

		return 1;
	}

	/*
	* Callback for object_get_distance_to_object
	*/
	static int sgs::object_get_distance_to_object(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isinteger(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);
		InstanceId const iInstance2Id = (InstanceId const)lua_tointeger(L, 2);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject1 = sgs::worldobject_from_id(L, iInstanceId);
		Vector2f vObj1Position = pObject1->get_position();

		IWorldObject * pObject2 = sgs::worldobject_from_id(L, iInstance2Id);
		Vector2f vObj2Position = pObject2->get_position();

		SG::get_world()->end_world_transaction();

		float flDistance = vObj1Position.distance(vObj2Position);

		lua_pushnumber(L, (lua_Number)flDistance);

		return 1;
	}

	/*
	* Callback for random_float
	*/
	static int sgs::random_float(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		float const flMin = lua_tonumber(L, 1);
		float const flMax = lua_tonumber(L, 2);

		std::uniform_real_distribution<float> dist(flMin, flMax);

		lua_pushnumber(L, (lua_Number)dist(SG::get_random()));

		return 1;
	}

	/*
	* Callback for random_int
	*/
	static int sgs::random_int(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1) || !lua_isinteger(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		int const iMin = (InstanceId const)lua_tointeger(L, 1);
		int const iMax = (InstanceId const)lua_tointeger(L, 2);

		std::uniform_int_distribution<int> dist(iMin, iMax);

		lua_pushinteger(L, (lua_Integer)dist(SG::get_random()));

		return 1;
	}

	/*
	* Callback for object_get_name.
	*/
	static int sgs::object_get_name(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject * pObject = sgs::worldobject_from_id(L, iInstanceId);

		if (!(pObject->instance_get_flags() & CShip::InstanceFlag))
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "object is not a CShip");
			lua_error(L);
		}

		CShip *pShip = static_cast<CShip *>(pObject);

		std::string szName = pShip->get_name();

		SG::get_world()->end_world_transaction();

		lua_pushstring(L, szName.c_str());

		return 1;
	}

	/*
	* Callback for waypoint_set
	*/
	static int sgs::waypoint_set(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));

		CInSpaceState *pState = dynamic_cast<CInSpaceState*>(SG::get_game_state_manager()->get_game_state());

		//the player not being in space is not a cause for error
		if(pState != nullptr)
		{
			pState->set_waypoint(vPosition);
		}

		return 0;
	}

	/*
	* Callback for effect_jump_in
	*/
	static int sgs::effect_jump_in(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));

		SG::get_audio_manager()->play_sound_spatialized(12, vPosition);
		SG::get_particle_manager()->add_particle(10, vPosition, Vector2f(0.0f, 0.0f), 0.0f, 0.0f);

		return 0;
	}

	/*
	* Callback for map_add_square
	*/
	static int sgs::map_add_square(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 5)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isinteger(L, 3) || !lua_isinteger(L, 4) || !lua_isinteger(L, 5))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));
		MapColor color{ lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5) };

		SG::get_render_pipeline()->get_sector_map_renderer()->add_map_object(MapObjectShape::SQUARE, vPosition, color);

		return 0;
	}

	/*
	* Callback for map_add_circle
	*/
	static int sgs::map_add_circle(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 5)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isinteger(L, 3) || !lua_isinteger(L, 4) || !lua_isinteger(L, 5))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));
		MapColor color{ lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5) };

		SG::get_render_pipeline()->get_sector_map_renderer()->add_map_object(MapObjectShape::CIRCLE, vPosition, color);

		return 0;
	}

	/*
	* Callback for map_add_zone_circular
	*/
	static int sgs::map_add_zone_circular(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 6)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isinteger(L, 4) || !lua_isinteger(L, 5) || !lua_isinteger(L, 6))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));
		float flRadius = (float)lua_tonumber(L, 3);
		MapColor color{ lua_tointeger(L, 4), lua_tointeger(L, 5), lua_tointeger(L, 6) };

		SG::get_render_pipeline()->get_sector_map_renderer()->add_map_zone_circular(vPosition, flRadius, color);

		return 0;
	}

	/*
	* Callback for map_add_zone_rectangular
	*/
	static int sgs::map_add_zone_rectangular(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 7)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isinteger(L, 5) || !lua_isinteger(L, 6) || !lua_isinteger(L, 7))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));
		Vector2f vDimensions = Vector2f((float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4));
		MapColor color{ lua_tointeger(L, 5), lua_tointeger(L, 6), lua_tointeger(L, 7) };

		SG::get_render_pipeline()->get_sector_map_renderer()->add_map_zone_rectangular(vPosition, vDimensions, color);

		return 0;
	}

	/*
	* Callback for map_init
	*/
	static int sgs::map_init(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		Vector2f vDimensions = Vector2f((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2));

		SG::get_render_pipeline()->get_sector_map_renderer()->initialize_map(vDimensions);

		return 0;
	}

	/*
	* Callback for map_finalize
	*/
	static int sgs::map_finalize(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		SG::get_render_pipeline()->get_sector_map_renderer()->finalize_map();

		return 0;
	}

	/*
	* Callback for waypoint_remove
	*/
	static int sgs::waypoint_remove(lua_State *L)
	{
		int n = lua_gettop(L);

		if (n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		CInSpaceState *pState = dynamic_cast<CInSpaceState*>(SG::get_game_state_manager()->get_game_state());

		//the player not being in space is not a cause for error
		if (pState != nullptr)
		{
			pState->remove_waypoint();
		}

		return 0;
	}

	/*
	* Callback for player_adjust_money
	*/
	static int sgs::player_adjust_money(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if (!lua_isnumber(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		int const iMoneyDelta = (InstanceId const)lua_tointeger(L, 1);

		SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory()->adjust_money(iMoneyDelta);

		//Audio feedback, coin drop sound
		SG::get_audio_manager()->play_sound(13);

		return 0;
	}

	/*
	* Callback for player_get_money
	*/
	static int sgs::player_get_money(lua_State* L)
	{
		int n = lua_gettop(L);

		if (n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		int iMoney = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory()->get_money();

		lua_pushinteger(L, iMoney);

		return 1;
	}

	/*
	* Callback for variable_set
	*/
	static int sgs::variable_set(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 2)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isstring(L, 1) || !lua_isstring(L, 2))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		char const *szKey = lua_tostring(L, 1);
		char const *szValue = lua_tostring(L, 2);
		
		SG::get_intransient_data_manager()->set_string_variable(std::string(szKey), std::string(szValue));

		return 0;
	}

	/*
	* Callback for variable_get
	*/
	static int sgs::variable_get(lua_State *L)
	{
		int n = lua_gettop(L);

		if(n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isstring(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		char const *szKey = lua_tostring(L, 1);

		std::string szValue = SG::get_intransient_data_manager()->get_string_variable(std::string(szKey));

		lua_pushstring(L, szValue.c_str());

		return 1;
	}

	/*
	* Callback for astfield_create
	*/
	static int sgs::astfield_create(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 4)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isinteger(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		AstfieldId uiAstfieldId = (FactionId)lua_tointeger(L, 1);
		Vector2f vPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		float flRadius = (float)lua_tonumber(L, 4);

		InstanceId newInstance;
		{
			CAstfield const &pAstfield = SG::get_universe()->get_astfield(uiAstfieldId);

			CAsteroidField* astfield;
			CInstanceFactory::create<CAsteroidField>(newInstance, astfield);

			CAsteroidField::CreationParameters parms;
			parms.vPosition = vPosition;
			parms.uiAppearanceId = APPEARANCE_NONE;
			
			parms.matType = pAstfield.get_mat_type();
			parms.flDropProbability = pAstfield.get_drop_probability();
			parms.uiDropMinQuantity = pAstfield.get_min_drop_quantity();
			parms.uiDropMaxQuantity = pAstfield.get_max_drop_quantity();
			parms.flPirateAttackChancePerTick = pAstfield.get_pirate_attack_chance_per_tick();
			parms.vPirateEncounterCharacters = pAstfield.get_pirate_encounters();
			parms.flRadius = flRadius;
			parms.uiAstfieldInstanceId = uiAstfieldId;

			astfield->initialize(parms);

			SG::get_world()->instance_add(astfield);
		}

		lua_pushinteger(L, (lua_Integer)newInstance);

		return 1;
	}

	/*
	* Callback for send_notification
	*/
	static int sgs::send_notification(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isstring(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		char const* szText = lua_tostring(L, 1);

		SG::get_game_state_manager()->get_game_state()->state_send_notification(std::string(szText));

		return 0;
	}

	/*
	* Callback for cam_begin_chase_camera.
	*/
	static int sgs::cam_begin_chase_camera(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject* pObject = sgs::worldobject_from_id(L, iInstanceId);

		CListenable* pListenable;

		if(pObject == nullptr || (pListenable = dynamic_cast<CListenable*>(pObject)) == nullptr)
		{
			SG::get_world()->end_world_transaction();

			lua_pushstring(L, "object is invalid or of incorrect type to be the target of a chase camera");
			lua_error(L);
		}

		SG::get_render_pipeline()->remove_active_camera();

		CChaseCamera* cc;
		InstanceId ccid;
		CInstanceFactory::create<CChaseCamera>(ccid, cc);

		pListenable->listener_add(cc);

		cc->acquire_target(pObject);
		cc->alive_set(true);
		cc->set_position(pObject->get_position());
		cc->set_bounds(Vector2f(2.275f * DEFAULT_WINDOW_WIDTH, 2.275f * DEFAULT_WINDOW_HEIGHT));

		SG::get_render_pipeline()->set_active_camera(cc);

		SG::get_world()->end_world_transaction();

		SG::get_world()->instance_add(cc);

		return 0;
	}

	/*
	* Callback for cam_begin_vignette_camera
	*/
	static int sgs::cam_begin_vignette_camera(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 5)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isnumber(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4) || !lua_isnumber(L, 5))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		float flSplineDuration = (float)lua_tonumber(L, 1);
		Vector2f vStartPosition = Vector2f((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		Vector2f vEndPosition = Vector2f((float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5));

		CVignetteCamera* vc;
		InstanceId ccid;
		CInstanceFactory::create<CVignetteCamera>(ccid, vc);

		vc->alive_set(true);
		vc->set_position(vStartPosition);
		vc->set_bounds(Vector2f(2.275f * DEFAULT_WINDOW_WIDTH, 2.275f * DEFAULT_WINDOW_HEIGHT));
		vc->set_origin_point(vStartPosition);
		vc->set_final_point(vEndPosition);
		vc->set_path_duration(flSplineDuration);

		SG::get_render_pipeline()->remove_active_camera();
		SG::get_render_pipeline()->set_active_camera(vc);

		SG::get_world()->instance_add(vc);

		return 0;
	}

	/*
	* Callback for hud_hide
	*/
	static int sgs::hud_hide(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		SG::get_game_state_manager()->get_game_state()->state_enable_input(false);

		return 0;
	}

	/*
	* Callback for hud_show
	*/
	static int sgs::hud_show(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 0)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		SG::get_game_state_manager()->get_game_state()->state_enable_input(true);

		return 0;
	}

	/*
	 * Callback for object_halt
	 */
	static int sgs::object_halt(lua_State* L)
	{
		int n = lua_gettop(L);

		if(n != 1)
		{
			lua_pushstring(L, "incorrect number of arguments");
			lua_error(L);
		}

		if(!lua_isinteger(L, 1))
		{
			lua_pushstring(L, "incorrect arg types");
			lua_error(L);
		}

		InstanceId const iInstanceId = (InstanceId const)lua_tointeger(L, 1);

		SG::get_world()->begin_world_transaction();

		IWorldObject* pObject = sgs::worldobject_from_id(L, iInstanceId);
		
		//if the object implements IPhysicalObject, zero its velocity and angular
		//velocity
		if(pObject->instance_get_flags() & IPhysicalObject::InstanceFlag)
		{
			IPhysicalObject* pPhysicalObject = dynamic_cast<IPhysicalObject*>(pObject);

			pPhysicalObject->set_velocity(Vector2f(0.0f, 0.0f));
			pPhysicalObject->set_angular_velocity(0.0f);
		}

		//if the object is a CShip, we want to set its throttle values to 0 so it
		//doesn't immediately start building up speed again
		if(pObject->instance_get_flags() & CShip::InstanceFlag)
		{
			CShip *pShip = static_cast<CShip*>(pObject);

			pShip->set_throttle(0.0f);
			pShip->set_spin_throttle(0.0f);
			pShip->set_strafe_throttle(0.0f);
		}

		SG::get_world()->end_world_transaction();

		return 0;
	}
}

/*
 * Register all callback functions with the script engine.
 * Invoked by 
 */
void sgs::register_callbacks(void)
{
	IScriptEngine *pScriptEngine = SG::get_script_engine();

	pScriptEngine->register_callback("sgs_enqueue_callback", &sgs::enqueue_callback);
	pScriptEngine->register_callback("sgs_ship_create", &sgs::ship_create);
	pScriptEngine->register_callback("sgs_jumpbuoy_create", &sgs::jumpbuoy_create);
	pScriptEngine->register_callback("sgs_dockbuoy_create", &sgs::dockbuoy_create);
	pScriptEngine->register_callback("sgs_astfield_create", &sgs::astfield_create);
	pScriptEngine->register_callback("sgs_staticobj_create", &sgs::staticobj_create);
	pScriptEngine->register_callback("sgs_object_set_target", &sgs::object_set_target);
	pScriptEngine->register_callback("sgs_object_get_position", &sgs::object_get_position);
	pScriptEngine->register_callback("sgs_object_get_velocity", &sgs::object_get_velocity);
	pScriptEngine->register_callback("sgs_object_get_acceleration", &sgs::object_get_acceleration);
	pScriptEngine->register_callback("sgs_object_get_rotation", &sgs::object_get_rotation);
	pScriptEngine->register_callback("sgs_object_get_angular_velocity", &sgs::object_get_angular_velocity);
	pScriptEngine->register_callback("sgs_object_get_angular_acceleration", &sgs::object_get_angular_acceleration);
	pScriptEngine->register_callback("sgs_object_exists", &sgs::object_exists);
	pScriptEngine->register_callback("sgs_send_comm", &sgs::send_comm);
	pScriptEngine->register_callback("sgs_object_set_attitude", &sgs::object_set_attitude);
	pScriptEngine->register_callback("sgs_object_get_attitude", &sgs::object_get_attitude);
	pScriptEngine->register_callback("sgs_object_goto_directive", &sgs::object_goto_directive);
	pScriptEngine->register_callback("sgs_world_get_player", &sgs::world_get_player);
	pScriptEngine->register_callback("sgs_object_get_distance_to_object", &sgs::object_get_distance_to_object);
	pScriptEngine->register_callback("sgs_object_get_distance_to_point", &sgs::object_get_distance_to_point);
	pScriptEngine->register_callback("sgs_ship_get_name", &sgs::object_get_name);
	pScriptEngine->register_callback("sgs_random_float", &sgs::random_float);
	pScriptEngine->register_callback("sgs_random_int", &sgs::random_int);
	pScriptEngine->register_callback("sgs_waypoint_set", &sgs::waypoint_set);
	pScriptEngine->register_callback("sgs_waypoint_remove", &sgs::waypoint_remove);
	pScriptEngine->register_callback("sgs_effect_jump_in", &sgs::effect_jump_in);
	pScriptEngine->register_callback("sgs_set_variable", &sgs::variable_set);
	pScriptEngine->register_callback("sgs_get_variable", &sgs::variable_get);
	pScriptEngine->register_callback("sgs_instant_dock", &sgs::instant_dock);
	pScriptEngine->register_callback("sgs_instant_jump", &sgs::instant_jump);
	pScriptEngine->register_callback("sgs_player_get_level", &sgs::player_get_level);
	pScriptEngine->register_callback("sgs_player_adjust_money", &sgs::player_adjust_money);
	pScriptEngine->register_callback("sgs_player_get_money", &sgs::player_get_money);
	pScriptEngine->register_callback("sgs_object_chill_directive", &sgs::object_chill_directive);
	pScriptEngine->register_callback("sgs_map_init", &sgs::map_init);
	pScriptEngine->register_callback("sgs_map_finalize", &sgs::map_finalize);
	pScriptEngine->register_callback("sgs_map_add_square", &sgs::map_add_square);
	pScriptEngine->register_callback("sgs_map_add_circle", &sgs::map_add_circle);
	pScriptEngine->register_callback("sgs_map_add_zone_rectangular", &sgs::map_add_zone_rectangular);
	pScriptEngine->register_callback("sgs_map_add_zone_circular", &sgs::map_add_zone_circular);
	pScriptEngine->register_callback("sgs_send_notification", &sgs::send_notification);
	pScriptEngine->register_callback("sgs_cam_begin_chase_camera", &sgs::cam_begin_chase_camera);
	pScriptEngine->register_callback("sgs_cam_begin_vignette_camera", &sgs::cam_begin_vignette_camera);
	pScriptEngine->register_callback("sgs_hud_hide", &sgs::hud_hide);
	pScriptEngine->register_callback("sgs_hud_show", &sgs::hud_show);
	pScriptEngine->register_callback("sgs_object_halt", &sgs::object_halt);
}

/*
 * Helper function to convert an instance id
 * to a world object pointer.
 *
 * Should ONLY be used from sgs context.
 * 
 * Call world->world_end_transaction after done using the object.
 */
IWorldObject *sgs::worldobject_from_id(lua_State *L, InstanceId const iInstanceId)
{
	IEngine *pEngine = SG::get_engine();
	IWorldInstance *pInstance = nullptr;

	try
	{
		pInstance = pEngine->instance_get(iInstanceId);
	}
	catch (SGException e)
	{
		lua_pushstring(L, "invalid instance id");
		lua_error(L);
	}

	if (pInstance->instance_get_flags() & IWorldObject::InstanceFlag)
	{
		IWorldObject *pObject = static_cast<IWorldObject*>(pInstance);
		return pObject;
	}
	else
	{
		lua_pushstring(L, "instance is not a world object");
		lua_error(L);

		return nullptr;
	}
}