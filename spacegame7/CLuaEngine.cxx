//TODO: Make callbacks have IDs so they can be unregistered at some point

#include "CLuaEngine.hxx"
#include "LuaCallbacks.hxx"
#include <iostream>

/*
 * Initialize the Lua engine.
 */
CLuaEngine::CLuaEngine(void)
	: m_pLuaState(luaL_newstate())
{
	luaL_openlibs(this->m_pLuaState);

	this->m_uiLastTimedCall = 0;
}

CLuaEngine::~CLuaEngine(void)
{
	lua_close(this->m_pLuaState);
}

void CLuaEngine::initialize(void)
{
	sgs::register_callbacks();
}

void CLuaEngine::script_tick(float const flDelta)
{
	this->m_mTimedCallQueueAccess.lock();

	if(!this->m_qPendingTimedCalls.size())
	{
		//previously, this unlock call was missing here.
		//there was a deadlock which was only detected in the
		//profiling, where script_tick was overrepresented
		this->m_mTimedCallQueueAccess.unlock();

		return;
	}

	for(TimedCallbackDescriptor &cbd : this->m_qPendingTimedCalls)
	{
		cbd.m_flRemainingSeconds -= flDelta;
	}

	TimedCallbackDescriptor *cbdBack = &m_qPendingTimedCalls.back();

	while(cbdBack->m_flRemainingSeconds < 0.0f)
	{
		std::string szFunctionName = cbdBack->m_szFunctionName;

		//to avoid a deadlock on the call queue access spinlock,
		//we will unlock the spinlock before invoking the function.
		//deadlock would otherwise occur if our callee invokes
		//script_enqueue_function
		this->m_mTimedCallQueueAccess.unlock();
		this->m_mLuaAccess.lock();

		lua_getglobal(this->m_pLuaState, szFunctionName.c_str());
		if(lua_pcall(this->m_pLuaState, 0, 1, 0))
		{
			std::string exceptionText = "Failed to execute function " + szFunctionName + "\n";
			exceptionText += lua_tostring(this->m_pLuaState, -1);
			lua_pop(this->m_pLuaState, 1);

			//mutex must be unlocked before we exit this function's context
			this->m_mLuaAccess.unlock();

			throw SGException(exceptionText);
		}

		//unlock the lua access mutex, and re-lock the call queue
		//access spinlock
		this->m_mLuaAccess.unlock();
		this->m_mTimedCallQueueAccess.lock();

		m_qPendingTimedCalls.pop_back();

		if(!m_qPendingTimedCalls.size())
		{
			break;
		}

		cbdBack = &m_qPendingTimedCalls.back();
	}

	this->m_mTimedCallQueueAccess.unlock();
}

/*
 * while this function is called enqueue,
 * it will in fact execute the script immediately.
 */
void CLuaEngine::script_enqueue(char const *szScript)
{
	std::lock_guard<std::mutex> lock(this->m_mLuaAccess);

	std::string szFullScriptPath = SG::get_game_data_manager()->get_full_data_file_path(szScript);

	/*
	 * load the script
	 */
	if(luaL_loadfile(this->m_pLuaState, szFullScriptPath.c_str()))
	{
		std::string exceptionText("Failed to load script.\n");
		exceptionText += lua_tostring(this->m_pLuaState, -1);
		lua_pop(this->m_pLuaState, 1);
		throw SGException(exceptionText);
	}

	/*
	 * execute the script
	 */
	if(lua_pcall(this->m_pLuaState, 0, 1, 0))
	{
		std::string exceptionText = "Failed to execute script " + std::string(szScript) + "\n";
		exceptionText += lua_tostring(this->m_pLuaState, -1);
		lua_pop(this->m_pLuaState, 1);
		throw SGException(exceptionText);
	}
}

/*
 * add a function to the timed callback queue.
 * 
 * we need to maintain sorting of the internal CallbackDescriptor queue,
 * as script_tick() will only check the back of the queue
 */
CallbackId CLuaEngine::script_enqueue_function(float const flTime, char const *szFunction)
{
	std::lock_guard<std::mutex> lock(this->m_mTimedCallQueueAccess);

	if(std::find(this->m_qPendingTimedCalls.begin(), this->m_qPendingTimedCalls.end(), this->m_uiLastTimedCall)
		!= this->m_qPendingTimedCalls.end())
	{
		throw SGException("timed callback id space possibly exhausted");
	}

	this->m_qPendingTimedCalls.emplace_back(TimedCallbackDescriptor(this->m_uiLastTimedCall, szFunction, flTime));

	std::sort(this->m_qPendingTimedCalls.begin(), this->m_qPendingTimedCalls.end());

	return this->m_uiLastTimedCall++;
}


/*
 * register a function with the Lua engine. in most cases, these are functions defined
 * in LuaCallbacks.h.
 * 
 * still thread-safe, so a function can be registered at any point, not just during
 * initialization
 */
void CLuaEngine::register_callback(char const *szFunctionName, void *pFunction)
{
	lua_CFunction luaCFunction = reinterpret_cast<lua_CFunction>(pFunction);

	std::lock_guard<std::mutex> lock(this->m_mLuaAccess);

	lua_pushcfunction(this->m_pLuaState, luaCFunction);
	lua_setglobal(this->m_pLuaState, szFunctionName);
}

void CLuaEngine::shifting_out(void)
{
	std::lock_guard<std::mutex> lock(this->m_mTimedCallQueueAccess);

	this->m_qPendingTimedCalls.clear();
	this->m_uiLastTimedCall = 0;
}
