#pragma once
#include "Engine/Core/NamedStrings.hpp"

class DevConsole;
class EventSystem;

#define UNUSED(x) (void)(x);

extern NamedStrings g_gameConfigBlackboard;
extern DevConsole* g_theConsole;
extern EventSystem* g_theEventSystem;

void LoadGameConfigBlackboard();