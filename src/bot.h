#pragma once
#include "./tts_queue.h"
#include <pthread.h>

#define MAX_VCS_AT_ONCE 100

typedef struct dc_bot_data_t {
    pthread_mutex_t lock;
    int length;
    guild_tts_queue_t guild_queues[MAX_VCS_AT_ONCE];
} dc_bot_data_t;

void init_dc_bot_data(dc_bot_data_t *bot);
void free_dc_bot_data(dc_bot_data_t *bot);

int dc_bot_dat_add_guild_tts_queue(dc_bot_data_t *bot, guild_tts_queue_t queue);
int dc_bot_dat_get_guild_tts_queue(dc_bot_data_t *bot, guild_tts_queue_t *ret, long guild_id, long channel_id);
void dc_bot_dat_remove_guild_tts_queue(dc_bot_data_t *bot, long guild_id, long channel_id);

