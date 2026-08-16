#pragma once
#include <string>
#include <fstream>
#include "../../../../utilities/convertString/convertString.h"



std::ofstream DebugLogInitialize();
void Log(std::string const& str_);
void Log(std::ostream& os, std::string const& message);

