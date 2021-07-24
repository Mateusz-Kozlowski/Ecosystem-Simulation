#include "pch.h"
#include "EventsAccessor.h"

bool EventsAccessor::hasEventOccured(
    const sf::Event::EventType& event_type, 
    const std::vector<sf::Event>& events)
{
    for (const auto& e : events)
        if (e.type == event_type)
            return true;
    
    return false;
}
