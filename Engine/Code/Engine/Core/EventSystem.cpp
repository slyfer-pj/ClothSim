#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

EventSystem* g_theEventSystem = nullptr;

EventSystem::EventSystem(const EventSystemConfig& config)
{
	m_config = config;
}

EventSystem::~EventSystem()
{

}

void EventSystem::Startup()
{

}

void EventSystem::Shutdown()
{

}

void EventSystem::BeginFrame()
{

}

void EventSystem::EndFrame()
{

}

void EventSystem::SubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr)
{
	std::map<std::string, SubscriptionList>::const_iterator iter;
	m_subscriptionListMutex.lock();
	iter = m_subscriptionListByEventName.find(eventName);
	if (iter == m_subscriptionListByEventName.end())
	{
		SubscriptionList subList;
		EventSubscription eventSub;
		eventSub.m_eventCallBack = functionPtr;
		subList.push_back(eventSub);
		m_subscriptionListByEventName[eventName] = subList;
	}
	else
	{
		SubscriptionList subList = iter->second;
		EventSubscription eventSub;
		eventSub.m_eventCallBack = functionPtr;
		subList.push_back(eventSub);
	}
	m_subscriptionListMutex.unlock();
}

void EventSystem::UnsubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr)
{
	std::map<std::string, SubscriptionList>::const_iterator iter;
	bool unsubscribeSuccess = false;
	m_subscriptionListMutex.lock();
	iter = m_subscriptionListByEventName.find(eventName);
	if (iter != m_subscriptionListByEventName.end())
	{
		SubscriptionList subList = iter->second;
		for (int i = 0; i < subList.size(); i++)
		{
			if (subList[i].m_eventCallBack == functionPtr)
				subList[i].m_eventCallBack = nullptr;
		}
		unsubscribeSuccess = true;
	}
	m_subscriptionListMutex.unlock();

	if(!unsubscribeSuccess)
	{
		GUARANTEE_OR_DIE(false, "Trying to unsubscribe from event that doesn't exist!");
	}
}

void EventSystem::FireEvent(const std::string& eventName, EventArgs& args)
{
	std::map<std::string, SubscriptionList>::const_iterator iter;
	bool fireEventSuccess = false;
	SubscriptionList subList;
	m_subscriptionListMutex.lock();
	iter = m_subscriptionListByEventName.find(eventName);
	if (iter != m_subscriptionListByEventName.end())
	{
		subList = iter->second;
	}
	m_subscriptionListMutex.unlock();

	for (int i = 0; i < subList.size(); i++)
	{
		fireEventSuccess = true;
		bool consumed = subList[i].m_eventCallBack(args);
		if (consumed)
			break;
	}

	if(!fireEventSuccess)
	{
		DebuggerPrintf("Trying to fire and event that doesn't exist.");
	}
}

void EventSystem::FireEvent(const std::string& eventName)
{
	EventArgs args;
	FireEvent(eventName, args);
}

void EventSystem::GetRegisteredEventNames(std::vector<std::string>& outNames) const
{
	m_subscriptionListMutex.lock();
	for (auto itr = m_subscriptionListByEventName.begin(); itr != m_subscriptionListByEventName.end(); ++itr)
	{
		SubscriptionList subList = itr->second;
		if (subList.size() > 0)
		{
			outNames.push_back(itr->first);
		}
	}
	m_subscriptionListMutex.unlock();
}

void SubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
	}
}

void UnsubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
	}
}

void FireEvent(const std::string& eventName, EventArgs& args)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->FireEvent(eventName, args);
	}
}

void FireEvent(const std::string& eventName)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->FireEvent(eventName);
	}
}
