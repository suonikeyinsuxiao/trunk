/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       test_unit.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-21
*   
***********************************************************************/
#ifndef _TEST_UNIT_H
#define _TEST_UNIT_H
#ifdef __cplusplus
extern "C"
{
#endif

CU_BOOL test_cunit_assert_impl(CU_BOOL value, const char* condition,
			const char* file, unsigned int line);

#ifdef __cplusplus
}
#endif
#endif // _TEST_UNIT_H
