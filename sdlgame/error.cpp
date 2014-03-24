#include "error.h"

#include "SDL_syswm.h"
//#ifdef __WIN32__

#include "windows.h"


void showerror(HWND h,LPCWSTR s){
	MessageBoxW(h,s,L"Error",MB_OK|MB_ICONERROR|MB_APPLMODAL);
	exit(1);
}

HWND get_sdl_window_handle(){
	SDL_SysWMinfo SysInfo; //Will hold our Window information
	SDL_VERSION(&SysInfo.version); //Set SDL version
 
	if(SDL_GetWMInfo(&SysInfo) <= 0) {
		printf("%s : %d\n", SDL_GetError(), SysInfo.window);
		return NULL;
	}
 
	HWND WindowHandle = SysInfo.window; //There it is, Win32 handle
	return WindowHandle;
}



//#else
//unix stuff
//#endif