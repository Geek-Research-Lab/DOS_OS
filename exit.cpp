#include<windows.h>
#include<dir.h>
#include<alloc.h>
#include<stdio.h>
#include<ctype.h>
int PASCAL WinMain(HANDLE hInstance,HANDLE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow);
BOOL FAR PASCAL ExitProc(HWND hwnd,WORD message,WORD WParam,LONG lParam);
#define EXIT_BUTTON 101
HANDLE hInst;
#pragma warn -par
int PASCAL WinMain(HANDLE hInstance,HANDLE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	FARPROC dlgProc;
	int r=0;
	hInst=hInstance;
	dlgProc=MakeProcInstance((FARPROC)ExitProc,hInst);
	r=DialogBox(hInst,"Exit",NULL,dlgProc);
	FreeProcInstance(dlgProc);
	return(r);
}
BOOL FAR PASCAL ExitProc(HWND hwnd,WORD message,WORD WParam,LONG lParam)
{
	RECT r;
	int x,y;
	switch(message)
	{
	case WM_INITDIALOG:
		GetWindowRect(hwnd,&r);
		x=GetSystemMetrics(SM_CXSCREEN) - (r.right-r.left);
		y=GetSystemMetrics(SM_CYSCREEN) - (r.bottom-r.top);
		SetWindowPos(hwnd,NULL,x,y,0,0,SWP_NOSIZE);
		return(FALSE);
	case WM_COMMAND:
		if(WParam==EXIT_BUTTON)
			ExitWindows(0L,0);
		return(TRUE);
	}
	return(FALSE);
}
