#include <csapp.h>
#include "map.h"

static int hash(char* key) {
    char* ptr = key;
    int rst = 0;
    while (*ptr != '\0') {
        rst += *ptr & MAP_SIZE;
        ptr++;
    }
    return rst & MAP_SIZE;
}

static Pair* get_pair(Map* map, char* key) {
    Pair* list = map->pairs[hash(key)];
    while (list != NULL) {
        if (!strcmp(list->key, key)) return list;
        list = list->next;
    }
    return NULL;
}

void map_init(Map* map) {
    map->pairs = Malloc(MAP_SIZE * sizeof(Pair*));
    int i;
    for (i = 0; i < MAP_SIZE; i++) map->pairs[i] = NULL;
    map->pair_cnt = 0;
    map->size = 0;
}

void map_put(Map* map, char* key, char* value) {
    Pair* pair = get_pair(map, key);
    int value_len = strlen(value);
    if (value_len > MAX_OBJECT_SIZE) {
        fprintf(stderr, "object too large\n");
        return;
    }
    if (pair != NULL) {
        int gap = value_len - pair->buffer_size;
        if (gap + map->size > MAX_CACHE_SIZE) {
            fprintf(stderr, "cache is not enough\n");
            return;
        }
        Free(pair->value);
        pair->value = Malloc((value_len + 1)* sizeof(char));
        memcpy(pair->value, value, value_len);
        pair->buffer_size = value_len;
        map->size += gap; 
        return;
    }

    if (value_len + map->size > MAX_CACHE_SIZE) {
        fprintf(stderr, "cache is not enough\n");
        return;
    }
    
    int key_len = strlen(key);
    char* key_cpy = Malloc((key_len + 1) * sizeof(char));
    char* value_cpy = Malloc((value_len + 1) * sizeof(char));
    memset(key_cpy, 0, key_len + 1);
    memcpy(key_cpy, key, key_len);
    memset(value_cpy, 0, value_len + 1);
    memcpy(value_cpy, value, value_len);
    pair = (Pair*)Malloc(1 * sizeof(Pair));
    pair->buffer_size = value_len;
    pair->key = key_cpy;
    pair->value = value_cpy;
    int hash_code = hash(pair->key);
    pair->next = map->pairs[hash_code];
    map->pairs[hash_code] = pair;
    map->pair_cnt++;
    map->size += value_len;
}

char* map_get(Map* map, char* key) {
    Pair* pair = get_pair(map, key);
    if (pair != NULL) return pair->value;
    return NULL;
}

void map_deinit(Map* map) {
    int i;
    for (i = 0; i < MAP_SIZE; i++) {
        Pair* list = map->pairs[i];
        while (list != NULL) {
            Pair* next = list->next;
            Free(list->key);
            Free(list->value);
            Free(list);
            list= next;
        }
    }
    Free(map->pairs);
}