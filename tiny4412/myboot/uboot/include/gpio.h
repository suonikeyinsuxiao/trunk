#ifndef __TINY4412_GPIO_H
#define __TINY4412_GPIO_H

/* Pin configurations */
#define GPIO_INPUT	0x0
#define GPIO_OUTPUT	0x1
#define GPIO_IRQ	0xf
#define GPIO_FUNC(x)	(x)

/* Pull mode */
#define GPIO_PULL_NONE	0x0  //Disables Pull-up/Pull-down
#define GPIO_PULL_DOWN	0x1  //Enables Pull-down
#define GPIO_PULL_UP	0x3  //Enables Pull-up              
						   
/* Drive Strength level */
#define GPIO_DRV_1X	0x0			//0x0 = 1x
#define GPIO_DRV_2X	0x2         //0x2 = 2x
#define GPIO_DRV_3X	0x1         //0x1 = 3x
#define GPIO_DRV_4X	0x3         //0x3 = 4x
#define GPIO_DRV_FAST	0x0     
#define GPIO_DRV_SLOW	0x1

typedef enum GPIO_Id	// Address Offset                                                         
{                                                                            
	// GPIO Base Addr = 0x1100_0000      
	// GPIO_Right_Top_Block							
	// offset : 0x0                                                
	eGPIO_K0 = 0x040,    		                                     
	eGPIO_K1 = 0x060,    		                                     
	eGPIO_K2 = 0x080,    		                                     
	eGPIO_K3 = 0x0A0,    		                                     
	eGPIO_L0 = 0x0C0,    		                                     
	eGPIO_L1 = 0x0E0,    		                                     
	eGPIO_L2 = 0x100,    		                                     
	eGPIO_Y0 = 0x120,    	                                     
	eGPIO_Y1 = 0x140,    	                                     
	eGPIO_Y2 = 0x160,    	                                     
	eGPIO_Y3 = 0x180,    	                                     
	eGPIO_Y4 = 0x1A0,    	                                     
	eGPIO_Y5 = 0x1C0,    	                                     
	eGPIO_Y6 = 0x1E0,    	                                        		                                     
   	eETC0    = 0x208,    		                                     
	eETC6    = 0x228,    
	eGPIO_M0 = 0x260,    		                                     
	eGPIO_M1 = 0x280,    		                                     
	eGPIO_M2 = 0x2A0,    		                                     
	eGPIO_M3 = 0x2C0, 	
	eGPIO_M4 = 0x2E0, 
	eGPIO_X0 = 0xC00,    		                                     
	eGPIO_X1 = 0xC20,    		                                     
	eGPIO_X2 = 0xC40,    		                                     
	eGPIO_X3 = 0xC60, 
	                                                                     
	// GPIO_Left_Bottom_Block	
	// offset : 0x40_0000
	eGPIO_A0 = 0x400000,    		             
	eGPIO_A1 = 0x400020,    	                                     
	eGPIO_B  = 0x400040,    	                                     
	eGPIO_C0 = 0x400060,    	                                     
	eGPIO_C1 = 0x400080,    	                                     
	eGPIO_D0 = 0x4000A0,    	                                     
	eGPIO_D1 = 0x4000C0,    	                                     
	eGPIO_E0 = 0x4000E0,    	                                     
	eGPIO_E1 = 0x400100,    	                                     
	eGPIO_E2 = 0x400120,    	                                     
	eGPIO_E3 = 0x400140,    	                                     
	eGPIO_E4 = 0x400160,    	                                     
	eGPIO_F0 = 0x400180,    	                                     
	eGPIO_F1 = 0x4001A0,    	                                     
	eGPIO_F2 = 0x4001C0,    	                                     
	eGPIO_F3 = 0x4001E0,    	                                        	                                     
	eETC1    = 0x400228,
	eGPIO_J0 = 0x400240,	
	eGPIO_J1 = 0x400260
	                                                                     
}                                                                            
GPIO_eId;     

struct tiny4412_gpio_bank {
	unsigned int	con;
	unsigned int	dat;
	unsigned int	pull;
	unsigned int	drv;
	unsigned int	pdn_con;
	unsigned int	pdn_pull;
	unsigned char	res1[8];
};

/* functions */
void gpio_cfg_pin(struct tiny4412_gpio_bank *bank, int gpio, int cfg);
void gpio_direction_output(struct tiny4412_gpio_bank *bank, int gpio, int en);
void gpio_direction_input(struct tiny4412_gpio_bank *bank, int gpio);
void gpio_set_value(struct tiny4412_gpio_bank *bank, int gpio, int en);
unsigned int gpio_get_value(struct tiny4412_gpio_bank *bank, int gpio);
void gpio_set_pull(struct tiny4412_gpio_bank *bank, int gpio, int mode);
void gpio_set_drv(struct tiny4412_gpio_bank *bank, int gpio, int mode);
void gpio_set_rate(struct tiny4412_gpio_bank *bank, int gpio, int mode);

#endif /*__TINY4412_GPIO_H*/
