#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <orca/discord.h>
#include "./tts_queue.h"
#include "./bot.h"
#include "../testing.h/logger.h"

static dc_bot_data_t bot_data;

static void on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);
    lprintf(LOG_INFO, "Logged in as %s (<@%ld>)\n", bot->username, bot->id);
}

#define TTS "tts"
#define TTS_LEAVE "tts-leave"
#define MSG_EQ(str) (strcmp(msg->content, str) == 0)

static int send_message(struct discord *client, const struct discord_message *msg, const char *message)
{
    struct discord_create_message_params params = {
        .content = message,
    };
    if (discord_create_message(client, msg->channel_id, &params, NULL) != ORCA_OK) {
        lprintf(LOG_ERROR, "Cannot send message in <#%ld> (%ld)\n", msg->channel_id, msg->guild_id);
        return 1;
    }
    return 0;
}

static void on_message(struct discord *client, const struct discord_message *msg)
{
    if (msg->author->bot) {
        return;
    }

#ifdef DEBUG
    lprintf(LOG_INFO, "<@%ld> says " ANSI_RED
            "%s" ANSI_RESET
            " in <#%ld> (%ld)\n",
            msg->author->id,
            msg->content,
            msg->channel_id,
            msg->guild_id);
#endif

    if (MSG_EQ("tts-help")) {
        send_message(client, msg, "```md\n"
                     " - tts, will make me join vc and, speak every message in your tc to vc.\n"
                     " - tts-leave, willl make me leave vc and, stop chatting shit.\n"
                     "```\n");
    } else if (MSG_EQ(TTS) || MSG_EQ(TTS_LEAVE)) {
        // Get VC
        struct discord_channel **channels;
        ORCAcode code = discord_get_guild_channels(client, msg->guild_id, &channels);
        if (code != ORCA_OK || channels == NULL) {
            lprintf(LOG_ERROR, "Cannot get channels in (%ld)\n", msg->guild_id);
            send_message(client, msg, "I cannot view channels\n");
            return;
        }

        long vc_id = 0;
        for (int i = 0; channels[i] != NULL; i++) {
            struct discord_channel *channel = channels[i];
            if (channel->type == DISCORD_CHANNEL_GUILD_VOICE) {
                vc_id = channel->id;
                break;
            }
        }

        discord_channel_list_free(channels);
        if (vc_id == 0) {
            send_message(client, msg, "There are no vcs to join\n");
            return;
        }

        // Exec VC commands
        char message_buffer[1024];
        if (MSG_EQ(TTS)) {
            snprintf(message_buffer,
                     sizeof(message_buffer),
                     "Joining <#%ld> for some TTS fun, invited by <@%ld> (<t:%ld:R>)\n",
                     vc_id,
                     msg->author->id,
                     time(NULL));
            send_message(client, msg, message_buffer);

            discord_voice_join(client, msg->guild_id, vc_id, false, false);
        } else if (MSG_EQ(TTS_LEAVE)) {
            snprintf(message_buffer,
                     sizeof(message_buffer),
                     "Leaving <#%ld>, kicked by <@%ld> (<t:%ld:R>)\n",
                     vc_id,
                     msg->author->id,
                     time(NULL));
            send_message(client, msg, message_buffer);

            const struct discord_user *bot = discord_get_self(client);
            discord_disconnect_guild_member(client, msg->guild_id, bot->id, NULL);
        }
    }
}

static void on_channel_delete(struct discord *discord, const struct discord_channel *channel)
{
    // TODO: Delete internal structs if you are part of the VC
    lprintf(LOG_ERROR, "TODO!\n");
}
// TODO: Delete internal structs if you are kicked from VC

int main(int argc, char **argv)
{
    if (argc != 2) {
        lprintf(LOG_ERROR, "You must execute this program as %s <token>\n", argv[0]);
        return 1;
    }

    lprintf(LOG_INFO, "Starting %s verison %s\n", PROJECT_NAME, VERSION);
    init_dc_bot_data(&bot_data);

    struct discord *client = discord_init(argv[1]);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message);
    discord_set_on_channel_delete(client, &on_channel_delete);
    discord_run(client);
    discord_cleanup(client);

    lprintf(LOG_ERROR, "Exiting the application, please restart!\n");
    free_dc_bot_data(&bot_data);
    return 1;
}
