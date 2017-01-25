#include "sqlite3.h"

#define SQL_CMD_BASE 					(0x1000)
#define NAMELEN                         (128)
#define USERID_MAX                      (5)
#define DATAID_MAX                      (10)
#define SQL_LEN							(256)

/**
 * @brief    SQL command enum
 */
typedef enum
{
	E_SQL_CMD_CREATE_TABLE = SQL_CMD_BASE,		/* create table */
	E_SQL_CMD_DELETE_TABLE,						/* delete table */
	E_SQL_CMD_INSERT_RECORD,					/* insert record */
	E_SQL_CMD_RM_RECORD,						/* delete record */
	E_SQL_CMD_QUERY_RECORD,						/* query  record */
	E_SQL_CMD_UPDATE_RECORD,					/* update record */
	E_SQL_CMD_REGISTER_REQ
}SQL_CMD_E;

/**
 * @brief    SQL command structure 
 */
typedef struct _sql_cmd_s
{
	SQL_CMD_E 	m_eSqlType;						/* SQL command type */
	char*		m_pcSql;						/* SQL statement */
}SQL_CMD_S;

/**
 * @brief	SQL_PERSON_S  store person informaton 
 *
 * 			m_nAlbumUserId==-1 is free
 * 			m_nAlbumUserDataId存放对应userID下的模板,
 * 			m_nAlbumUserDataId[0]=1 表示有存模板,m_nAlbumUserDataId[0]=0 表示空闲
 * 			m_nAlbumUserDataId[1]=1 表示有存模板,m_nAlbumUserDataId[1]=0 表示空闲
 * 			......
 * 			m_nAlbumUserDataId[9]=1 表示有存模板,m_nAlbumUserDataId[1]=0 表示空闲
 */
typedef struct _sql_person_s
{
	int                 m_nAlbumUserId;							/* userid */
	int                 m_nAlbumUserDataId[DATAID_MAX];			/* dataid */
	unsigned char       m_acName[NAMELEN];						/* name   */
	int                 m_nNameLen;								/* name length */
}SQL_PERSON_S;

/**
 * @brief	SQL handle 
 */
typedef struct _sql_handle_s
{
	sqlite3* 		m_psDb;							/* sqlite3 Database handle */
	sqlite3_stmt* 	m_psStmt;						/* Statement handle */
	char* 			m_pcPathDb;						/* path of database file */
	SQL_CMD_S		m_sSqlCmd;						/* SQL_CMD_S */
	SQL_PERSON_S	m_sPerson;						/* SQL_PERSON_S */
}SQL_HANDLE_S;

/**
 * @brief    sqlite_callback 
 *
 * @param[in]    pv: user data
 * @param[in]    argc: the number of columns in the result
 * @param[in]    argv: an array of pointers to strings obtained as if from sqlite3_column_text(), one for each column. If an element of a result row is NULL then the corresponding string pointer for the sqlite3_exec() callback is a NULL pointer.
 * @param[in]    azColName: an array of pointers to strings where each entry represents the name of corresponding result column as obtained from sqlite3_column_name().
 *
 * @return   If an sqlite3_exec() callback returns non-zero, the sqlite3_exec() routine returns SQLITE_ABORT without invoking the callback again and without running any subsequent SQL statements. 
 */
int sqlite_callback(void *pv, int argc, char **argv, char **azColName);

/**
 * @brief    initSqlHandle 
 *
 * @return   sql handle success, otherwise NULL 
 */
void* initSqlHandle();

/**
 * @brief    destroySqlHandle 
 *
 * @param[in]    pHandle: sql handle
 *
 * @return   1 success, 0 fail 
 */
int destroySqlHandle(void* pHandle);

/**
 * @brief    sql_create_db 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcPathDb: path of database
 *
 * @return  	If the database is created successfully, then SQLITE_OK is returned. 
 * 				Otherwise an error code is returned. 
 * 				The sqlite3_errmsg() or sqlite3_errmsg16() routines can be used to obtain an English language description of the error following a failure of any of the sqlite3_open() routines. 
 */
int sql_create_db(void* pHandle, char* pcPathDb);

/**
 * @brief    sql_open_db_r 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcPathDb: path of database
 *
 * @return   	If the database is opened successfully, then SQLITE_OK is returned. 
 * 				Otherwise an error code is returned. 
 * 				The sqlite3_errmsg() or sqlite3_errmsg16() routines can be used to obtain an English language description of the error following a failure of any of the sqlite3_open() routines. 
 */
int sql_open_db_r(void* pHandle, char* pcPathDb);

/**
 * @brief    sql_open_db_rw 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcPathDb: path of database
 *
 * @return    	If the database is opened successfully, then SQLITE_OK is returned. 
 * 				Otherwise an error code is returned. 
 * 				The sqlite3_errmsg() or sqlite3_errmsg16() routines can be used to obtain an English language description of the error following a failure of any of the sqlite3_open() routines. 
 */
int sql_open_db_rw(void* pHandle, char* pcPathDb);

/**
 * @brief    sql_close_db 
 *
 * @param[in]    pHandle: sql handle
 *
 * @return   	Calls to sqlite3_close_v2() return SQLITE_OK if the sqlite3 object is successfully destroyed and all associated resources are deallocated. 
 */
int sql_close_db(void* pHandle);

/**
 * @brief    sql_exec_cmd 
 *
 * @param[in]    pHandle: sql handle
 *
 * @return   	return SQLITE_OK if exec cmd success, other SQLITE_ERROR 
 */
int sql_exec_cmd(void* pHandle);

/**
 * @brief    sql_create_table 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if create table successfully, other SQLITE_ERROR
 */
int sql_create_table(void* pHandle, char* pcSql);

/**
 * @brief    sql_delete_table 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if delete table successfully, other SQLITE_ERROR
 */
int sql_delete_table(void* pHandle, char* pcSql);

/**
 * @brief    sql_insert_record 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if insert record successfully, other SQLITE_ERROR
 */
int sql_insert_record(void* pHandle, char* pcSql);

/**
 * @brief    sql_rm_record 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if  delete record successfully, other SQLITE_ERROR
 */
int sql_rm_record(void* pHandle, char* pcSql);

/**
 * @brief    sql_query_record 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if query record successfully, other SQLITE_ERROR
 */
int sql_query_record(void* pHandle, char* pcSql);

/**
 * @brief    sql_query_record_all 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if query record successfully, other SQLITE_ERROR
 */
int sql_query_record_all(void* pHandle, char* pcSql);

/**
 * @brief    sql_update_record 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 *
 * @return    return SQLITE_OK if update record successfully, other SQLITE_ERROR
 */
int sql_update_record(void* pHandle, char* pcSql);

/**
 * @brief    sql_get_total_record: get record number from PATH_DATABASE
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 * @param[in]    pnRecdCnt: number of record
 *
 * @return   1 success, 0 failed 
 */
int sql_get_total_record(void* pHandle, char* pcSql, int* pnRecdCnt);

/**
 * @brief    sql_get_user_num: get user number from PATH_DATABASE 
 *
 * @param[in]    pHandle: sql handle
 * @param[in]    pcSql: SQL statement
 * @param[in]    pnUserNum: number of user
 *
 * @return    1 success, 0 failed 
 */
int sql_get_user_num(void* pHandle, char* pcSql, int* pnUserNum);
