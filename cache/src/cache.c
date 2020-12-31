#include <stdio.h>
#include <cache.h>
#include <errCodes.h>
#include <util.h>
#include <string.h>
#include <libmemcached/memcached.h>

#define cacheConfigFile "../config/cacheConfig.txt"

tErrCode cacheInitialise()
{
    //i,ntialising global variables
    gCacheCnf.threadCount = 0;
    gCacheCnf.expireTime = 0;
    tErrCode err = NO_ERROR;

    cacheReadConfig();
    if ((err = cacheConfigValidate()) != NO_ERROR) {

	return err;
    }

    gCacheCtx.servers = NULL;
    memcached_return rc;
    gCacheCtx.memc = memcached_create(NULL);

    gCacheCtx.servers =
	memcached_server_list_append(gCacheCtx.servers, gCacheCnf.serverIP,
				     gCacheCnf.port, &rc);
    rc = memcached_server_push(gCacheCtx.memc, gCacheCtx.servers);

    if (rc == MEMCACHED_SUCCESS)
	printf("Added server successfully\n");
    else
	printf("Couldn't add server: %s\n",
	       memcached_strerror(gCacheCtx.memc, rc));

    return 0;

}

tErrCode cacheReadConfig()
{

    utilCaptureConfig(cacheConfigFile, cacheAddConfigValue);
    return NO_ERROR;

}

void cacheAddConfigValue(char *buffer, int key, int val)
{
    char *bufval = buffer + val;
    char *bufkey = buffer + key;
    int size = utilNoOfChars(bufval);
    if (strcmp(bufkey, "thread") == 0) {
	gCacheCnf.threadCount = atoi(bufval);
    } else if (strcmp(bufkey, "server") == 0) {
	gCacheCnf.serverIP = (char *) malloc(size + 1);
	strncpy(gCacheCnf.serverIP, bufval, size + 1);
    } else if (strcmp(bufkey, "time") == 0) {
	gCacheCnf.expireTime = atoi(bufval);
    } else if (strcmp(bufkey, "port") == 0) {
	gCacheCnf.port = atoi(bufval);
    }

}

tErrCode cacheConfigValidate()
{
    return NO_ERROR;

}
