/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       test_unit.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-21
*   
***********************************************************************/
#include "test_unit.h"

CU_BOOL test_cunit_assert_impl(CU_BOOL value, const char* condition,
		const char* file, unsigned int line)
{
	test_cunit_add_test();
	if (CU_FALSE == value) 
	{
		test_cunit_add_failure();
		printf(_("\nTEST FAILED: File '%s', Line %d, Condition '%s.'\n"),
				file, line, condition);
	}
	return value;
}
