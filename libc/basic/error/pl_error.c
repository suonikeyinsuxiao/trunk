/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       pl_error.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-21
*   
***********************************************************************/
#include "pl_error.h"

#undef _
#define _(String) (String)
#undef N_
#define N_(String) String

char* get_error_desc(CU_ErrorCode iError)
{
	int iMaxIndex;

	static const char* ErrorDescription[] = {
		N_("No Error."),                             /* CUE_SUCCESS - 0 */
		N_("Memory allocation failed."),            /* CUE_NOMEMORY - 1 */
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		N_("Test registry does not exist."),          /* CUE_NOREGISTRY - 10 */
		N_("Registry already exists."),               /* CUE_REGISTRY_EXISTS - 11 */
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		N_("NULL suite not allowed."),                /* CUE_NOSUITE - 20 */
		N_("Suite name cannot be NULL."),             /* CUE_NO_SUITENAME - 21 */
		N_("Suite initialization function failed."),  /* CUE_SINIT_FAILED - 22 */
		N_("Suite cleanup function failed."),         /* CUE_SCLEAN_FAILED - 23 */
		N_("Suite having name already registered."),  /* CUE_DUP_SUITE - 24 */
		N_("Requested suite is not active."),         /* CUE_SUITE_INACTIVE - 25 */
		"",
		"",
		"",
		"",
		N_("NULL test or test function not allowed."),/* CUE_NOTEST - 30 */
		N_("Test name cannot be NULL."),              /* CUE_NO_TESTNAME - 31 */
		N_("Test having this name already in suite."),/* CUE_DUP_TEST - 32 */
		N_("Test not registered in specified suite."),/* CUE_TEST_NOT_IN_SUITE - 33 */
		N_("Requested test is not active"),           /* CUE_TEST_INACTIVE - 34 */
		"",
		"",
		"",
		"",
		"",
		N_("Error opening file."),                    /* CUE_FOPEN_FAILED - 40 */
		N_("Error closing file."),                    /* CUE_FCLOSE_FAILED - 41 */
		N_("Bad file name."),                         /* CUE_BAD_FILENAME - 42 */
		N_("Error during write to file."),            /* CUE_WRITE_ERROR - 43 */
		N_("Undefined Error")
	};

	iMaxIndex = (int)(sizeof(ErrorDescription)/sizeof(char *) - 1);
	printf("iMaxIndex=%d\n", iMaxIndex);
	if ((int)iError < 0) {
		return (char*)_(ErrorDescription[iMaxIndex]);
	}
	else if ((int)iError > iMaxIndex) {
		return (char*)_(ErrorDescription[iMaxIndex]);
	}
	else {
		return (char*)_(ErrorDescription[(int)iError]);
	}
}
