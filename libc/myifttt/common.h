/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       common.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-20
*   
***********************************************************************/
#ifndef _COMMON_H
#define _COMMON_H


#ifdef __cplusplus
extern "C"
{
#endif

#define CLASS_BASE				(0x1000)


/*input class name*/
#define CLASS_SYSTEM					("system")
#define CLASS_WIFI						("wifi")
#define CLASS_NETWORK					("nerwork")
#define CLASS_ZIGBEE_CO					("zigbee_co")
#define CLASS_ZIGBEE_DEV				("zigbee_dev")
#define CLASS_SDCARD					("sdcard")
#define CLASS_AUDIO						("audio")
#define CLASS_VIDEO						("video")
#define CLASS_POWER						("power")
#define CLASS_TIME						("time")
#define CLASS_LOCAL_SENSOR_AND_INPUT	("local_senser_and_input")
#define CLASS_GIS						("GIS")
#define CLASS_LOCAL_COMMAND				("local_command")
#define CLASS_CLOUD_COMMAND				("cloud_command")

/*input subclass name*/
/*CLASS_SYSTEM*/
#define SYS_SUBCLASS_FIRMWARE		("firmware")//All firmware version or firmware sub block change generated input,i.e. firmware version needs update.
#define SYS_SUBCLASS_HARDWARE		("hardware")//All hardware component changes generated input, i.e. one of hardware didn't function
#define SYS_SUBCLASS_USAGE			("usage")//usage conditions caused input, i.e. "reset-to-factory" cause input to trigger beep
#define SYS_SUBCLASS_WORKMODE		("workmode")//Work mode change caused input, i.e. work mode change cause notification to other blocks
#define SYS_SUBCLASS_POWER			("power")//Power mode change caused input, i.e. from "battery" to "power connected" cause notification to other blocks
#define SYS_SUBCLASS_SELFTEST		("selftest")
#define SYS_SUBCLASS_ERROR			("error")//System Error cause input notification
/*CLASS_WIFI*/
#define WIFI_SUBCLASS_CONFIG		("config")//Config change cause input to other blocks
#define WIFI_SUBCLASS_PHYSICAL		("physical")//Physical change cause input to other blocks
/*CLASS_NETWORK*/
#define NETWORK_SUBCLASS_CONFIG		("config")//Config change cause input to other blocks
#define NETWORK_SUBCLASS_PHYSICAL	("status")//Status change cause input to other blocks
/*CLASS_ZIGBEE_CO*/
#define ZIGBEE_CO_SUBCLASS_FIRMWARE		("firmware")
#define ZIGBEE_CO_SUBCLASS_PHYSICAL		("physical") 
/*CLASS_ZIGBEE_DEV*/
#define ZIGBEE_DEV_SUBCLASS_SENSOR_STATUS	("sensor_status")//Sensor Status change, including new device join, device drop, RF signal power weak, etc
#define ZIGBEE_DEV_SUBCLASS_SENSOR_ALERT	("sensor_alert")//Sensor alert: INT_Sensor; Switch, light, lock, curtain, etc.
#define ZIGBEE_DEV_SUBCLASS_COMMAND			("command")//Zigbee command receive from Zigbee devices.
/*CLASS_SDCARD*/
#define SDCARD_SUBCLASS_VOLUME				("volume")//Volume change. including unplug SD card.
#define SDCARD_SUBCLASS_CONTENT				("content")//Content change in SD card
/*CLASS_TIME*/
#define TIME_SUBCLASS_LOCAL					("local")//Input to change system time, Scheduled time reached;Update in 1 minute
#define TIME_SUBCLASS_WORLDWIDE				("worldwide")//Input to change World time, time zone, etc. Scheduled time reached;Update in 1 minute
/*CLASS_LOCAL_SENSOR_AND_INPUT*/
#define LOCAL_SUBCLASS_KEY					("key")//Input from press key (play/pause/volume up/volume down/forward/backward)
#define LOCAL_SUBCLASS_GSENSOR				("gsensor")//INT of Gsensor and Gsensor reading
#define LOCAL_SUBCLASS_TOUCHKEY				("touchkey")//INT of touch key 
#define LOCAL_SUBCLASS_PIR					("pir")//INT of PIR 
#define LOCAL_SUBCLASS_LIGHT_SENSOR			("light_sensor")//INT of Light Sensor and Light intensity reading
/*CLASS_GIS*/
#define GIS_SUBCLASS_WORLDWIDE				("worldwide")//Physical location detected worldwide changed cause input
#define GIS_SUBCLASS_IN_HOUSE				("in_house")//Physical location  in house changed cause input
/*CLASS_LOCAL_COMMAND*/
#define LOCAL_CMD_SUBCLASS_RECEIVED			("received")//local command received
/*CLASS_CLOUD_COMMAND*/
#define CLOUD_CMD_SUBCLASS_RECEIVED			("received")//remote command received
/*CLASS_AUDIO*/
#define AUDIO_SUBCLASS_LOCAL_CMD			("local_cmd")//Received voice match in local command set
#define AUDIO_SUBCLASS_EXPRESSION			("expression")//A expression sound match, i.e. laugh, cry, etc.
#define AUDIO_SUBCLASS_SOUND				("sound")//A sound match, i.e. water drop, hammer, etc.
#define AUDIO_SUBCLASS_CLOUD_MATCHED_CMD	("cloud_matched_cmd")//A voice match in pre-define word or sentence. 


#ifdef __cplusplus
}
#endif
#endif // _COMMON_H
