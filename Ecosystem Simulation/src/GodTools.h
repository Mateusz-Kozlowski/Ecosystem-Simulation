#pragma once

enum class GodTool { NONE, TRACK, KILL, REPLACE, BRAIN, STOP, INFO };

std::string getGodToolStr(GodTool god_tool);

GodTool getGodTool(const std::string& key);

std::ostream& operator<<(std::ostream& os, const GodTool& rhs);

std::istream& operator>>(std::istream& is, GodTool& rhs);
