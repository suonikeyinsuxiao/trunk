#include "cmd.h"
#include "cmd_led.h"
#include "cmd_buzzer.h"

unsigned int findCommand (const char* pcCmd, uartCommand_S* psCmd, int size)
{
	int i = 0;	
	CMDTYPE_E eCmdType = CMDUNKNOWN;	

	for (i = 0;i < size; i++)
	{
		if (0 == strcmp(pcCmd, psCmd[i].m_acCmdName))	
		{
		    eCmdType = psCmd[i].m_cmdType;	
			break;
		}
	}

	return eCmdType; 
}

void doCmd (CMDTYPE_E cmdType)
{
	switch(cmdType)
	{
		case LEDALLON:
		{
			ledAllOn();			
		}
		break;
		case LEDALLOFF:
		{
			ledAllOff();		
		}
		break;
		case BUZZERON:
		{
			buzzerOn();	
		}
		break;
		case BUZZEROFF:
		{
			buzzerOff();	
		}
		break;
		case KEY1ON:
		{
		
		}
		break;
		case KEY1OFF:
		{
		
		}
		break;
		case KEY2ON:
		{
		
		}
		break;
		case KEY2OFF:
		{
		
		}
		break;
	    case KEY3ON:
		{
		
		}
		break;
		case KEY3OFF:
		{
		
		}
		break;
		case KEY4ON:
		{
		
		}
		break;
		case KEY4OFF:
		{
		
		}
		break;
	}
}
