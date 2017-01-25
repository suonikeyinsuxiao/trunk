#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sql.h"

#define SQLITE3_LOG(fmt, arg...)  \
		printf("[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)

//static SQL_HANDLE_S s_sql_handle;
#if 0
int register_req_callback(void *pUser, int argc, char **argv, char **azColName)
{
	int i;
	SQL_PERSON_S* psPerson = (SQL_PERSON_S*)pUser;
	for(i=0; i<argc; i++)
	{
		SQLITE3_LOG("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (0 == strcmp("USERID", azColName[i]))
		{
			psPerson->m_nAlbumUserId = atoi(argv[i]);
		}
		else if (0 == strcmp("DATAID", azColName[i])) 
		{
			int nDataId = atoi(argv[i]);

			psPerson->m_nAlbumUserDataId = atoi(argv[i]);
		}
		else if (0 == strcmp("NAME", azColName[i]))
		{
			if (NULL != argv[i])
			{
				strcpy(psPerson->m_acName, argv[i]);
				psPerson->m_nNameLen = strlen(psPerson->m_acName);
			}
			else
			{
				memset(psPerson->m_acName, '\0', NAMELEN);
				psPerson->m_nNameLen = 0;
			}
		}
		else if (0 == strcmp("AGE", azColName[i]))
		{
			//reserve
		}
		else if (0 == strcmp("GENDER", azColName[i]))
		{
			//reserve
			//if (NULL == argv[i])
			//{
			//	SQLITE3_LOG("GENDER is NULL");	
			//}
		}


	}
	return 0;
}
#endif

int query_callback(void *pUser, int argc, char **argv, char **azColName)
{
	int i;
	SQL_PERSON_S* psPerson = (SQL_PERSON_S*)pUser;
	for(i=0; i<argc; i++)
	{
		SQLITE3_LOG("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (0 == strcmp("USERID", azColName[i]))
		{
			psPerson->m_nAlbumUserId = atoi(argv[i]);
			//SQLITE3_LOG("psPerson->m_nAlbumUserId = %d\n", psPerson->m_nAlbumUserId);
		}
		else if (0 == strcmp("DATAID", azColName[i])) 
		{
			int nDataId = atoi(argv[i]);
			//SQLITE3_LOG("psPerson->m_nAlbumUserDataId = %d\n", nDataId);

			psPerson->m_nAlbumUserDataId[nDataId] = 1;
		}
		else if (0 == strcmp("NAME", azColName[i]))
		{
			if (NULL != argv[i])
			{
				strcpy(psPerson->m_acName, argv[i]);
				psPerson->m_nNameLen = strlen(psPerson->m_acName);
			}
			else
			{
				memset(psPerson->m_acName, '\0', NAMELEN);
				psPerson->m_nNameLen = 0;
			}
		}
		else if (0 == strcmp("AGE", azColName[i]))
		{
			//reserve
		}
		else if (0 == strcmp("GENDER", azColName[i]))
		{
			//reserve
			//if (NULL == argv[i])
			//{
			//	SQLITE3_LOG("GENDER is NULL");	
			//}
		}
	}
	return 0;
}

//int sql_make_string(void* pHandle,)

void* initSqlHandle()
{
	SQL_HANDLE_S* psSqlHandle = NULL;

	psSqlHandle = (SQL_HANDLE_S*)malloc(sizeof(SQL_HANDLE_S));
	if (NULL == psSqlHandle)
	{
		SQLITE3_LOG("malloc sql handle failed");	
		return NULL;
	}
	memset(psSqlHandle, 0, sizeof(SQL_HANDLE_S));

	psSqlHandle->m_sSqlCmd.m_pcSql = (char*)malloc(SQL_LEN);
	memset(psSqlHandle->m_sSqlCmd.m_pcSql,'\0', SQL_LEN);

	psSqlHandle->m_sPerson.m_nAlbumUserId = -1;
	
	return (void*)psSqlHandle;
}

int destroySqlHandle(void* pHandle)
{
	if (NULL == pHandle)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return 0;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;

	if (NULL != psSqlHandle->m_sSqlCmd.m_pcSql)
	{
		free(psSqlHandle->m_sSqlCmd.m_pcSql);
		psSqlHandle->m_sSqlCmd.m_pcSql = NULL;
	}

	free(psSqlHandle);
	psSqlHandle = NULL;

	return 1;
}

#if 0
int sql_open_db(void* pHandle, char* pcPathDb)
{
	if (NULL == pHandle)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;

	if (NULL != pcPathDb)
	{
		psSqlHandle->m_pcPathDb = pcPathDb;
		nRet = sqlite3_open(pcPathDb, &(psSqlHandle->m_psDb));
		if (nRet != SQLITE_OK)
		{
			SQLITE3_LOG("open database error:%s", sqlite3_errmsg(psSqlHandle->m_psDb));
			return nRet;
		}
	}
	else
	{
		SQLITE3_LOG("database path is %s!", pcPathDb);	
		return SQLITE_ERROR;
	}

	return nRet;
}
#endif

int sql_create_db(void* pHandle, char* pcPathDb)
{
	if (NULL == pHandle)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;

	if (NULL != pcPathDb)
	{
		if (0 == access(pcPathDb, F_OK))
		{
			SQLITE3_LOG("%s is exist!!!", pcPathDb);
			return SQLITE_ERROR;
		}
		else
		{
			psSqlHandle->m_pcPathDb = pcPathDb;
			nRet = sqlite3_open_v2(psSqlHandle->m_pcPathDb, &(psSqlHandle->m_psDb), SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
			if (nRet != SQLITE_OK)
			{
				SQLITE3_LOG("open database error:%s", sqlite3_errmsg(psSqlHandle->m_psDb));
				return nRet;
			}
		}
	}
	else
	{
		SQLITE3_LOG("database path is NULL!");	
		return SQLITE_ERROR;
	}

	return nRet;
}

int sql_open_db_r(void* pHandle, char* pcPathDb)
{
	if (NULL == pHandle || NULL == pcPathDb)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;
	psSqlHandle->m_pcPathDb = pcPathDb;

	nRet = sqlite3_open_v2(psSqlHandle->m_pcPathDb, &(psSqlHandle->m_psDb), SQLITE_OPEN_READONLY, NULL);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(psSqlHandle->m_psDb));
		return nRet;
	}

	return nRet;
}

int sql_open_db_rw(void* pHandle, char* pcPathDb)
{
	if (NULL == pHandle || NULL == pcPathDb)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;
	psSqlHandle->m_pcPathDb = pcPathDb;

	nRet = sqlite3_open_v2(psSqlHandle->m_pcPathDb, &(psSqlHandle->m_psDb), SQLITE_OPEN_READWRITE, NULL);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("open database error:%s\n", sqlite3_errmsg(psSqlHandle->m_psDb));
		return nRet;
	}

	return nRet;
}

int sql_close_db(void* pHandle)
{
	if (NULL == pHandle)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	sqlite3* pDb = (sqlite3*)psSqlHandle->m_psDb;
	//SQLITE3_LOG("pDb=%p", pDb);
	int nRet = SQLITE_ERROR;

	nRet = sqlite3_close_v2(pDb);
	if (nRet != SQLITE_OK)
	{
		SQLITE3_LOG("close database error:%s\n", sqlite3_errmsg(pDb));
		return nRet;
	}

	return nRet;
}

int sql_exec_cmd(void* pHandle)
{
	if (NULL == pHandle)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	sqlite3* pDb = (sqlite3*)psSqlHandle->m_psDb;
	//SQLITE3_LOG("pDb=%p", pDb);
	char* pcErrmsg = NULL;
	char* pcSQL = psSqlHandle->m_sSqlCmd.m_pcSql;
	//SQLITE3_LOG("pcSQL=%p, sql:%s", pcSQL, pcSQL);
	SQLITE3_LOG("sql:%s", pcSQL);
	int nRet = SQLITE_ERROR;

	switch (psSqlHandle->m_sSqlCmd.m_eSqlType)
	{
		case E_SQL_CMD_CREATE_TABLE:
			{
				SQLITE3_LOG("E_SQL_CMD_CREATE_TABLE");
				nRet = sqlite3_exec(pDb, pcSQL, NULL, NULL, &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec %s  error(%d):%s\n", pcSQL, nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			}
			break;
		case E_SQL_CMD_DELETE_TABLE:
			{
				SQLITE3_LOG("E_SQL_CMD_DELETE_TABLE");
				nRet = sqlite3_exec(pDb, pcSQL, NULL, NULL, &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec %s  error(%d):%s\n", pcSQL, nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			}
			break;
		case E_SQL_CMD_INSERT_RECORD:
			{
				SQLITE3_LOG("E_SQL_CMD_INSERT_RECORD");
				nRet = sqlite3_exec(pDb, pcSQL, NULL, NULL, &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec sql  error(%d):%s\n", nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			}
			break;
		case E_SQL_CMD_RM_RECORD:
			{
				SQLITE3_LOG("E_SQL_CMD_RM_RECORD");
				nRet = sqlite3_exec(pDb, pcSQL, NULL, NULL, &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec %s  error(%d):%s\n", pcSQL, nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			}
			break;
		case E_SQL_CMD_QUERY_RECORD:
			{
				SQLITE3_LOG("E_SQL_CMD_QUERY_RECORD");
				nRet = sqlite3_exec(pDb, pcSQL, query_callback, (void*)&(psSqlHandle->m_sPerson), &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec sql  error(%d):%s\n", nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			}
			break;
		case E_SQL_CMD_UPDATE_RECORD:
			{
				SQLITE3_LOG("E_SQL_CMD_UPDATE_RECORD");
				nRet = sqlite3_exec(pDb, pcSQL, NULL, NULL, &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec sql  error(%d):%s\n", nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			}
			break;
#if 0
		case E_SQL_CMD_REGISTER_REQ:
			{
				SQLITE3_LOG("E_SQL_CMD_REGISTER_REQ");
				nRet = sqlite3_exec(pDb, pcSQL, register_req_callback, (void*)&(psSqlHandle->m_sPerson), &pcErrmsg);
				if (nRet != SQLITE_OK)
				{
					SQLITE3_LOG("exec sql  error(%d):%s\n", nRet, pcErrmsg);
					sqlite3_free(pcErrmsg);
					return nRet;
				}
			
			}
			break;
#endif
	}

	return nRet;
}

#if 1
int sql_create_table(void* pHandle, char* pcSql)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;

	psSqlHandle->m_sSqlCmd.m_pcSql = pcSql;
	psSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_CREATE_TABLE;
	nRet = sql_exec_cmd(psSqlHandle);
	memset(pcSql,'\0', SQL_LEN);

	return nRet;
}
#endif


#if 0
int sql_delete_table(void* pHandle, char* pcSql);
#endif

#if 1
int sql_insert_record(void* pHandle, char* pcSql)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;
	psSqlHandle->m_sSqlCmd.m_pcSql = pcSql;
	psSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_INSERT_RECORD;
	nRet = sql_exec_cmd(psSqlHandle);
	memset(pcSql,'\0', SQL_LEN);

	return nRet;
}
#endif

#if 1
int sql_rm_record(void* pHandle, char* pcSql)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;

	psSqlHandle->m_sSqlCmd.m_pcSql = pcSql;
	psSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_RM_RECORD;
	nRet = sql_exec_cmd(psSqlHandle);
	memset(pcSql,'\0', SQL_LEN);

	return nRet;
}
#endif

#if 1
int sql_query_record(void* pHandle, char* pcSql)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;
	int i = 0;

	psSqlHandle->m_sSqlCmd.m_pcSql = pcSql;
	psSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_QUERY_RECORD;
	psSqlHandle->m_sPerson.m_nAlbumUserId = -1;
	for (i = 0; i < DATAID_MAX; ++i)
		psSqlHandle->m_sPerson.m_nAlbumUserDataId[i] = 0;
	nRet = sql_exec_cmd(psSqlHandle);
	memset(pcSql,'\0', SQL_LEN);

	return nRet;
}
#endif

int sql_query_record_all(void* pHandle, char* pcSql)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;

	psSqlHandle->m_sSqlCmd.m_pcSql = pcSql;
	psSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_QUERY_RECORD;
	nRet = sql_exec_cmd(psSqlHandle);
	memset(pcSql,'\0', SQL_LEN);

	return nRet;
}

#if 1
int sql_update_record(void* pHandle, char* pcSql)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return SQLITE_ERROR;
	}

	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;
	int nRet = SQLITE_ERROR;

	psSqlHandle->m_sSqlCmd.m_pcSql = pcSql;
	psSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_UPDATE_RECORD;
	nRet = sql_exec_cmd(psSqlHandle);
	memset(pcSql,'\0', SQL_LEN);

	return nRet;
}
#endif

int sql_get_total_record(void* pHandle, char* pcSql, int* pnRecdCnt)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return 0;
	}

	int nRet = 0;
	int nUserNum = 0;
	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;

	if ( SQLITE_OK != sqlite3_prepare_v2(psSqlHandle->m_psDb, pcSql, strlen(pcSql)+1, &psSqlHandle->m_psStmt, NULL))
	{
		SQLITE3_LOG("sqlite3_prepare_v2 failed");
		sqlite3_finalize(psSqlHandle->m_psStmt);
	}

	while(sqlite3_step(psSqlHandle->m_psStmt) == SQLITE_ROW)
		++nUserNum;

	*pnRecdCnt = nUserNum;
	nRet = 1;

	sqlite3_finalize(psSqlHandle->m_psStmt);

	return nRet;
}

int sql_get_user_num(void* pHandle, char* pcSql, int* pnUserNum)
{
	return sql_get_total_record(pHandle, pcSql, pnUserNum);
}

#if 0
int sql_get_user_num(void* pHandle, char* pcSql, int* pnUserNum)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return 0;
	}

	int nRet = 0;
	int nUserId = -1;
	int nSavedUserId = -1;
	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;

	if ( SQLITE_OK != sqlite3_prepare_v2(psSqlHandle->m_psDb, pcSql, strlen(pcSql)+1, &psSqlHandle->m_psStmt, NULL))
	{
		SQLITE3_LOG("sqlite3_prepare_v2 failed");
		sqlite3_finalize(psSqlHandle->m_psStmt);
	}

	while(sqlite3_step(psSqlHandle->m_psStmt) == SQLITE_ROW)
	{
		nUserId = sqlite3_column_int(psSqlHandle->m_psStmt, 0);
		if (nUserId != nSavedUserId)
		{
			nSavedUserId = nUserId;	
			++(*pnUserNum);
		}
	}

	nRet = 1;

	sqlite3_finalize(psSqlHandle->m_psStmt);

	return nRet;
}
#endif

#if 1
int sql_get_users_id(void* pHandle, char* pcSql, int* pnUsersId, int nUsrNum)
{
	if (NULL == pHandle || NULL == pcSql)
	{
		SQLITE3_LOG("input handle is NULL!");	
		return 0;
	}

	int nRet = 0;
	int nUserId = -1;
	int i = 0;
	SQL_HANDLE_S* psSqlHandle = (SQL_HANDLE_S*)pHandle;

	if ( SQLITE_OK != sqlite3_prepare_v2(psSqlHandle->m_psDb, pcSql, strlen(pcSql)+1, &psSqlHandle->m_psStmt, NULL))
	{
		SQLITE3_LOG("sqlite3_prepare_v2 failed");
		sqlite3_finalize(psSqlHandle->m_psStmt);
	}

	while((sqlite3_step(psSqlHandle->m_psStmt) == SQLITE_ROW) && (i < nUsrNum))
	{
		nUserId = sqlite3_column_int(psSqlHandle->m_psStmt, 0);
		pnUsersId[i] = nUserId;
		//SQLITE3_LOG("xxxxuserid =%d", nUserId);
		i++;
	}

	nRet = 1;

	sqlite3_finalize(psSqlHandle->m_psStmt);

	return nRet;
}
#endif
