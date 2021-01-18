#include <stdio.h>
#include <cache.h>
#include <errCodes.h>
#include <util.h>
#include <string.h>
#include <libmemcached/memcached.h>

#define cacheConfigFile "../config/cacheConfig.txt"

tErrCode cacheInitialise()
{  
	gCacheCnf.expireTime = ZERO;
	tErrCode err = NO_ERROR;

	err = cacheReadConfig();
	if ( NO_ERROR != err) {

		return err;
	}

	return err;

}

tErrCode thCacheConnect( memcached_st **memc){

	memcached_return rc;
	memcached_server_st *servers;
    tErrCode err = NO_ERROR;

 	*memc = memcached_create(NULL);
	servers = 
		memcached_server_list_append( servers, gCacheCnf.serverIP,
			gCacheCnf.port, &rc);
	
	rc = memcached_server_push(*memc,  servers);
	
	if (MEMCACHED_SUCCESS == rc ){
		RESPONDER_LOG("Added server successfully\n");
    }
	else
	{
		RESPONDER_LOG("Couldn't add server: %s\n",
				memcached_strerror(*memc, rc));
		err = MEMCACHE_CONNECTION_ERROR;
    }
	return err;

}



tErrCode cacheReadConfig()
{

	utilCaptureConfig(cacheConfigFile, cacheAddConfigValue);
	return cacheConfigValidate();

}

void cacheAddConfigValue(char *buffer, int key, int val)
{
	char *bufval = buffer + val;
	char *bufkey = buffer + key;
	int size = utilNoOfChars(bufval);
	if (strcmp(bufkey, SERVER) == 0) {
		gCacheCnf.serverIP = (char *) malloc(size + 1);
		strncpy(gCacheCnf.serverIP, bufval, size + 1);
	} else if (strcmp(bufkey, TIME) == 0) {
		gCacheCnf.expireTime = atoi(bufval);
	} else if (strcmp(bufkey, PORT) == 0) {
		gCacheCnf.port = atoi(bufval);
	}

}

tErrCode cacheConfigValidate()
{
	if( NULL == gCacheCnf.serverIP ){
		gCacheCnf.serverIP = LOCALHOST;
	}
	if( ZERO == gCacheCnf.port ){
		gCacheCnf.port = DEFAULT_MEMCACHE_PORT;
	}
    if (MAX_EXPIRY_TIME < gCacheCnf.expireTime){

			gCacheCnf.expireTime = MAX_EXPIRY_TIME;
			RESPONDER_LOG("MEMCHACHE CONFIG: exiry time out of bound. set to maximum allowed");
	}
	return NO_ERROR;
}
