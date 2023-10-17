/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <vector>

#include "Defs.hxx"
#include "EngineProtos.hxx"

struct Notification
{
	//Note: All implementers of IListenable *MUST* send
	//DEPENDENT_DESTROYED to ALL listeners
	//before destruction! This should usually be done
	//in instance_destroy() for IWorldInstance implementers.
	enum class Type
	{
		DEPENDENT_DESTROYED
	};

	Type m_type;
};

interface IListener
{
public:
	//this function should be overridden by derived classes.
	//handling of notifications
	virtual void listen_notify(Notification const, IListenable *) = 0;
};

interface IListenable
{
public:
	//invoked when this object has been subscribed to
	//by an IListener. From then on, the IListener should
	//be sent relevant notifications via listeners_send().
	virtual void listener_add(IListener *) = 0;

	//invoked when an IListener wishes to unsubscribe.
	virtual void listener_remove(IListener *) = 0;

	//test if a given listener is already listening to us
	virtual bool is_listener(IListener *) = 0;

	//send a notification to all listeners of this object.
	virtual void listeners_send(Notification const) = 0;

	//Removes all current listeners
	virtual void listeners_clear(void) = 0;
};

class CListener : public IListener
{
	//must be implemented by any derived classes
	virtual void listen_notify(Notification const, IListenable *) = 0;
};

/*
 * Implementation of IListenable using a vector field
 * to track listeners.
 */
class CListenable : public IListenable
{
public:
	virtual void listener_add(IListener *pListener) final
	{
		this->m_vecListeners.push_back(pListener);
	};

	virtual void listener_remove(IListener *pListener) final
	{
		for(auto i = this->m_vecListeners.begin(); i != this->m_vecListeners.end(); ++i)
		{
			if(*i == pListener)
			{
				this->m_vecListeners.erase(i);
				break;
			}
		}
	};

	virtual bool is_listener(IListener *pListener) final
	{
		for(auto &pMyListener : this->m_vecListeners)
		{
			if(pListener == pMyListener)
			{
				return true;
			}
		}

		return false;
	};

	virtual void listeners_send(Notification const notification) final
	{
		if (!this->m_vecListeners.size())
		{
			return;
		}

		for(IListener *pListener : this->m_vecListeners)
		{
			pListener->listen_notify(notification, this);
		}
	};

	virtual void listeners_clear(void) final
	{
		if(!this->m_vecListeners.size())
		{
			return;
		}

		this->m_vecListeners.clear();
	};
private:
	std::vector<IListener *> m_vecListeners;
};