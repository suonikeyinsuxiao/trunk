#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sql.h"

#define SQLITE3_LOG(fmt, arg...)  \
		printf("[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)

#define PATH_DATABASE ("./test.db")

int main(int argc, char** argv)
{
	char* pcSQL = NULL;
	char* pcErrmsg = NULL;
	int nRet = SQLITE_ERROR;
	SQL_HANDLE_S* pSqlHandle = (SQL_HANDLE_S*)initSqlHandle();

	pcSQL = (char*)malloc(SQL_LEN);
	memset(pcSQL,'\0', 256);

	if (0 == access(PATH_DATABASE, F_OK))
	{
		if (SQLITE_OK != sql_open_db_rw(pSqlHandle, PATH_DATABASE))	
		{
			SQLITE3_LOG("open database failed");
			destroySqlHandle(pSqlHandle);
			return -1;
		}
		SQLITE3_LOG("open database success!");	
	}
	else
	{
		if (SQLITE_OK != sql_create_db(pSqlHandle, PATH_DATABASE))
		{
			SQLITE3_LOG("create database failed");
			destroySqlHandle(pSqlHandle);
			return -1;
		}
		SQLITE3_LOG("create database success!");	
		#if 1
			/* create SQL statement */
			sprintf(pcSQL, "CREATE TABLE ALBUM(USERID INT NOT NULL,DATAID INT NOT NULL,NAME TEXT NOT NULL,AGE INT,GENDER TEXT);");
			//pcSQL = "CREATE TABLE ALBUM("  \
			//	   "USERID         INT     NOT NULL," \
			//	   "DATAID         INT     NOT NULL," \
			//	   "NAME           TEXT    NOT NULL," \
			//	   "AGE            INT     " \
			//	   "GENDER         CHAR(10))";
		
			pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
			pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_CREATE_TABLE;
			sql_exec_cmd(pSqlHandle);
			memset(pcSQL,'\0', 256);
		#endif

		#if 1
			/* 插入一条记录*/
			sprintf(pcSQL, "INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (0, 0, 'frank');");
			//pcSQL ="INSERT INTO ALBUM (USERID,DATAID,NAME) "  \
			//	   "VALUES (0, 0, 'frank'); " \
			//	   "INSERT INTO ALBUM (USERID,DATAID,NAME) "  \
			//	   "VALUES (0, 1, 'frank'); "     \
			//	   "INSERT INTO ALBUM (USERID,DATAID,NAME)" \
			//	   "VALUES (0, 2, 'frank');" \
			//	   "INSERT INTO ALBUM (USERID,DATAID,NAME)" \
			//	   "VALUES (0, 3, 'frank');";
			pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
			pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_INSERT_RECORD;
			sql_exec_cmd(pSqlHandle);
			memset(pcSQL,'\0', 256);
		#endif
	}

#if 0
	//插入记录
	sprintf(pcSQL, "INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (1, 0, 'zhangsan');INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (2, 0, 'lisi');INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (3, 0, 'wangwu');INSERT INTO ALBUM (USERID,DATAID,NAME) VALUES (4, 0, 'zhaoliu');");
	pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
	pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_INSERT_RECORD;
	sql_exec_cmd(pSqlHandle);
	memset(pcSQL,'\0', 256);
#endif

#if 0
	//查询记录
	sprintf(pcSQL, "SELECT * FROM ALBUM WHERE USERID=%d AND DATAID=%d;", 0, 2);
	pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
	pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_QUERY_RECORD;
	pSqlHandle->m_sPerson.m_nAlbumUserId = -1;
	pSqlHandle->m_sPerson.m_nAlbumUserDataId = -1;
	sql_exec_cmd(pSqlHandle);
	memset(pcSQL,'\0', 256);

	if ((-1 != pSqlHandle->m_sPerson.m_nAlbumUserId) && (-1 != pSqlHandle->m_sPerson.m_nAlbumUserDataId))
		SQLITE3_LOG("userid=%d, dataid=%d, name=%s",pSqlHandle->m_sPerson.m_nAlbumUserId, pSqlHandle->m_sPerson.m_nAlbumUserDataId, pSqlHandle->m_sPerson.m_acName);
	else
		SQLITE3_LOG("userid=%d, dataid=%d, is not exist", 0, 2);
#endif

#if 0
	//删除指定的userid、dataid
	sprintf(pcSQL, "DELETE FROM ALBUM WHERE USERID=%d AND DATAID=%d;", 4, 0);
	pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
	pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_RM_RECORD;
	sql_exec_cmd(pSqlHandle);
	memset(pcSQL,'\0', 256);
#endif

#if 0
	//插入记录
	pcSQL ="INSERT INTO ALBUM (USERID,DATAID,NAME) "  \
		   "VALUES (1, 0, 'zhangsan'); ";
	pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
	pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_INSERT_RECORD;
	sql_exec_cmd(pSqlHandle);
#endif

#if 1
	//修改记录
	sprintf(pcSQL, "UPDATE ALBUM SET NAME='FJ' WHERE USERID=0 AND DATAID=0");
	pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
	pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_UPDATE_RECORD;
	sql_exec_cmd(pSqlHandle);
	memset(pcSQL,'\0', 256);
#endif

#if 1
	//查询记录
	sprintf(pcSQL, "SELECT * FROM ALBUM ORDER BY USERID;");
	//pcSQL = "SELECT * FROM ALBUM ORDER BY USERID";
	pSqlHandle->m_sSqlCmd.m_pcSql = pcSQL;
	pSqlHandle->m_sSqlCmd.m_eSqlType = E_SQL_CMD_QUERY_RECORD;
	sql_exec_cmd(pSqlHandle);
	memset(pcSQL,'\0', 256);
#endif

	//SQLITE3_LOG("%s", sqlite3_db_filename(pSqlHandle->m_psDb, PATH_DATABASE));


	if (SQLITE_OK != sql_close_db(pSqlHandle))
	{
		SQLITE3_LOG("close database failed");
		destroySqlHandle(pSqlHandle);
		return -1;
	}

	if (NULL != pcSQL)
		free(pcSQL);

	destroySqlHandle(pSqlHandle);

	return 0;
}
