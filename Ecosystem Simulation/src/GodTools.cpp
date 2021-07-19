#include "stdafx.h"
#include "GodTools.h"

std::string getGodToolStr(GodTool god_tool)
{
    switch (god_tool)
    {
    case GodTool::NONE:
        return "NONE";

    case GodTool::TRACK:
        return "TRACK";

    case GodTool::KILL:
        return "KILL";

    case GodTool::REPLACE:
        return "REPLACE";

    case GodTool::BRAIN:
        return "BRAIN";

    case GodTool::STOP:
        return "STOP";

    case GodTool::INFO:
        return "INFO";

    default:
        throw "ERROR::getGodToolStr::THE FUNCTION SEEMS TO BE OUT OF DATE - IT DOESN'T TAKE INTO ACCOUNT ALL GOD TOOLS\n";
    }
}

GodTool getGodTool(const std::string& key)
{
    if (key == "NONE")
        return GodTool::NONE;

    else if (key == "TRACK")
        return GodTool::TRACK;

    else if (key == "KILL")
        return GodTool::KILL;

    else if (key == "REPLACE")
        return GodTool::REPLACE;

    else if (key == "BRAIN")
        return GodTool::BRAIN;

    else if (key == "STOP")
        return GodTool::STOP;

    else if (key == "INFO")
        return GodTool::INFO;

    else
        throw("ERROR::getGodTool::THE FUNCTION SEEMS TO BE OUT OF DATE - IT DOESN'T TAKE INTO ACCOUNT ALL GOD TOOLS\n");
}

std::ostream& operator<<(std::ostream& os, const GodTool& rhs)
{
    os << getGodToolStr(rhs);

    return os;
}

std::istream& operator>>(std::istream& is, GodTool& rhs)
{
    std::string value;

    if (is >> value)
        rhs = getGodTool(value.c_str());

    return is;
}
