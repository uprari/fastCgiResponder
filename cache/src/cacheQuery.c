#include <stdio.h>
#include <cache.h>
#include <cacheQuery.h>
#include <errCodes.h>
#include <cache.h>
#include <cacheQuery.h>
#include <libmemcached/memcached.h>

tErrCode cacheSetValue(char *key, char *value)
{

	memcached_return rc;
	rc = memcached_set(gCacheCtx.memc, key, strlen(key), value,
			strlen(value), (time_t) gCacheCnf.expireTime,
			(uint32_t) 0);

	if (rc == MEMCACHED_SUCCESS)
		printf("Key stored successfully\n");
	else
		printf("Couldn't store key: %s\n",
				memcached_strerror(gCacheCtx.memc, rc));
	return NO_ERROR;
}


tErrCode cacheGetValue(char *key , char** value)
{
	long unsigned int len = 0;
	unsigned int flag = 0;
	memcached_return rc;

	*value = memcached_get(gCacheCtx.memc, key, strlen(key), &len, &flag,
			&rc);

	if (rc == MEMCACHED_SUCCESS)
		printf("Key retrieved successfully\n");
	else
		printf("Could not retrieve key: %s\n",
				memcached_strerror(gCacheCtx.memc, rc));
	return NO_ERROR;
}
