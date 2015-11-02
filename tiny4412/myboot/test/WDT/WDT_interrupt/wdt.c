#include "wdt.h"

void initWDT(WDTConfig_S* psWDT)
{
	//t_watchdog = 1/(PCLK/(Prescaler value + 1)/Division_factor)
	
	//PMU SET enable WDT RESET
	if (psWDT->m_nEnableReset)
	{
		AUTOMATIC_WDT_RESET_DISABLE &= ~(1 << 20) ; 
		MASK_WDT_RESET_REQUEST &= ~(1 << 20) ; 

		//timer count
		WTCNT = psWDT->m_nCount; 
		
		WTCON &= ~(0xffff);
		//set prescaler
		WTCON |= (psWDT->m_nPrescaler << 8);	
		//set division factor
		WTCON |= (psWDT->m_nDivision << 3);
		//enbale WDT timer
		WTCON |= (psWDT->m_nEnableWDT << 5);
		//enable/disable reset
		WTCON |= (psWDT->m_nEnableReset);

	}

	//PMU SET enable WDT interrupt 
	if (psWDT->m_nEnableInterrupt)
	{
		//timer count
		WTCNT = psWDT->m_nCount; 
		WTDAT = psWDT->m_nCount;
		
		WTCON &= ~(0xffff);
		//set prescaler
		WTCON |= (psWDT->m_nPrescaler << 8);	
		//set division factor
		WTCON |= (psWDT->m_nDivision << 3);
		//enbale WDT timer
		WTCON |= (psWDT->m_nEnableWDT << 5);
		//enbale/disable interrupt
		WTCON |= (1 << 2);

	}
}

void feedDog(unsigned int count)
{
	WTCNT = count;
}
