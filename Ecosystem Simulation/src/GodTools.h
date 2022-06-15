#pragma once

#include "Libraries.h"

enum class GodTool { NONE, MUTATE, TRACK, KILL, REPLACE, BRAIN, STOP, INFO };

std::string getGodToolStr(GodTool godTool);

GodTool getGodTool(const std::string& key);

std::ostream& operator<<(std::ostream& os, const GodTool& rhs);

std::istream& operator>>(std::istream& is, GodTool& rhs);
