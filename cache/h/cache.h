#ifndef CACHE_H
#define CACHE_H

#include <libmemcached/memcached.h>
#include <errCodes.h>

typedef struct globalCacheConfig {

    int threadCount;
    unsigned int expireTime;
    char *serverIP;
    int port;
} gCacheConf;

typedef struct globalCacheContex {

    memcached_server_st *servers;
    memcached_st *memc;

} gCacheContex;

gCacheConf gCacheCnf;
gCacheContex gCacheCtx;

tErrCode cacheInitialise();
tErrCode cacheReadConfig();
void cacheAddConfigValue(char *buffer, int key, int val);
tErrCode cacheConfigValidate();
tErrCode thCacheConnect( memcached_st **memc);

#endif
