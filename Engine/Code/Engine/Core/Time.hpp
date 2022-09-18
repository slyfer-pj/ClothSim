//-----------------------------------------------------------------------------------------------
// Time.hpp
//
#pragma once
#include <stdint.h>


//-----------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds();
uint64_t GetCurrentTimeRaw();
double ConvertRawTimeToSeconds(const uint64_t& rawTime);

	