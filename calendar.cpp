#include<windows.h>
#include<dir.h>
#include<alloc.h>
#include<stdio.h>
#include<dos.h>
#define QUIT 101
#define ABOUT 102
#define STARTYEAR 1583
#define STARTMONTH 0
#define STARTDAY 6
#define ENDYEAR 4046
#define LEFT 50
#define YEARJUMP 10
#define MONTHJUMP 3
#define DAYSPACE 6
int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
DWORD FAR PASCAL CalendarProc(HWND hwnd,WORD message,WORD WParam,LONG lParam);
DWORD FAR PASCAL AboutDlgProc(HWND hwnd,WORD message,WORD WParam,LONG lParam);
int firstday(int year,int month);
void showmonth(HDC hdc,int year,int month,RECT *r);
int isleap(int n);
HANDLE hInst;
int number_days[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};
char months[12][10]={"January","February","March","April","May","June","July","August","September","October","November","December"};
char days[7][4]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
#pragma warn -par
int PASCAL WinMain(HANDLE hInstance,HANDLE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	FARPROC dlgPROC;
	int r=0;
	hInst=hInstance;
	dlgProc=MakeProcInstance((FARPROC)CalendarProc,hInst);
	r=DialogBox(hInst,"Calendar",NULL,dlgProc);
	FreeProcInstance(dlgProc);
	return(r);
}
DWORD FAR PASCAL CalendarProc(HWND hwnd,WORD message,WORD WParam,LONG lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	FARPROC lpfnDlgProc;
	RECT rect;
	POINT point;
	struct date d;
	static int year,month;
	switch(message)
	{
	case WM_INITDIALOG:
		SetClassWord(hwnd,GCW_HICON,(WORD)LoadIcon(hInst,"Calendar"));
		SetScrollRange(hwnd,SB_VERT,STARTYEAR,ENDYEAR,FALSE);
		SetScrollRange(hwnd,SB_HORZ,0,11,FALSE);
		getdate(&d);
		year=.d.da_year;
		month=d.da_mon-1;
		SetScrollPos(hwnd,SB_HORZ,month,TRUE);
		SetScrollPos(hwnd,SB_VERT,year,TRUE);
		InvalidateRect(hwnd,NULL,TRUE);
		break;
	case WM_CTLCOLOR:
		if(HIWORD(lParam)==CTLCOLOR_STATIC||HIWORD(lParam)==CTLCOLOR_DLG)
		{
			SetBkColor(WParam,RGB(192,192,192));
			SetTextColor(WParam,RGB(0,0,0));
			ClientToScreen(hwnd,&point);
			UnrealizeObject(GetStockObject(LTGRAY_BRUSH));
			SetBrushOrg(WParam,point.x,point.y);
			return((DWORD)GetStockObject(LTGRAY_BRUSH));
		}
		if(HIWORD(lParam)==CTLCOLOR_BTN)
		{
			SetBkColor(WParam,RGB(192,192,192));
			SetTextColor(WParam,RGB(0,0,0));
			ClientToScreen(hwnd,&point);
			UnrealizeObject(GetStockObject(BLACK_BRUSH));
			SetBrushOrg(WParam,point.x,point.y);
			return((DWORD)GetStockObject(BLACK_BRUSH));
		}
		break;
	case WM_VSCROLL:
		switch(WParam)
		{
		case SB_LINEUP:
			year-=1;
			break;
		case SB_LINEDOWN:
			year+=1;
			break;
		case SB_PAGEUP:
			year-=YEARJUMP;
			break;
		case SB_PAGEDOWN:
			year+=YEARJUMP;
			break;
		case SB_THUMBPOSITION:
			year=LOWORD(lParam);
			break;
		}
		if(year<STARTYEAR)
			year=STARTYEAR;
		else if(year>=ENDYEAR)
			year=ENDYEAR-1;
		if(year!=GetScrollPos(hwnd,SB_VERT))
		{
			SetScrollPos(hwnd,SB_VERT,year,TRUE);
			InvalidateRect(hwnd,NULL,TRUE);
		}
		break;
	case WM_HSCROLL:
		switch(WParam)
		{
		case SB_LINEUP:
			month-=1;
			break;
			case SB_LINEDOWN:
			month+=1;
			break;
		case SB_PAGEUP:
			month-=YEARJUMP;
			break;
		case SB_PAGEDOWN:
			month+=YEARJUMP;
			break;
		case SB_THUMBPOSITION:
			month=LOWORD(lParam);
			break;
		}
		if(month<0)
			month=0;
		else if(month>=12)
			month=11;
		if(month!=GetScrollPos(hwnd,SB_HORZ))
		{
			SetScrollPos(hwnd,SB_HORZ,month,TRUE);
			InvalidateRect(hwnd,NULL,TRUE);
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);
		GetClientRect(hwnd,&rect);
		showmonth(hdc,year,month,&rect);
		EndPaint(hwnd,&ps);
		break;
	case WM_COMMAND:
		switch(WParam)
		{
		case QUIT:
			PostQuitMessage(0);
			break;
		case ABOUT:
			if((lpfnDlgProc=MakeProcInstance((FARPROC)AboutDlgProc,hInst))!=NULL)
			{
				DialogBox(hInst,"About",hwnd,lpfnDlgProc);
				FreeProcInstance(lpfnDlgProc);
			}
			return(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		}
		return(FALSE);
		}
		int firstday(int year,int month)
		{
			int i,d=0;
			for(i=STARTYEAR;i<year;++i)
			{
				if(isleap(i))
					d+=366;
				else d+=365;
				d%=7;
			}
			for(i=STARTMONTH;i<month;++i)
			{
				d+=number_days[isleap(year)][i];
				d%=7;
			}
			if(d==0)
				d=7;
			return(d-1);
		}
		void showmonth(HDC hdc,int year,int month,RECT *r)
		{
			TEXTMETRIC tm;
			char b[128];
			int i,n,y,cx,cy,day,thisday;
			SetBkColor(hdc,RGB(192,192,192));
			GetTextMetrics(hdc,&tm);
			cx=tm.tmAveCharWidth;
			cy=tm.tmHeight+tm.tmExternalLeading;
			SetTextAlign(hdc,TA_CENTER);
			n=wsprintf((LPSTR)b,(LPSTR)"%s %u",(LPSTR)months[month],year);
			TextOut(hdc,r->left+((r->right-r->left)/2),r->top+cy,b,n);
			thisday=day=firstday(year,month);
			y=r->top+cy*3;
			SetTextAlign(hdc,TA_RIGHT);
			for(i=0;i<7;i++)
				TextOut(hdc,r->left+LEFT+i*(cx*DAYSPACE),y,days[i].3);
			y+=(cy*2);
			for(i=0;i<number_days[isleap(year)][month];++i,++day,++thisday)
			{
				if(thisday>=7)
				{
					y+=cy;
					thisday=0;
				}
				n=wsprintf(b,"%u",i+1);
				TextOut(hdc,r->left+LEFT+thisday*(cx*DAYSPACE),y,b,n);
			}
		}
		int isleap(int n)
		{
			int r=0;
			if(!(n%4))
				r=1;
			if(!(n%100))
			{
				r=0;
				if(!(n%400))
					r=1;
			}
			return(r);
		}
		DWORD FAR PASCALAboutDlgProc(HWND hwnd,WORD message,WORD WParam,LONG lParam)
		{
			POINT point;
			switch(message)
			{
			case WM_INITDIALOG:
				return(TRUE);
			case WM_CTLCOLOR:
				if(HIWORD(lParam)==CTLCOLOR_STATIC || HIWORD(lParam)==CTLCOLOR_DLG)
				{
					SetBkColor(WParam,RGB(192,192,192));
					SetTextColor(WParam,RGB(0,0,0));
					ClientToScreen(hwnd,&point);
					UnrealizeObject(GetStockObject(LTGRAY_BRUSH));
					SetBrushOrg(WParam,point.x,point.y);
					return((DWORD)GetStockObject(LTGRAY_BRUSH));
				}
				if(HIWORD(lParam)==CTLCOLOR_BTN)
				{
					SetBkColor(WParam,RGB(192,192,192));
					SetTextColor(WParam,RGB(0,0,0));
					ClientToScreen(hwnd,&point);
					UnrealizeObject(GetStockObject(BLACK_BRUSH));
					SetBrushOrg(WParam,point.x,point.y);
					return((DWORD)GetStockObject(BLACK_BRUSH));
				}
				break;
			case WM_COMMAND:
				switch(WParam)
				{
				case IDOK:
					EndDialog(hwnd,WParam);
					return(TRUE);
				}
				break;
			}
			return(FALSE);
		}

