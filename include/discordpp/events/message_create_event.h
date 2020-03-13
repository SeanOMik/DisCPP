#ifndef DISCORDPP_MESSAGE_CREATE_EVENT_H
#define DISCORDPP_MESSAGE_CREATE_EVENT_H

#include "event.h"
#include "message.h"

#include <nlohmann/json.hpp>

namespace discord {
	class MessageCreateEvent : public Event {
	public:
		inline MessageCreateEvent(nlohmann::json json) : message(discord::Message(json)) { }
		inline MessageCreateEvent(discord::Message message) : message(message) {}

		discord::Message message;
	};
}

#endif