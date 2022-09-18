#pragma once
#include <stdint.h>
#include <string>
#include <vector>

struct NetAddress
{
public:
	unsigned int address = 0U;
	uint16_t port = 0;

public:
	static NetAddress FromString(const std::string& addressString);
	static NetAddress GetLoopBack(uint16_t port);
	static std::vector<NetAddress> GetAllInternal(uint16_t port);
	std::string ToString();
};