#include "role.h"
#include "guild.h"

namespace discpp {
	Role::Role(const Snowflake& role_id, const discpp::Guild& guild) : DiscordObject(role_id) {
		auto it = guild.roles.find(role_id);
		if (it != guild.roles.end()) {
			*this = *it->second;
		}
	}

	Role::Role(const discpp::JsonObject& json) {
		id = SnowflakeFromString(json["id"].GetString());
		name = json["name"].GetString();
		color = json["color"].GetInt();
        if (json.Get<bool>("hoist")) {
            flags |= 0b1;
        }
		position = json["position"].GetInt();
        permissions = Permissions(PermissionType::ROLE, json["permissions"].GetInt());
        if (json.Get<bool>("managed")) {
            flags |= 0b10;
        }
        if (json.Get<bool>("mentionable")) {
            flags |= 0b100;
        }
	}

    bool Role::IsHoistable() const {
        return (flags & 0b1) == 0b1;
    }

    bool Role::IsManaged() const {
        return (flags & 0b10) == 0b10;
    }

    bool Role::IsMentionable() const {
        return (flags & 0b100) == 0b100;
    }
}