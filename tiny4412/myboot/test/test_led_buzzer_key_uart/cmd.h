#ifndef __CMD_H
#define __CMD_H

#define ARRAYSIZE(a) (sizeof(a)/sizeof((a)[0]))

typedef enum
{
	LEDALLON,
	LEDALLOFF,
	BUZZERON,
	BUZZEROFF,
	KEY1ON,
	KEY2ON,
	KEY3ON,
	KEY4ON,
	KEY1OFF,
	KEY2OFF,
	KEY3OFF,
	KEY4OFF,
	CMDUNKNOWN
}CMDTYPE_E;

typedef struct _uartCommand
{
	CMDTYPE_E    m_cmdType;	
	char         m_acCmdName[32];
	//unsigned int (*findCommand_F) (const char* pcCmd, command_S* psCmd, int size);
	//void (* doCmd_F) (CMDTYPE_E cmdType);
}uartCommand_S;

//void initCmd(void);

unsigned int findCommand(const char* pcCmd, uartCommand_S* psCmd, int size);

void doCmd(CMDTYPE_E cmdType);

#endif
