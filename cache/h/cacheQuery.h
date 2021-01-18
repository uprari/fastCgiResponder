#ifndef CACHE_QUERY_H
#define CACHE_QUERY_H

#include<errCodes.h>

tErrCode cacheSetValue(char *key, char *value);
tErrCode cacheGetValue(char *key, char **value);
tErrCode thCacheSetValue( memcached_st *memc,char *key, char *value);
tErrCode thCacheGetValue( memcached_st *memc,char *key , char **value);
void cacheClean( memcached_st *memc);

#endif

