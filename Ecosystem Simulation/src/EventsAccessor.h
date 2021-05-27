#pragma once

static class EventsAccessor
{
public:
	static bool hasEventOccured(
		const sf::Event::EventType& event_type,
		const std::vector<sf::Event>& events
	);
};
