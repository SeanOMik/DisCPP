#include "emoji.h"
#include "guild.h"
#include "utils.h"
#include "user.h"

namespace discord {
	Emoji::Emoji(std::string name, snowflake id) : name(name), id(id) {

	}

	Emoji::Emoji(discord::Guild guild, snowflake id) {
		auto emoji = std::find_if(guild.emojis.begin(), guild.emojis.end(), [id](discord::Emoji a) { return id == a.id; });

		if (emoji != guild.emojis.end()) {
			*this = *emoji;
		}
	}

	Emoji::Emoji(nlohmann::json json) {
		id = ToSnowflake(json["id"]);
		name = json["name"];
		// roles?
		user = discord::User(GetSnowflakeSafely(json, "user"));
		require_colons = GetDataSafely<bool>(json, "require_colons");
		managed = GetDataSafely<bool>(json, "managed");
		animated = GetDataSafely<bool>(json, "animated");
	}
}