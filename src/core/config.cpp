#include "core.h"
#include "config.h"

Config& Config::Instance()
{
	static Config conf;
	return conf;
}
