#include "./tts_queue.h"
#include "./utils.h"
#include <stdlib.h>

void init_guild_tts_queue(guild_tts_queue_t *queue, long guild_id, long channel_id)
{
    guild_tts_queue_t tmp_queue = {guild_id, channel_id, PTHREAD_MUTEX_INITIALIZER, -1, 0};
    *queue = tmp_queue;
}

void free_guild_tts_queue(guild_tts_queue_t *queue)
{
    pthread_mutex_destroy(&queue->lock);
}

int init_discord_msg_t(discord_msg_t *msg, const struct discord_message *d_msg)
{
    char *message = clone_string(d_msg->content);
    if (message == NULL) {
        msg->message = NULL;
        return 0;
    }

    msg->message = message;
    msg->sender_id = d_msg->author->id;
    msg->message_id = d_msg->id;
    return 1;
}

void free_discord_msg_t(discord_msg_t *msg)
{
    if (msg->message != NULL) {
        free(msg->message);
    }
}

int remove_guild_tts_queue(guild_tts_queue_t *queue, discord_msg_t *ret)
{
    int status = 0;
    pthread_mutex_lock(&queue->lock);
    if (queue->front_ptr == -1 || queue->front_ptr == queue->back_ptr) {
        goto cleanup;
    }

    *ret = queue->queue[queue->front_ptr];
    queue->front_ptr = (queue->front_ptr + 1) % ARR_SIZE(queue->queue);

    status = 1;
cleanup:
    pthread_mutex_unlock(&queue->lock);
    return status;
}

int add_guild_tts_queue(guild_tts_queue_t *queue, discord_msg_t item)
{
    int status = 0;
    pthread_mutex_lock(&queue->lock);
    if (queue->front_ptr == queue->back_ptr) {
        goto cleanup;
    }

    if (queue->front_ptr == -1) {
        queue->front_ptr = 0;
    }

    queue->back_ptr = (queue->back_ptr + 1) % ARR_SIZE(queue->queue);
    queue->queue[queue->back_ptr] = item;

    status = 1;
cleanup:
    pthread_mutex_unlock(&queue->lock);
    return status;
}

