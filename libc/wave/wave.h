/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       wave.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-03-05
*   
***********************************************************************/
#ifndef _WAVE_H
#define _WAVE_H
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief    standard wave file format
 * default format
 *	WAVEFORMAT_S default_wavfmt = 
 *	{
 *		{ 'R', 'I', 'F', 'F' },
 *		0,
 *		{'W', 'A', 'V', 'E'},
 *		{'f', 'm', 't', ' '},
 *		16,
 *		1,
 *		1,
 *		16000,
 *		32000,
 *		2,
 *		16,
 *		{'d', 'a', 't', 'a'},
 *		0  
 *	};
 */
typedef struct _WAVEFORMAT_S
{
	//"RIFF" chunk descriptor
	unsigned char 	m_aucChunkID[4];	/*4-big endian 一般就是RIFF*/
	int 			m_nChunkSize;       /*4-file size, 不包括m_nChunkID和m_nChunkSize, filesize-8*/
	unsigned char	m_aucFormat[4];			/*4-big endian "WAVE"*/
	//"fmt" sub-chunk
	unsigned char	m_aucSubChunk1ID[4];		/*4-big endian 一般就是'f','m','t',' '四个字母*/
	int 			m_nSubChunk1Size;	/*4-本数据块的大小(16),不包括m_nSubChunk1ID和m_nSubChunk1Size*/
	short 			m_stAudioFormat;	/*2-pcm:1*/		
	short 			m_stNumChannels;	/*2-通道数*/		
	int				m_nSampleRate;		/*4-采样率 8000 | 16000 | 44100*/	
	int 			m_nByteRate;		/*4-比特率,每秒的字节数 m_nSampleRate*m_stBitsPerSample/8*/
	short 			m_stBlockAlign;		/*2-每个采样点的字节数 m_stBitsPerSample/8*/	
	short 			m_stBitsPerSample;	/*2-采样位数 8 | 16*/	
	//"data" sub-chunk
	unsigned char	m_aucSubChunk2ID[4];	/*4-big endian, 一般为"data"*/
	int 			m_nSubChunk2Size;	/*4-本数据块的大小,即音频数据的长度,不包括m_acSubChunk2ID[4]和m_nSubChunk2Size, filezize-44*/	
	unsigned char* 	m_pucData;		
}WAVEFORMAT_S;

double* wavread(char* pcFileName, WAVEFORMAT_S* psWaveFmt); 

void freeWave(double* pdAudRecord);

double* getAudAbs(double* pdAudData, int nDataLen);


#ifdef __cplusplus
}
#endif
#endif // _WAVE_H
