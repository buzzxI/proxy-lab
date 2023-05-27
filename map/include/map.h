#define MAP_SIZE 16
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

typedef struct Pair{
    char* key;
    char* value;
    int buffer_size;
    struct Pair* next;
} Pair;

typedef struct {
    Pair** pairs;
    int pair_cnt;
    int size;
} Map;

void map_init(Map* map);

void map_put(Map* map, char* key, char* value);

char* map_get(Map* map, char* key);

void map_deinit(Map* map);



