/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       face.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-27
*   
***********************************************************************/
#ifndef _FACE_H
#define _FACE_H

#include "ssudp_api.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define CMD_KEY							("cmd")
#define CMD_TAKEPHOTO_REQUEST 			("takephoto_request")
#define CMD_TAKEPHOTO_RESPONSE 			("takephoto_response")
#define CMD_REGISTER_REQUEST 			("register_request")
#define CMD_REGISTER_RESPONSE 			("register_response")
#define CMD_CHGNAME_REQUEST 			("chgname_request")
#define CMD_CHGNAME_RESPONSE 			("chgname_response")
#define CMD_ALBUM_NAME_QUERY_REQUEST 	("album_name_query")
#define CMD_ALBUM_NAME_QUERY_RESPONSE 	("album_name_query_response")
#define	CMD_RECOG_RESULT_REQUEST		("recog_result_request")
#define	CMD_RECOG_RESULT_RESPONSE		("recog_result_response")
#define CMD_ALBUM_QUERY_REQUEST 		("album_query")
#define CMD_ALBUM_QUERY_RESPONSE 		("album_query_response")
#define CMD_RM_REQUEST					("rm_request")
#define CMD_RM_RESPONSE					("rm_response")

#define CMDID_KEY						("cmd_id")

#define METADATA_KEY					("metadata")
#define METADATA_ALBUM_USERID			("album_userid")
#define METADATA_ALBUM_DATAID			("album_dataid")
#define METADATA_NAME					("name")
#define METADATA_STATUS					("status")

//#define NAMELEN							(128)
//#define USERID_MAX						(5)
//#define DATAID_MAX						(5)

#if 0
typedef struct _person_s
{
	int                 m_nAlbumUserId;//id有效值从1开始计数
	int                 m_anAlbumUserDataId[DATAID_MAX];
	unsigned char      	m_acName[NAMELEN];
	int                 m_nNameLen;
}PERSON_S;

typedef struct _handles_s
{
	HDETECTION 		m_hDT;			/* Face Detection Handle */
	HDTRESULT 		m_hDtResult;	/* Face Detection Result Handle */
	HPOINTER 		m_hPT;			/* Facial Parts Detection Handle */
	HPTRESULT 		m_hPtResult;	/* Facial Parts Detection Result Handle */
	HALBUM 			m_hAL;			/* Album Handle */
	HFEATURE 		m_hFD;			/* Feature Handle */

	PERSON_S    	m_asAlbum[USERID_MAX];  	/* Album Data */
	int				m_nCmdId;
}HANDLES;
#endif



typedef struct _handles_s HANDLES;

/*==========parse json API==========*/
int decodeTakePhotoRequest(unsigned char* pcStr, HANDLES* pHandles);
int encodeTakePhotoResponse(void* pcs, HANDLES* pHandles);

int decodeRegisterRequest(unsigned char* pcStr, HANDLES* pHandles, int* pnAlbumUserId, int* pnAlbumDataId);
int encodeRegisterResponse(void* pcs, HANDLES* pHandles, int nAlbumUserId, int nAlbumDataId, int nStatus);

int decodeChgnameRequest(unsigned char* pcStr, HANDLES* pHandles, int* pnAlbumUserId);
int encodeChgnameResponse(void* pcs, int nAlbumUserId, HANDLES* pHandles);

int decodeAlbumNameQuery(unsigned char* pcStr, HANDLES* pHandles);
int encodeAlbumNameQueryResponse(void* pcs, HANDLES* pHandles);

int decodeAlbumQuery(unsigned char* pcStr, HANDLES* pHandles);
int encodeAlbumQueryResponse(unsigned char* pcStr, HANDLES* pHandles);

int decodeRmRequest(unsigned char* pcStr, HANDLES* pHandles);
int encodeRmResponse(void* pcs, HANDLES* pHandles, unsigned char* pcReqStr);

int encodeRocResponse(void* pcs, HANDLES* pHandles, int nAlbumUserId);
/*==========parse json API end==========*/


/*==========face handle API==========*/
int initFaceLibs(HANDLES* pHandles, unsigned int unDbSize);
/*========== face handle API end==========*/

/*========== ==========*/
#ifdef __cplusplus
}
#endif
#endif // _FACE_H
