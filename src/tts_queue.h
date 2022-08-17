#pragma once
#include <pthread.h>

#define QUEUE_SIZE 180

typedef struct discord_msg_t {
    char *message;
    long sender_id;
    long message_id;
} discord_msg_t;

typedef struct guild_tts_queue_t {
    pthread_mutex_t lock;
    int front_ptr, back_ptr;
    discord_msg_t queue[QUEUE_SIZE];
} guild_tts_queue_t;

