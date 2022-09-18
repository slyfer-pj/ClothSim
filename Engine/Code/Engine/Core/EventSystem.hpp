#pragma once
#include <vector>
#include <string>
#include <mutex>
#include "Engine/Core/NamedStrings.hpp"

typedef NamedStrings EventArgs;
typedef bool (*EventCallbackFunction) (EventArgs& args);

struct EventSubscription
{
	EventCallbackFunction m_eventCallBack = nullptr;
};

struct EventSystemConfig
{

};

typedef std::vector<EventSubscription> SubscriptionList;

class EventSystem
{
public:
	EventSystem(const EventSystemConfig& config);
	~EventSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void SubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr);
	void FireEvent(const std::string& eventName, EventArgs& args);
	void FireEvent(const std::string& eventName);
	void GetRegisteredEventNames(std::vector<std::string>& outNames) const;

protected:
	EventSystemConfig m_config;
	std::map<std::string, SubscriptionList> m_subscriptionListByEventName;
	mutable std::mutex m_subscriptionListMutex;
};

void SubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(const std::string& eventName, EventCallbackFunction functionPtr);
void FireEvent(const std::string& eventName, EventArgs& args);
void FireEvent(const std::string& eventName);