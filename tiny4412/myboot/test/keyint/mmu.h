#ifndef __MMU_H
#define __MMU_H

#define TTBADDR      0x41000000       //translation table physical base address 
#define EXCEPTHANDLERBASEADDR 0X510000000	  //exception handlers physical base address 

void initTTB(unsigned int *addr);
void enableMMU();
void vpMap(unsigned int *addr, unsigned va, unsigned pa);

void doReset(void);
void doUndefined(void);
void doSvc(void);
void doPrefetchAbt(void);
void doDataAbt(void);
void doIrq(void);
void doFiq(void);

//void initExceptHandlers(unsigned long long nPhyBaseAddr);
void initExceptHandlers();

void vectorCopy(void);


#endif //__MMU_H
