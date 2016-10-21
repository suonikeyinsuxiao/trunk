#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_NAME "config"


int isMainKey(char* pcStr);
int getFileSize(FILE* fp);

int main(int argc, char** argv)
{
	FILE* fp = NULL;
	char acLineBuf[256] = {0};
	long lFileSize = 0;
	long lTmpSize = 0;
	char acMainKey[128] = {0};
	int  nMainKeyLen = 0;
	char acSubKey[128] = {0};
	int  nSubKeyLen = 0;

	fp = fopen(CONFIG_NAME, "r");
	if (NULL == fp)
	{
		perror("open file " CONFIG_NAME);
		return -1;
	}

	lFileSize = getFileSize(fp);	
	printf("file %s size:%ld\n", CONFIG_NAME, lFileSize);

#if 0
	while(fgets(acLineBuf, 256, fp))
	{
		printf("line string is %s\n", acLineBuf);		
		memset(acLineBuf, 0, 256);
	}
#endif

#if 0
	do
	{
		memset(acLineBuf, 0, 256);
		fgets(acLineBuf, 256, fp);
		printf("line string is %s\n", acLineBuf);		
	}
	while(isMainKey(acLineBuf));
#endif

	while(1)
	{
		memset(acLineBuf, 0, 256);
		fgets(acLineBuf, 256, fp);
		printf("line string is %s", acLineBuf);		

		lTmpSize += strlen(acLineBuf);

		if (isMainKey(acLineBuf))
		{
			printf("main key...\n");
			getMainKey(acLineBuf, acMainKey, &nMainKeyLen);
			acMainKey[nMainKeyLen] = '\0';
			printf("main key2 %s\n", acMainKey);
		}
		else
		{
			printf("sub key...\n");
		}
		printf("\n");

		if (lTmpSize >= lFileSize)
			break;
	}

	return 0;
}

int isMainKey(char* pcStr)
{
	if (NULL == pcStr)
		return 0;

	int nLen = 0;	

	nLen = strlen(pcStr);

	if ((pcStr[0] == '[') && (pcStr[nLen-2] == ']'))
	{
		printf("nLen = %d\n", nLen);
		return 1;
	}

	return 0;
}

int getMainKey(char* pcStr, char* pcMainKey, int* pnLen)
{
	int nSrcLen = strlen(pcStr);
	memcpy(pcMainKey, pcStr+1, nSrcLen-2);
	*pnLen = nSrcLen-3;

	return 0;
}

int getFileSize(FILE* fp)
{
	long lCur = ftell(fp);
	long lFileSize = 0;

	if (0 != fseek(fp, 0L, SEEK_END))
	{
		perror("fseek ");	
		return -1;
	}

	lFileSize = ftell(fp);

	if (0 != fseek(fp, lCur, SEEK_SET))
	{
		perror("fseek ");	
		return -1;
	}

	return lFileSize;
}

int getSubKey()
{

}

int getSubVal()
{

}
	
