#pragma once
#include <orca/discord.h>
#include <pthread.h>

#define QUEUE_SIZE 180

typedef struct discord_msg_t {
    char *message;
    long sender_id;
    long message_id;
} discord_msg_t;

typedef struct guild_tts_queue_t {
    long guild_id;
    long channel_id;
    long vc_id;
    pthread_mutex_t lock;
    int front_ptr, back_ptr;
    discord_msg_t queue[QUEUE_SIZE];
} guild_tts_queue_t;

void init_guild_tts_queue(guild_tts_queue_t *queue, long guild_id, long channel_id, long vc_id);
void free_guild_tts_queue(guild_tts_queue_t *queue);

int init_discord_msg_t(discord_msg_t *msg, const struct discord_message *d_msg);
void free_discord_msg_t(discord_msg_t *msg);

int remove_guild_tts_queue(guild_tts_queue_t *queue, discord_msg_t *ret);
int add_guild_tts_queue(guild_tts_queue_t *queue, discord_msg_t item);

