#ifndef CACHE_H
#define CACHE_H

#include <libmemcached/memcached.h>
#include <errCodes.h>
#define ZERO 0
#define SERVER "server"
#define TIME "time"
#define PORT "port"
#define DEFAULT_MEMCACHE_PORT 11211
#define MAX_EXPIRY_TIME 8640
#define LOCALHOST "localhost"

typedef struct globalCacheConfig {

    int threadCount;
    unsigned int expireTime;
    char *serverIP;
    int port;
} gCacheConf;

gCacheConf gCacheCnf;

tErrCode cacheInitialise();
tErrCode cacheReadConfig();
void cacheAddConfigValue(char *buffer, int key, int val);
tErrCode cacheConfigValidate();
tErrCode thCacheConnect( memcached_st **memc);

#endif
