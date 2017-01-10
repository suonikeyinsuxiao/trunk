#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"


#define PATH_DATABASE ("./test.db")


#define SQLITE3_LOG(fmt, arg...)  \
		printf("[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)

/* test for sqlite3_open*/
#if 1
int main(int argc, char** argv)
{
	sqlite3* pDb = NULL;
	int nRet = SQLITE_ERROR;

	nRet = sqlite3_open(PATH_DATABASE, &pDb);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(pDb));
		return -1;
	}
	SQLITE3_LOG("open database success\n");

	sqlite3_close(pDb);
	return 0;
}
#endif

/* test for sqlite3_open_v2*/
#if 0
/* SQLITE_OPEN_READONLY*/
int main(int argc, char** argv)
{
	sqlite3* pDb = NULL;
	int nRet = SQLITE_ERROR;

	nRet = sqlite3_open_v2(PATH_DATABASE, &pDb, SQLITE_OPEN_READONLY, NULL);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(pDb));
		return -1;
	}
	SQLITE3_LOG("open database success\n");

	sqlite3_close_v2(pDb);
	return 0;
}
#endif

#if 0
/* SQLITE_OPEN_READWRITE*/
int main(int argc, char** argv)
{
	sqlite3* pDb = NULL;
	int nRet = SQLITE_ERROR;

	nRet = sqlite3_open_v2(PATH_DATABASE, &pDb, SQLITE_OPEN_READWRITE, NULL);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(pDb));
		return -1;
	}
	SQLITE3_LOG("open database success\n");

	sqlite3_close_v2(pDb);
	return 0;
}
#endif

#if 0
/* SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE == sqlite3_open()*/
int main(int argc, char** argv)
{
	sqlite3* pDb = NULL;
	int nRet = SQLITE_ERROR;

	nRet = sqlite3_open_v2(PATH_DATABASE, &pDb, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(pDb));
		return -1;
	}
	SQLITE3_LOG("open database success\n");

	sqlite3_close_v2(pDb);
	return 0;
}
#endif

#if 0
/* SQLITE_OPEN_READONLY|SQLITE_OPEN_CREATE */
int main(int argc, char** argv)
{
	sqlite3* pDb = NULL;
	int nRet = SQLITE_ERROR;

	nRet = sqlite3_open_v2(PATH_DATABASE, &pDb, SQLITE_OPEN_READONLY|SQLITE_OPEN_CREATE, NULL);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(pDb));//out of memory
		return -1;
	}
	SQLITE3_LOG("open database success\n");

	sqlite3_close_v2(pDb);
	return 0;
}
#endif
