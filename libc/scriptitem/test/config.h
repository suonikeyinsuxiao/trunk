
#define ITEM_NAME_SIZE_MAX    (32)

typedef enum 
{          
	e_CONFIG_ITEM_VALUE_TYPE_INVALID = 0,
	e_CONFIG_ITEM_VALUE_TYPE_INT,
	e_CONFIG_ITEM_VALUE_TYPE_STR
}CONFIG_ITEM_VALUE_TYPE_E;

typedef union 
{
	int                 u_nVal;
	char*				u_pcStr;
}CONFIG_ITEM_VALUE_U;

typedef struct 
{
	char 						m_acName[ITEM_NAME_SIZE_MAX];
	CONFIG_ITEM_VALUE_U*		m_puValue;
	CONFIG_ITEM_VALUE_TYPE_E    m_eType;
	int                         m_nHash;
	void*                       m_pNext;
}CONFIG_SUB_KEY_S;

typedef struct 
{
	char 					m_acName[ITEM_NAME_SIZE_MAX];
	CONFIG_SUB_KEY_S*		m_psSubKey;
	CONFIG_ITEM_VALUE_U*	m_puSubKeyVal;
	int                 	m_nHash;
	void*					m_pNext;
}CONFIG_MAIN_KEY_S;






