/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       pl_error.h
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-21
*   
***********************************************************************/
#ifndef _PL_ERROR_H
#define _PL_ERROR_H

#include <errno.h>

typedef enum {
	/* basic errors */
	CUE_SUCCESS           = 0,  /**< No error condition. */
	CUE_NOMEMORY          = 1,  /**< Memory allocation failed. */

	/* Test Registry Level Errors */
	CUE_NOREGISTRY        = 10,  /**< Test registry not initialized. */
	CUE_REGISTRY_EXISTS   = 11,  /**< Attempt to CU_set_registry() without CU_cleanup_registry(). */

	/* Test Suite Level Errors */
	CUE_NOSUITE           = 20,  /**< A required CU_pSuite pointer was NULL. */
	CUE_NO_SUITENAME      = 21,  /**< Required CU_Suite name not provided. */
	CUE_SINIT_FAILED      = 22,  /**< Suite initialization failed. */
	CUE_SCLEAN_FAILED     = 23,  /**< Suite cleanup failed. */
	CUE_DUP_SUITE         = 24,  /**< Duplicate suite name not allowed. */
	CUE_SUITE_INACTIVE    = 25,  /**< Test run initiated for an inactive suite. */

	/* Test Case Level Errors */
	CUE_NOTEST            = 30,  /**< A required CU_pTest or CU_TestFunc pointer was NULL. */
	CUE_NO_TESTNAME       = 31,  /**< Required CU_Test name not provided. */
	CUE_DUP_TEST          = 32,  /**< Duplicate test case name not allowed. */
	CUE_TEST_NOT_IN_SUITE = 33,  /**< Test not registered in specified suite. */
	CUE_TEST_INACTIVE     = 34,  /**< Test run initiated for an inactive test. */

	/* File handling errors */
	CUE_FOPEN_FAILED      = 40,  /**< An error occurred opening a file. */
	CUE_FCLOSE_FAILED     = 41,  /**< An error occurred closing a file. */
	CUE_BAD_FILENAME      = 42,  /**< A bad filename was requested (NULL, empty, nonexistent, etc.). */
	CUE_WRITE_ERROR       = 43   /**< An error occurred during a write to a file. */
} CU_ErrorCode;

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
#endif // _PL_ERROR_H
