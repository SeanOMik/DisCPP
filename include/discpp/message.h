#ifndef DISCPP_MESSAGE_H
#define DISCPP_MESSAGE_H

#include "discord_object.h"
#include "channel.h"
#include "user.h"
#include "member.h"
#include "guild.h"
#include "reaction.h"
#include "role.h"
#include "embed_builder.h"
#include "attachment.h"

namespace discpp {
	enum class GetReactionsMethod : int {
		BEFORE_USER,
		AFTER_USER
	};

	struct MessageActivity {
		enum class ActivityType : int {
			NONE = 0,
			JOIN = 1,
			SPECTATE = 2,
			LISTEN = 3,
			JOIN_REQUEST = 5
		};

		ActivityType type;
		std::string party_id;

		MessageActivity() = default;
		MessageActivity(rapidjson::Document& json) {
			type = static_cast<ActivityType>(json["type"].GetInt());
			party_id = GetDataSafely<std::string>(json, "party_id");
		}
	};

	struct MessageApplication : public DiscordObject {
		//snowflake id;
		std::string cover_image;
		std::string description;
		std::string icon;
		std::string name;

		MessageApplication() = default;
		MessageApplication(rapidjson::Document& json) {
			id = SnowflakeFromString(json["id"].GetString());
			cover_image = GetDataSafely<std::string>(json, "cover_image");
			description = json["description"].GetString();
			icon = json["icon"].GetString();
			name = json["name"].GetString();
		}
	};

	struct MessageReference {
		snowflake message_id;
		snowflake channel_id;
		snowflake guild_id;

		MessageReference() = default;
		MessageReference(rapidjson::Document& json) {
			message_id = GetIDSafely(json, "message_id");
			channel_id = SnowflakeFromString(json["channel_id"].GetString());
			guild_id = GetIDSafely(json, "guild_id");
		}
	};

	class Message : public DiscordObject {
	public:
	    class ChannelMention : public DiscordObject {
	    public:
            ChannelMention(rapidjson::Document& json) {
                id = SnowflakeFromString(json["id"].GetString());
                guild_id = SnowflakeFromString(json["id"].GetString());
                type = static_cast<discpp::ChannelType>(json["type"].GetInt());
                name = json["name"].GetString();
            }

            discpp::snowflake guild_id;
            discpp::ChannelType type;
            std::string name;
	    };

		Message() = default;

        /**
         * @brief Constructs a discpp::Message object from an id.
         *
         * ```cpp
         *      discpp::Message message(583251190591258624);
         * ```
         *
         * @param[in] id The id of the message.
         *
         * @return discpp::Message, this is a constructor.
         */
		Message(const snowflake& id);

        /**
		 * @brief Sends a REST request to get the message, use this if the message isn't cached.
		 *
		 * ```cpp
		 *      discpp::Message message(583251190591258624);
		 * ```
		 *
		 * @param[in] message_id The id of the message.
         * @param[in] channel_id The id of the channel.
		 *
		 * @return discpp::Message, this is a constructor.
		 */
		Message(const snowflake& message_id, const snowflake& channel_id);

        /**
         * @brief Constructs a discpp::Message object by parsing json
         *
         * ```cpp
         *      discpp::Message message(json);
         * ```
         *
         * @param[in] json The json that makes up the message.
         *
         * @return discpp::Message, this is a constructor.
         */
		Message(rapidjson::Document& json);


        /**
         * @brief Add a reaction to the message.
         *
         * ```cpp
         *      message.AddReaction(emoji);
         * ```
         *
         * @param[in] emoji The emoji to react with.
         *
         * @return void
         */
		void AddReaction(const discpp::Emoji& emoji);

        /**
         * @brief Remove a bot reaction from the message.
         *
         * ```cpp
         *      message.RemoveBotReaction(emoji);
         * ```
         *
         * @param[in] emoji The emoji to remove a bot reaction of.
         *
         * @return void
         */
		void RemoveBotReaction(const discpp::Emoji& emoji);

        /**
         * @brief Removes a user's reaction from the message.
         *
         * ```cpp
         *      message.RemoveReaction(user, emoji);
         * ```
         *
         * @param[in] user The user's reaction to remove.
         * @param[in] emoji The emoji to remove the reaction to remove.
         *
         * @return void
         */
		void RemoveReaction(const discpp::User& user, const discpp::Emoji& emoji);

        /**
         * @brief Get reactors of a specific emoji.
         *
         * You can use `std::unordered_map::find` to check if a user is contained in it with the users id.
         *
         * ```cpp
         *      std::unordered_map<discpp::snowflake, discpp::User> reactors = message.GetReactorOfEmoji(emoji, 50);
         * ```
         *
         * @param[in] emoji The emoji to get reactors of.
         * @param[in] amount The amount of users to get.
         *
         * @return std::vector<discpp::User>
         */
        std::unordered_map<discpp::snowflake, discpp::User> GetReactorsOfEmoji(const discpp::Emoji& emoji, const int& amount);

        /**
         * @brief Get reactors of a specific emoji of the specific method.
         *
         * You can use `std::unordered_map::find` to check if a user is contained in it with the users id.
         *
         * ```cpp
         *      std::unordered_map<discpp::snowflake, discpp::User> reactors = message.GetReactorOfEmoji(emoji, 50, reaction_method);
         * ```
         *
         * @param[in] emoji The emoji to get reactors of.
         * @param[in] amount The amount of users to get.
         * @param[in] method The method the users reacted by.
         *
         * @return std::vector<discpp::User>
         */
		std::unordered_map<discpp::snowflake, discpp::User> GetReactorsOfEmoji(const discpp::Emoji& emoji, const discpp::User& user, const GetReactionsMethod& method);

        /**
         * @brief Clear message reactions.
         *
         * ```cpp
         *      message.ClearReactions();
         * ```
         *
         * @return void
         */
		void ClearReactions();

        /**
         * @brief Edit the message's text.
         *
         * ```cpp
         *      discpp::Message edited_text_message = message.EditMessage("This is edited text");
         * ```
         *
         * @param[in] text The new message text.
         *
         * @return discpp::Message
         */
		discpp::Message EditMessage(const std::string& text);

        /**
         * @brief Edit the message's embed.
         *
         * ```cpp
         *      discpp::Message edited_embed_message = message.EditMessage(new_embed);
         * ```
         *
         * @param[in] embed The new embed.
         *
         * @return discpp::Message
         */
		discpp::Message EditMessage(discpp::EmbedBuilder& embed);

        /**
         * @brief Edit the message's embed.
         *
         * ```cpp
         *      discpp::Message edited_embed_message = message.EditMessage(1 << 0);
         * ```
         *
         * @param[in] flags The message flags.
         *
         * @return discpp::Message
         */
		discpp::Message EditMessage(const int& flags);

        /**
         * @brief Delete this message.
         *
         * ```cpp
         *      message.DeleteMessage();
         * ```
         *
         * @return void
         */
		void DeleteMessage();

        /**
         * @brief Pin the message to the channel.
         *
         * ```cpp
         *      message.PinMessage();
         * ```
         *
         * @return void
         */
        inline void PinMessage();

        /**
         * @brief Unpin this message.
         *
         * ```cpp
         *      message.UnpinMessage();
         * ```
         *
         * @return void
         */
		inline void UnpinMessage();

        /**
         * @brief Check if this message is tts (text to speech).
         *
         * ```cpp
         *      bool is_tts = message.IsTTS();
         * ```
         *
         * @return bool
         */
        inline bool IsTTS();

        /**
         * @brief Check if this message mentions everyone.
         *
         * ```cpp
         *      bool mentions_everyone = message.MentionsEveryone();
         * ```
         *
         * @return bool
         */
        inline bool MentionsEveryone();

        /**
         * @brief Check if this message is pinned.
         *
         * ```cpp
         *      bool is_pinned = message.IsPinned();
         * ```
         *
         * @return bool
         */
        inline bool IsPinned();

        discpp::Channel channel;
        std::shared_ptr<discpp::Guild> guild;
        std::shared_ptr<discpp::User> author;
		std::string content;
		std::string timestamp; // TODO: Convert to iso8601Time
		std::string edited_timestamp; // TODO: Convert to iso8601Time
		std::unordered_map<discpp::snowflake, discpp::User> mentions;
		std::vector<discpp::snowflake> mentioned_roles;
        std::unordered_map<discpp::snowflake, ChannelMention> mention_channels;
		std::vector<discpp::Attachment> attachments;
		std::vector<discpp::EmbedBuilder> embeds;
		std::vector<discpp::Reaction> reactions;
		snowflake webhook_id;
		int type;
		discpp::MessageActivity activity;
		discpp::MessageApplication application;
		discpp::MessageReference message_reference;
		int flags;
	private:
	    char bit_flags; /**< For internal use only. */
	};
}

#endif