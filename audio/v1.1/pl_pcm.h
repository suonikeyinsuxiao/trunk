/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       pl_pcm.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-06-06
*   
***********************************************************************/
#ifndef _PL_PCM_H
#define _PL_PCM_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "wave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <getopt.h>
#include <sys/time.h>
#include <math.h>
#include "asoundlib.h"

typedef struct _sndPcmCxt_S
{
	snd_pcm_t*				m_psHandle;
	snd_pcm_hw_params_t*	m_psHwParams;
	snd_pcm_sw_params_t*	m_psSwParams;
	snd_output_t*			m_psOutput;
	unsigned int 			m_nBufferTime;		/* ring buffer length in us */
	unsigned int 			m_nPeriodTime;		/* period time in us */
	snd_pcm_sframes_t 		m_lBufferSize;
	snd_pcm_sframes_t 		m_lPeriodSize;
	int 					m_nVerbose;			/* verbose flag */
	snd_pcm_format_t 		m_nFormat;			/* sample format */
	unsigned int 			m_nRate;			/* stream rate */
	unsigned int 			m_nChannels;		/* count of channels */
	snd_pcm_access_t 		m_nAccess;
	int 					m_nPeriodEvent;		/* produce poll event after each period */
	WAVEINFO_S*				m_psWaveInfo;
}SNDPCMCXT_S;

#ifdef __cplusplus
}
#endif
#endif // _PL_PCM_H
