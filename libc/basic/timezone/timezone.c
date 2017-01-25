/***********************************************************************
*   Copyright (C) 2017 pilot-lab.inc All rights reserved.
*   
*   @file:       main.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2017-01-17
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if 0
int main(int argc, char** argv)
{
	time_t time_utc = 0;
	struct tm *p_tm_time;
	int time_zone = 0;

	p_tm_time = localtime(&time_utc);   //转成当地时间

	time_zone = ( p_tm_time->tm_hour > 12 ) ?   ( p_tm_time->tm_hour-=  24 )  :  p_tm_time->tm_hour;
	printf("time_zone=%d, p_tm_time->tm_hour=%d\n", time_zone, p_tm_time->tm_hour);

	return 0;
}
#endif

#if 0
int getZone()
{  
	time_t time_utc;  
	struct tm tm_local;  

	// Get the UTC time  
	time(&time_utc);  

	// Get the local time  
	// Use localtime_r for threads safe  
	localtime_r(&time_utc, &tm_local);  

	time_t time_local;  
	struct tm tm_gmt;  

	// Change tm to time_t   
	time_local = mktime(&tm_local);  

	// Change it to GMT tm  
	gmtime_r(&time_utc, &tm_gmt);  

	printf("tm_local.tm_hour = %d, tm_gmt.tm_hour=%d\n", tm_local.tm_hour ,tm_gmt.tm_hour);

	int time_zone = tm_local.tm_hour - tm_gmt.tm_hour;  
	if (time_zone < -12) {  
		time_zone += 24;   
	} else if (time_zone > 12) {  
		time_zone -= 24;  
	}  

	char cur_time[256];  
	strftime (cur_time,256, " %B %A %Y_%m_%d  %H_%M_%S   %c  %x %X", &tm_local);  
	printf("LOCAL TIME  :%s\n", cur_time);  

	strftime(cur_time,256, " %B %A %Y_%m_%d  %H_%M_%S   %c  %x %X", &tm_gmt);  
	printf("GMT TIME    :%s\n", cur_time);  

	printf("Your time zone is +%d. (- is west, + is east)\n", time_zone);  

	return time_zone;  
}
#endif

int getZone()
{  
	time_t time_utc;  
	struct tm tm_local;  
	struct tm tm_gmt;  
	//time_t time_local;  

	// Get the UTC time  
	time(&time_utc);  

	// Get the local time  
	// Use localtime_r for threads safe  
	localtime_r(&time_utc, &tm_local);  


	// Change tm to time_t   
	//time_local = mktime(&tm_local);  

	// Change it to GMT tm  
	gmtime_r(&time_utc, &tm_gmt);  

	printf("tm_local.tm_hour = %d, tm_gmt.tm_hour=%d\n", tm_local.tm_hour ,tm_gmt.tm_hour);

	int time_zone = tm_local.tm_hour - tm_gmt.tm_hour;  
	if (time_zone < -12) {  
		time_zone += 24;   
	} else if (time_zone > 12) {  
		time_zone -= 24;  
	}  

	//char cur_time[256];  
	//strftime (cur_time,256, " %B %A %Y_%m_%d  %H_%M_%S   %c  %x %X", &tm_local);  
	//printf("LOCAL TIME  :%s\n", cur_time);  

	//strftime(cur_time,256, " %B %A %Y_%m_%d  %H_%M_%S   %c  %x %X", &tm_gmt);  
	//printf("GMT TIME    :%s\n", cur_time);  

	printf("Your time zone is +%d. (- is west, + is east)\n", time_zone);  

	return time_zone;  
}

int main(int argc,char *argv[])  
{  
	getZone();

	return 0;  
}  


