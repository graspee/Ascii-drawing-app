#ifndef _error_h
#define _error_h


#define __bounds 1


//#ifdef __WIN32__

#include "windows.h"

void showerror(HWND h,LPCWSTR s);
HWND get_sdl_window_handle();

//#else
//unix stuff
//#endif



#endif