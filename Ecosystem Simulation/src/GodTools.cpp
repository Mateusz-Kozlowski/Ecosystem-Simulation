#include "GodTools.h"

std::string getGodToolStr(GodTool godTool)
{
    switch (godTool)
    {
    case GodTool::NONE:
        return "NONE";

    case GodTool::MUTATE:
        return "MUTATE";

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
        throw std::invalid_argument(
            Blueberry::Formatter()
            << "Error::getGodToolStr(GodTool)::"
            << "there is no such tool as "
            << godTool
            << '\n'
        );
    }
}

GodTool getGodTool(const std::string& key)
{
    if (key == "NONE")
        return GodTool::NONE;

    else if(key == "MUTATE")
        return GodTool::MUTATE;

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
    {
        throw std::invalid_argument(
            Blueberry::Formatter()
            << "Error::getGodTool(const std::string&)::"
            << "there is no such a tool as "
            << key
            << '\n'
        );
    }
}

std::ostream& operator<<(std::ostream& os, const GodTool& rhs)
{
    return os << getGodToolStr(rhs);
}

std::istream& operator>>(std::istream& is, GodTool& rhs)
{
    std::string value;

    if (is >> value)
    {
        rhs = getGodTool(value.c_str());
    }

    return is;
}
