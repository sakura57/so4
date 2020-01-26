#pragma once

#include <mutex>
#include <queue>

#include "Util.hxx"
#include "IScriptEngine.hxx"

class CLuaEngine : public IScriptEngine
{
public:
	CLuaEngine();
	~CLuaEngine();

	virtual void initialize(void) final;

	virtual void script_tick(float const) final;

	//load and enqueue a script's main method for execution
	virtual void script_enqueue(char const *) final;

	//enqueue a particular function for execution,
	//callback is invoked after a certain amount of time
	virtual CallbackId script_enqueue_function(float const, char const *) final;

	//register a C function that corresponds to a particular
	//lua function
	virtual void register_callback(char const *, void *) final;

	//inherited from ITransientStateStructure
	virtual void shifting_out(void) final;

private:
	struct CallbackDescriptor
	{
		CallbackId m_uiCallbackId;
		std::string m_szFunctionName;

		INLINE CallbackDescriptor(CallbackId uiCallbackId, std::string szFunctionName)
			: m_uiCallbackId(uiCallbackId), m_szFunctionName(szFunctionName)
		{ };

		/*
		 * CallbackDescriptor objects will compare equal if they have
		 * the same id
		 */
		INLINE bool operator==(const CallbackId &o) const
		{
			if(this->m_uiCallbackId == o)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};

	struct TimedCallbackDescriptor : public CallbackDescriptor
	{
		float m_flRemainingSeconds;

		INLINE TimedCallbackDescriptor(CallbackId uiCallbackId, std::string szFunctionName, float flRemainingSeconds)
			: CallbackDescriptor(uiCallbackId, szFunctionName), m_flRemainingSeconds(flRemainingSeconds)
		{ };

		INLINE friend bool operator<(TimedCallbackDescriptor const &a, TimedCallbackDescriptor const &b)
		{
			return a.m_flRemainingSeconds > b.m_flRemainingSeconds;
		};

		INLINE friend bool operator>(TimedCallbackDescriptor const &a, TimedCallbackDescriptor const &b)
		{
			return a.m_flRemainingSeconds < b.m_flRemainingSeconds;
		};
	};

	std::mutex m_mLuaAccess;
	lua_State *m_pLuaState;

	std::mutex m_mTimedCallQueueAccess;
	std::vector<TimedCallbackDescriptor> m_qPendingTimedCalls;
	CallbackId m_uiLastTimedCall;
};