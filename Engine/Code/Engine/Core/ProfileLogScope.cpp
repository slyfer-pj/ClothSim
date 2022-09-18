#include "Engine/Core/ProfileLogScope.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/StringUtils.hpp"

extern DevConsole* g_theConsole;

ProfileLogScope::ProfileLogScope(const char* tag)
	:m_tag(tag), m_startRawTime(GetCurrentTimeRaw())
{

}

ProfileLogScope::~ProfileLogScope()
{
	uint64_t duration = GetCurrentTimeRaw() - m_startRawTime;
	if (g_theConsole)
	{
		g_theConsole->AddLine(g_theConsole->INFO_MINOR, Stringf("%s = %f ms", m_tag, ConvertRawTimeToSeconds(duration) * 1000.f));
	}
}

