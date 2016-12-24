/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       input.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-20
*   
***********************************************************************/
#ifndef _INPUT_H
#define _INPUT_H

#include "klist.h"
#include "status.h"
#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*input class name*/
#define INPUT_CLASS_SYSTEM						("system")
#define INPUT_CLASS_WIFI						("wifi")
#define INPUT_CLASS_NETWORK						("nerwork")
#define INPUT_CLASS_ZIGBEE_CO					("zigbee_co")
#define INPUT_CLASS_ZIGBEE_DEV					("zigbee_dev")
#define INPUT_CLASS_SDCARD						("sdcard")
#define INPUT_CLASS_AUDIO						("audio")
#define INPUT_CLASS_VIDEO						("video")
#define INPUT_CLASS_POWER						("power")
#define INPUT_CLASS_TIME						("time")
#define INPUT_CLASS_LOCAL_SENSOR_AND_INPUT		("local_senser_and_input")
#define INPUT_CLASS_GIS							("GIS")
#define INPUT_CLASS_LOCAL_COMMAND				("local_command")
#define INPUT_CLASS_CLOUD_COMMAND				("cloud_command")

/*input subclass name*/
/*CLASS_SYSTEM*/
#define INPUT_SYS_SUBCLASS_FIRMWARE				("firmware")//All firmware version or firmware sub block change generated input,i.e. firmware version needs update.
#define INPUT_SYS_SUBCLASS_HARDWARE				("hardware")//All hardware component changes generated input, i.e. one of hardware didn't function
#define INPUT_SYS_SUBCLASS_USAGE					("usage")//usage conditions caused input, i.e. "reset-to-factory" cause input to trigger beep
#define INPUT_SYS_SUBCLASS_WORKMODE				("workmode")//Work mode change caused input, i.e. work mode change cause notification to other blocks
#define INPUT_SYS_SUBCLASS_POWER					("power")//Power mode change caused input, i.e. from "battery" to "power connected" cause notification to other blocks
#define INPUT_SYS_SUBCLASS_SELFTEST				("selftest")
#define INPUT_SYS_SUBCLASS_ERROR					("error")//System Error cause input notification
/*CLASS_WIFI*/
#define INPUT_WIFI_SUBCLASS_CONFIG				("config")//Config change cause input to other blocks
#define INPUT_WIFI_SUBCLASS_PHYSICAL				("physical")//Physical change cause input to other blocks
/*CLASS_NETWORK*/
#define INPUT_NETWORK_SUBCLASS_CONFIG				("config")//Config change cause input to other blocks
#define INPUT_NETWORK_SUBCLASS_PHYSICAL			("status")//Status change cause input to other blocks
/*CLASS_ZIGBEE_CO*/
#define INPUT_ZIGBEE_CO_SUBCLASS_FIRMWARE			("firmware")
#define INPUT_ZIGBEE_CO_SUBCLASS_PHYSICAL			("physical") 
/*CLASS_ZIGBEE_DEV*/
#define INPUT_ZIGBEE_DEV_SUBCLASS_SENSOR_STATUS	("sensor_status")//Sensor Status change, including new device join, device drop, RF signal power weak, etc
#define INPUT_ZIGBEE_DEV_SUBCLASS_SENSOR_ALERT	("sensor_alert")//Sensor alert: INT_Sensor; Switch, light, lock, curtain, etc.
#define INPUT_ZIGBEE_DEV_SUBCLASS_COMMAND			("command")//Zigbee command receive from Zigbee devices.
/*CLASS_SDCARD*/
#define INPUT_SDCARD_SUBCLASS_VOLUME				("volume")//Volume change. including unplug SD card.
#define INPUT_SDCARD_SUBCLASS_CONTENT				("content")//Content change in SD card
/*CLASS_TIME*/
#define INPUT_TIME_SUBCLASS_LOCAL					("local")//Input to change system time, Scheduled time reached;Update in 1 minute
#define INPUT_TIME_SUBCLASS_WORLDWIDE				("worldwide")//Input to change World time, time zone, etc. Scheduled time reached;Update in 1 minute
/*CLASS_LOCAL_SENSOR_AND_INPUT*/
#define INPUT_LOCAL_SUBCLASS_KEY					("key")//Input from press key (play/pause/volume up/volume down/forward/backward)
#define INPUT_LOCAL_SUBCLASS_GSENSOR				("gsensor")//INT of Gsensor and Gsensor reading
#define INPUT_LOCAL_SUBCLASS_TOUCHKEY				("touchkey")//INT of touch key 
#define INPUT_LOCAL_SUBCLASS_PIR					("pir")//INT of PIR 
#define INPUT_LOCAL_SUBCLASS_LIGHT_SENSOR			("light_sensor")//INT of Light Sensor and Light intensity reading
/*CLASS_GIS*/
#define INPUT_GIS_SUBCLASS_WORLDWIDE				("worldwide")//Physical location detected worldwide changed cause input
#define INPUT_GIS_SUBCLASS_IN_HOUSE				("in_house")//Physical location  in house changed cause input
/*CLASS_LOCAL_COMMAND*/
#define INPUT_LOCAL_CMD_SUBCLASS_RECEIVED			("received")//local command received
/*CLASS_CLOUD_COMMAND*/
#define INPUT_CLOUD_CMD_SUBCLASS_RECEIVED			("received")//remote command received
/*CLASS_AUDIO*/
#define INPUT_AUDIO_SUBCLASS_LOCAL_CMD			("local_cmd")//Received voice match in local command set
#define INPUT_AUDIO_SUBCLASS_EXPRESSION			("expression")//A expression sound match, i.e. laugh, cry, etc.
#define INPUT_AUDIO_SUBCLASS_SOUND				("sound")//A sound match, i.e. water drop, hammer, etc.
#define INPUT_AUDIO_SUBCLASS_CLOUD_MATCHED_CMD	("cloud_matched_cmd")//A voice match in pre-define word or sentence. 



#define INPUT_NAME_LEN			(256)
#define INPUT_CLASS_NAME_LEN	(64)


//一个condition必须含有一个已经存在的status， 一个status可以属于[0~n]个condition
typedef struct ifttt_condition_s
{
	int 					m_nCondition;
	int						m_nCount;
	int						m_nIndex;
	IFTTT_STATUS_S*			m_psSatus;
	struct list_head*		m_psList;
}IFTTT_CONDITION_S;

//一个input属于[0~n]个ifttt
typedef struct ifttt_input_cell_s
{
	int 	m_nInputId;//需要知道ID的分类和范围
	char 	m_acClassName[INPUT_CLASS_NAME_LEN];
	char 	m_acSubClassName[INPUT_CLASS_NAME_LEN];
	int 	m_nTrigger;//static_or_dynamic: (external trigger or system status trigger)
	int		m_nPriority;//1-8, 1 being the highest priority
	int		m_nRef;//count of reference
	//char 	m_acInputName[INPUT_NAME_LEN];//暂不使用
}IFTTT_INPUT_CELL_S;

//一个input可以包含[1~n]个condition， 一个condition可以属于[0~n]个input
typedef struct ifttt_input_s
{
	int					m_nCount;//number of cells
	IFTTT_INPUT_CELL_S* m_psCells;
	int					m_nResize;//number of resize
	struct list_head*	m_psList;
	struct list_head*	m_psFirst;
	struct list_head*	m_psLast;
}IFTTT_INPUT_S;

#ifdef __cplusplus
}
#endif
#endif // _INPUT_H
