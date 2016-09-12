/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       config.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-08-18
*   
***********************************************************************/
//#include "config.h"
#include <stdio.h>
#include <stdlib.h>

static int hash(char *string)
{
	int hash = 0;

	if(!string) 
	{
		return 0;
	}

	while(*string)
	{
		printf("%d\n", *string);
		hash += *string;
		string++;
	}

	return hash;
}

CONFIG_ITEM_VALUE_TYPE_E config_get_item(char *main_key, char *sub_key, CONFIG_ITEM_U *item)
{
	int     main_hash, sub_hash;
	script_main_key_t   *mainkey = g_script;

	if(!main_key || !sub_key || !item || !g_script) 
	{
		return CONFIG_ITEM_VALUE_TYPE_INVALID;
	}

	main_hash = hash(main_key);
	sub_hash = hash(sub_key);

	/* try to look for the main key from main key list */
	while(mainkey) 
	{
		if((mainkey->hash == main_hash) && !strcmp(mainkey->name, main_key)) 
		{
			/* find the main key */
			script_sub_key_t    *subkey = mainkey->subkey;
			while(subkey) 
			{
				if((subkey->hash == sub_hash) && !strcmp(subkey->name, sub_key)) 
				{
					/* find the sub key */
					*item = *subkey->value;
					return subkey->type;
				}
				subkey = subkey->next;
			}

			/* no sub key defined under the main key */
			return SCIRPT_ITEM_VALUE_TYPE_INVALID;
		}
		mainkey = mainkey->next;
	}

	return SCIRPT_ITEM_VALUE_TYPE_INVALID;
}

/**
 * @brief    config_init 
 *
 * @return    
 */
int config_init(void)
{

	/* alloc memory for main keys */
	g_script = CONFIG_MALLOC(script_hdr->main_cnt*sizeof(script_main_key_t));
	if (!g_script) 
	{
		printk(KERN_ERR "try to alloc memory for main keys!\n");
		return -1;
	}
}


int main(int argc, char** argv)
{

	printf("hash=%d\n", hash("csi0"));

	return 0;
}



