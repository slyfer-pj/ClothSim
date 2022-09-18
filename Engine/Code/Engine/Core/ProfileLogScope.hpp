#pragma once
#include <stdint.h>

#define PROFILE_LOG_SCOPE(tag) ProfileLogScope _log_scope_##tag(#tag);

class ProfileLogScope
{
public:
	ProfileLogScope(const char* tag);
	~ProfileLogScope();

private:
	uint64_t m_startRawTime = 0;
	char const* m_tag;
};