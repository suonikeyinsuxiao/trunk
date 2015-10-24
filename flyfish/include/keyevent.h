#ifndef _KEYEVENT_H
#define _KEYEVENT_H

#include "public.h"



/**
 * function:eventProcess()
 *
 * Description:when a event is reported, do something
 *
 * parameter:
 *		SDL_EVENT* psEvent
 * return:
 */
int eventProcess(SDL_EVENT* psEvent);

#endif
