/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       pl_pcm.c
*   @brief:      pcm playback 
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-06-06
*   
***********************************************************************/
#include "pl_pcm.h"
#include "log.h"

#ifndef PL_TAG
#define PL_TAG "PL_PCM"
#endif


//#define FILEPATH "tts_sample.wav"
#define FILEPATH "aiscatherine_up1.5.wav"

static char *device = "default";			/* playback device */

int set_hwparams(SNDPCMCXT_S* psSndPcmCxt)
{
	assert(psSndPcmCxt);
	assert(psSndPcmCxt->m_psHandle);
	assert(psSndPcmCxt->m_psHwParams);

	snd_pcm_t *handle = psSndPcmCxt->m_psHandle;
	snd_pcm_hw_params_t *params = psSndPcmCxt->m_psHwParams;
	snd_pcm_access_t access = psSndPcmCxt->m_nAccess;
	snd_pcm_format_t format = psSndPcmCxt->m_nFormat;			/* sample format */
	unsigned int channels = psSndPcmCxt->m_nChannels;			/* count of channels */
	unsigned int rate = psSndPcmCxt->m_nRate;					/* stream rate */
	unsigned int buffer_time = psSndPcmCxt->m_nBufferTime;		/* ring buffer length in us */
	unsigned int period_time = psSndPcmCxt->m_nPeriodTime;		/* period time in us */
	unsigned int rrate;
	snd_pcm_uframes_t size;
	int err, dir;

	/* choose all parameters */
	err = snd_pcm_hw_params_any(handle, params);
	if (err < 0) {
		printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
		return err;
	}
	/* set hardware resampling */
	//err = snd_pcm_hw_params_set_rate_resample(handle, params, resample);
	//if (err < 0) {
	//	printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
	//	return err;
	//}
	/* set the interleaved read/write format */
	err = snd_pcm_hw_params_set_access(handle, params, access);
	if (err < 0) {
		printf("Access type not available for playback: %s\n", snd_strerror(err));
		return err;
	}
	/* set the sample format */
	err = snd_pcm_hw_params_set_format(handle, params, format);
	if (err < 0) {
		printf("Sample format not available for playback: %s\n", snd_strerror(err));
		return err;
	}
	/* set the count of channels */
	err = snd_pcm_hw_params_set_channels(handle, params, channels);
	if (err < 0) {
		printf("Channels count (%i) not available for playbacks: %s\n", channels, snd_strerror(err));
		return err;
	}
	/* set the stream rate */
	rrate = rate;
	err = snd_pcm_hw_params_set_rate_near(handle, params, &rrate, 0);
	if (err < 0) {
		printf("Rate %iHz not available for playback: %s\n", rate, snd_strerror(err));
		return err;
	}
	if (rrate != rate) {
		printf("Rate doesn't match (requested %iHz, get %iHz)\n", rate, err);
		return -EINVAL;
	}
	/* set the buffer time */
	err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
	if (err < 0) {
		printf("Unable to set buffer time %i for playback: %s\n", buffer_time, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_get_buffer_size(params, &size);
	if (err < 0) {
		printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
		return err;
	}
	psSndPcmCxt->m_lBufferSize = size;
	//buffer_size = size;
	PL_LOGD(PL_TAG,"buffer_size=%d, dir=%d\n", psSndPcmCxt->m_lBufferSize, dir);
	/* set the period time */
	err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
	if (err < 0) {
		printf("Unable to set period time %i for playback: %s\n", period_time, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
	if (err < 0) {
		printf("Unable to get period size for playback: %s\n", snd_strerror(err));
		return err;
	}
	psSndPcmCxt->m_lPeriodSize = size;
	//period_size = size;
	PL_LOGD(PL_TAG,"period_size=%d, dir=%d\n", psSndPcmCxt->m_lPeriodSize, dir);
	/* write the parameters to device */
	err = snd_pcm_hw_params(handle, params);
	if (err < 0) {
		printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
		return err;
	}
	return 0;
}

int set_swparams(SNDPCMCXT_S* psSndPcmCxt)
{
	assert(psSndPcmCxt);
	assert(psSndPcmCxt->m_psHandle);
	assert(psSndPcmCxt->m_psSwParams);

	snd_pcm_t *handle = psSndPcmCxt->m_psHandle;
	snd_pcm_sw_params_t *swparams = psSndPcmCxt->m_psSwParams;
	snd_pcm_sframes_t buffer_size = psSndPcmCxt->m_lBufferSize;
	snd_pcm_sframes_t period_size = psSndPcmCxt->m_lPeriodSize;
	int period_event = 0;				/* produce poll event after each period */
	int err;

	/* get the current swparams */
	err = snd_pcm_sw_params_current(handle, swparams);
	if (err < 0) {
		printf("Unable to determine current swparams for playback: %s\n", snd_strerror(err));
		return err;
	}
	/* start the transfer when the buffer is almost full: */
	/* (buffer_size / avail_min) * avail_min */
	err = snd_pcm_sw_params_set_start_threshold(handle, swparams, (buffer_size / period_size) * period_size);
	if (err < 0) {
		printf("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
		return err;
	}
	/* allow the transfer when at least period_size samples can be processed */
	/* or disable this mechanism when period event is enabled (aka interrupt like style processing) */
	err = snd_pcm_sw_params_set_avail_min(handle, swparams, period_event ? buffer_size : period_size);
	if (err < 0) {
		printf("Unable to set avail min for playback: %s\n", snd_strerror(err));
		return err;
	}
	/* enable period events when requested */
	if (period_event) {
		err = snd_pcm_sw_params_set_period_event(handle, swparams, 1);
		if (err < 0) {
			printf("Unable to set period event: %s\n", snd_strerror(err));
			return err;
		}
	}
	/* write the parameters to the playback device */
	err = snd_pcm_sw_params(handle, swparams);
	if (err < 0) {
		printf("Unable to set sw params for playback: %s\n", snd_strerror(err));
		return err;
	}
	return 0;
}

static int xrun_recovery(SNDPCMCXT_S* psSndPcmCxt, int err)
{
	assert(psSndPcmCxt);
	assert(psSndPcmCxt->m_psHandle);
	assert(psSndPcmCxt->m_psSwParams);

	snd_pcm_t *handle = psSndPcmCxt->m_psHandle;
	int verbose = psSndPcmCxt->m_nVerbose;					/* verbose flag */

	if (verbose)
		printf("stream recovery\n");
	if (err == -EPIPE) {	/* under-run */
		err = snd_pcm_prepare(handle);
		if (err < 0)
			printf("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
		return 0;
	} else if (err == -ESTRPIPE) {
		while ((err = snd_pcm_resume(handle)) == -EAGAIN)
			sleep(1);	/* wait until the suspend flag is released */
		if (err < 0) {
			err = snd_pcm_prepare(handle);
			if (err < 0)
				printf("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
		}
		return 0;
	}
	return err;
}

/*
 *   Transfer method - write only
 */
static int write_loop(SNDPCMCXT_S* psSndPcmCxt, signed short *samples, snd_pcm_channel_area_t *areas)
{
	assert(psSndPcmCxt);
	assert(psSndPcmCxt->m_psHandle);
	assert(psSndPcmCxt->m_psWaveInfo);
	assert(psSndPcmCxt->m_psWaveInfo->m_pucData);

    WAVEINFO_S* psWaveInfo = psSndPcmCxt->m_psWaveInfo;
	double phase = 0;
	signed short *ptr;
	int err, cptr;
	char* pcSrc = NULL;
	int nCount = 0;
	int nWriteSize=0;
	int nExitFlag = 0;
	snd_pcm_t *handle = psSndPcmCxt->m_psHandle;
	snd_pcm_sframes_t period_size = psSndPcmCxt->m_lPeriodSize;
	unsigned int channels = psSndPcmCxt->m_nChannels;			/* count of channels */


	char data1[64];
	char data2[64];
	sprintf(data1,"amixer cset numid=27 0");
	sprintf(data2,"amixer cset numid=2 55");
	system(data1);
	system(data2);
	//printf("period_size=%d, buffer_size=%d\n", period_size, buffer_size);

	//memset(&sWaveInfo, 0, sizeof(WAVEINFO_S));
	//openWave(FILEPATH, &sWaveInfo);
	//getWaveHead(&sWaveInfo);

	//sWaveInfo.m_pucData = (unsigned char*)malloc((sWaveInfo.m_sWavHead.m_nSubChunk2Size)); 
	//if(!sWaveInfo.m_pucData)
	//{ 
	//	PL_LOGE(PL_TAG, "[%s,%d]Memory alloc failed!\n",__func__, __LINE__); 
	//	return -1; 
	//}
	pcSrc = psWaveInfo->m_pucData;

	//readWave(&sWaveInfo);

	while (1) {

		if (psWaveInfo->m_sWavHead.m_nSubChunk2Size-nCount < period_size*psWaveInfo->m_sWavHead.m_stBlockAlign)
		{
			memcpy(samples, pcSrc, psWaveInfo->m_sWavHead.m_nSubChunk2Size-nCount);
			nWriteSize = psWaveInfo->m_sWavHead.m_nSubChunk2Size-nCount;
			ptr = samples;
			cptr = nWriteSize/psWaveInfo->m_sWavHead.m_stBlockAlign;
			nExitFlag = 1;
		}
		else
		{
			memcpy(samples, pcSrc, period_size*psWaveInfo->m_sWavHead.m_stBlockAlign);
			nWriteSize = period_size*psWaveInfo->m_sWavHead.m_stBlockAlign;
			ptr = samples;
			cptr = period_size;
		}
		
		while (cptr > 0) {
			err = snd_pcm_writei(handle, ptr, cptr);
			if (err == -EAGAIN)
				continue;
			if (err < 0) {
				if (xrun_recovery(psSndPcmCxt, err) < 0) {
					printf("Write error: %s\n", snd_strerror(err));
					exit(EXIT_FAILURE);
				}
				break;	/* skip one period */
			}
			ptr += err * channels;
			cptr -= err;
		}
		pcSrc += nWriteSize; 
		nCount += nWriteSize;
		if (nExitFlag)
		{
			sleep(1);
			break;
		}
	}

	//destroyWave(&sWaveInfo);
	return err;
}

struct transfer_method {
	const char *name;
	snd_pcm_access_t access;
	int (*transfer_loop)(SNDPCMCXT_S* psSndPcmCxt,
			     signed short *samples,
			     snd_pcm_channel_area_t *areas);
};

static struct transfer_method transfer_methods[] = {
	{ "write", SND_PCM_ACCESS_RW_INTERLEAVED, write_loop },
	//{ "write_and_poll", SND_PCM_ACCESS_RW_INTERLEAVED, write_and_poll_loop },
	//{ "async", SND_PCM_ACCESS_RW_INTERLEAVED, async_loop },
	//{ "async_direct", SND_PCM_ACCESS_MMAP_INTERLEAVED, async_direct_loop },
	//{ "direct_interleaved", SND_PCM_ACCESS_MMAP_INTERLEAVED, direct_loop },
	//{ "direct_noninterleaved", SND_PCM_ACCESS_MMAP_NONINTERLEAVED, direct_loop },
	//{ "direct_write", SND_PCM_ACCESS_MMAP_INTERLEAVED, direct_write_loop },
	{ NULL, SND_PCM_ACCESS_RW_INTERLEAVED, NULL }
};


int main(int argc, char *argv[])
{
	snd_pcm_t *handle;
	int err, morehelp;
	snd_pcm_hw_params_t *hwparams;
	snd_pcm_sw_params_t *swparams;
	snd_output_t *output = NULL;
	int method = 0;
	signed short *samples;
	unsigned int chn;
	snd_pcm_channel_area_t *areas;
	unsigned int channels = 1;						/* count of channels */
	snd_pcm_format_t format = SND_PCM_FORMAT_S16;	/* sample format */
	unsigned int rate = 16000;						/* stream rate */
	snd_pcm_sframes_t period_size;
	SNDPCMCXT_S sSndPcmContext;
    WAVEINFO_S sWaveInfo;

	memset(&sWaveInfo, 0, sizeof(WAVEINFO_S));
	memset(&sSndPcmContext, 0, sizeof(sSndPcmContext));

	openWave(FILEPATH, &sWaveInfo);
	getWaveHead(&sWaveInfo);

	sWaveInfo.m_pucData = (unsigned char*)malloc((sWaveInfo.m_sWavHead.m_nSubChunk2Size)); 
	if(!sWaveInfo.m_pucData)
	{ 
		PL_LOGE(PL_TAG, "[%s,%d]Memory alloc failed!\n",__func__, __LINE__); 
		return -1; 
	}
	readWave(&sWaveInfo);

	//init SNDPCMCXT_S
	sSndPcmContext.m_psWaveInfo = &sWaveInfo;
	sSndPcmContext.m_nChannels = sWaveInfo.m_sWavHead.m_stNumChannels;//1;
	sSndPcmContext.m_nRate = sWaveInfo.m_sWavHead.m_nSampleRate;//16000;
	sSndPcmContext.m_nFormat = SND_PCM_FORMAT_S16;
	sSndPcmContext.m_nVerbose = 0;
	sSndPcmContext.m_nPeriodEvent = 0;
	sSndPcmContext.m_nAccess = SND_PCM_ACCESS_RW_INTERLEAVED;
	
	handle = sSndPcmContext.m_psHandle;
	snd_pcm_hw_params_alloca(&(sSndPcmContext.m_psHwParams));
	snd_pcm_sw_params_alloca(&(sSndPcmContext.m_psSwParams));
	hwparams = sSndPcmContext.m_psHwParams;
	swparams = sSndPcmContext.m_psSwParams;

	output = sSndPcmContext.m_psOutput;
	channels = sSndPcmContext.m_nChannels;
	format = sSndPcmContext.m_nFormat;
	rate = sSndPcmContext.m_nRate;

	err = snd_output_stdio_attach(&output, stdout, 0);
	if (err < 0) {
		printf("Output failed: %s\n", snd_strerror(err));
		return 0;
	}

	printf("Playback device is %s\n", device);
	printf("Stream parameters are %iHz, %s, %i channels\n", rate, snd_pcm_format_name(format), channels);
	//printf("Sine wave rate is %.4fHz\n", freq);
	printf("Using transfer method: %s\n", transfer_methods[method].name);

	if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		printf("Playback open error: %s\n", snd_strerror(err));
		return 0;
	}
	sSndPcmContext.m_psHandle = handle;
	
	//if ((err = set_hwparams(handle, hwparams, transfer_methods[method].access)) < 0) {
	if ((err = set_hwparams(&sSndPcmContext)) < 0) {
		printf("Setting of hwparams failed: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	//if ((err = set_swparams(handle, swparams)) < 0) {
	if ((err = set_swparams(&sSndPcmContext)) < 0) {
		printf("Setting of swparams failed: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	//if (verbose > 0)
	if (sSndPcmContext.m_nVerbose > 0)
		snd_pcm_dump(handle, output);

	period_size = sSndPcmContext.m_lPeriodSize;
	samples = malloc((period_size * channels * snd_pcm_format_physical_width(format)) / 8);//1600*2 Bytes
	if (samples == NULL) {
		printf("No enough memory\n");
		exit(EXIT_FAILURE);
	}
	
	areas = calloc(channels, sizeof(snd_pcm_channel_area_t));
	if (areas == NULL) {
		printf("No enough memory\n");
		exit(EXIT_FAILURE);
	}
	for (chn = 0; chn < channels; chn++) {
		areas[chn].addr = samples;
		areas[chn].first = chn * snd_pcm_format_physical_width(format);
		areas[chn].step = channels * snd_pcm_format_physical_width(format);
	}

	err = transfer_methods[method].transfer_loop(&sSndPcmContext, samples, areas);
	if (err < 0)
		printf("Transfer failed: %s\n", snd_strerror(err));

	destroyWave(&sWaveInfo);
	free(areas);
	free(samples);
	snd_pcm_close(handle);
	return 0;
}

