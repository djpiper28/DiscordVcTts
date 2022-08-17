#include "./bot.h"
#include "./utils.h"
#include "../testing.h/logger.h"

void init_dc_bot_data(dc_bot_data_t *bot)
{
    dc_bot_data_t tmp = {PTHREAD_MUTEX_INITIALIZER, 0};
    *bot = tmp;
}

void free_dc_bot_data(dc_bot_data_t *bot)
{
    pthread_mutex_destroy(&bot->lock);
}

int dc_bot_dat_add_guild_tts_queue(dc_bot_data_t *bot, guild_tts_queue_t queue)
{
    int status = 0;
    pthread_mutex_lock(&bot->lock);
    if (bot->length == ARR_SIZE(bot->guild_queues)) {
        lprintf(LOG_ERROR, "Maximum instance count has been reached\n");
        goto cleanup;
    }

    bot->guild_queues[bot->length] = queue;
    bot->length++;

    status = 1;
cleanup:
    pthread_mutex_unlock(&bot->lock);
    return status;
}

int dc_bot_dat_get_guild_tts_queue(dc_bot_data_t *bot, guild_tts_queue_t *ret, long guild_id, long channel_id)
{
    int status = 0;
    pthread_mutex_lock(&bot->lock);
    for (int i = 0; i < bot->length; i++) {
        guild_tts_queue_t q = bot->guild_queues[i];
        if (q.guild_id = guild_id || q.channel_id == channel_id) {
            *ret = q;
            status = 1;
            goto cleanup;
        }
    }
cleanup:
    pthread_mutex_unlock(&bot->lock);
    return status;
}

void dc_bot_dat_remove_guild_tts_queue(dc_bot_data_t *bot, long guild_id, long channel_id)
{
    int status = 0;
    pthread_mutex_lock(&bot->lock);
    for (int i = 0; i < bot->length; i++) {
        guild_tts_queue_t q = bot->guild_queues[i];
        if (q.guild_id = guild_id || q.channel_id == channel_id) {
            status = 1;
            free_guild_tts_queue(&q);
            bot->length--;
            goto cleanup;
        }
    }
cleanup:
    pthread_mutex_unlock(&bot->lock);
    return status;
}
