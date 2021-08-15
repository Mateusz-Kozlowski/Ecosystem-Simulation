#include "pch.h"
#include "EventsAccessor.h"

bool EventsAccessor::hasEventOccured(
    const sf::Event::EventType& eventType, 
    const std::vector<sf::Event>& events)
{
    for (const auto& event : events)
    {
        if (event.type == eventType) return true;
    }
    
    return false;
}
