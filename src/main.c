#include <stdlib.h>
#include <stdio.h>
#include <orca/discord.h>
#include <string.h>
#include "../testing.h/logger.h"

static void on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);
    lprintf(LOG_INFO, "Logged in as %s\n", bot->username);
}

#define MSG_EQ(str) (strcmp(msg->content, str) == 0)

static void on_message(struct discord *client, const struct discord_message *msg)
{
    if (MSG_EQ("!help") || MSG_EQ("?help") || MSG_EQ(".help")) {
        discord_async_next(client, NULL);
        struct discord_create_message_params params = { .content = "Jim'll fix it" };
        discord_create_message(client, msg->channel_id, &params, NULL);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        lprintf(LOG_ERROR, "You must execute this program as %s <token>\n", argv[0]);
        return 1;
    }

    lprintf(LOG_INFO, "Starting %s verison %s\n", PROJECT_NAME, VERSION);

    struct discord *client = discord_init(argv[1]);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message);
    discord_run(client);
    return 1;
}
