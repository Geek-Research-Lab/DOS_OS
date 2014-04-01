#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<alloc.h>
#include<dir.h>
/*define*/
#define MINMAZESIZE 16
#define CELLSIZE 8
#define PATHPEN 3
/*constants for file menu*/
#define FILE_ABOUT 103
#define FILE_EXIT 104

#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
/*data structures*/
typedef struct STACK1REC
{
	unsigned char index_1;
	struct STACK1REC *next_ptr;
}
*STACK1RECPTR;
typedef struct STACK2REC
{
	unsigned char index_1;
	unsigned char index_2;
	struct STACK2REC *next_ptr;
}
*STACK2RECPTR;
typedef struct
{
	int delta_x[96];
	int delta_y[96];
	int fatal_error;
	int magnitude_delta_x;
	int magnitude_delta_y;
	int num_columns;
	int num_rows;
	int r_n[18];
	int twice_magnitude_delta_x;
	int twice_magnitude_delta_y;
	int x_max;
	int y_max;
	DWORD passage;
	DWORD path;
	DWORD wall;
	HPEN solve_pen;
	HPEN passage_pen;
	HPEN path_pen;
	HPEN wall_pen;
	HBRUSH passage_brush;
	HBRUSH path_brush;
}
MAZE;
/*Drawing a fucking line*/
#define Line(hdc,l,t,r,b)
{
	MoveTo(hdc,l,t);
	LineTo(hdc,r,b);
}
#define Wait() {
for(int n=0;n<10000;++n);
}
#define Wait()
{
	for(long n=0;n<10000L;++n);
}
/*prototypes*/
long FAR PASCAL WndProc(HWND,unsigned int,unsigned int,long);
DWORD FAR PASCAL AboutDlgProc(HWND hwnd,WORD message,WORD wParam, LONG lparam);
void DoMaze(HDC hdc,RECT *rect);
void InitializeMaze(MAZE *maze, HDC hdc, RECT *rect);
void GenerateMaze(MAZE *maze, HDC hdc, RECT *rect);
char szAppName[]="Maze";
HANDLE hInst;
/* Main part...:| carefully coding because this is where maze main thingy is done.. =P */
MAZE maze;
#pragma warn -par
int PASCAL WinMain(HANDLE hInstance,HANDLE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	RECT rect;
	HWND hwnd;
	MSG msg;
	HDC hdc;
	WNDCLASS wndclass;
	if(!hPrevInstance)
	{
		wndclass.style=CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc=WndProc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance=hInstance;
		wndclass.hIcon=LoadIcon(hInstance,szAppName);
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName=szAppName;
		wndclass.lpszClassName=szAppName;
		RegisterClass(&wndclass);
	}
	hInst=hInstance;
	/*create a window*/
	hwnd=CreateWindow(szAppName,"Maze",WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,100,100,300,300,NULL,NULL,hInstance,NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	for(;;)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			hdc=GetDC(hwnd);
			GetClientRect(hwnd,&rect);
			DoMaze(hc,&rect);
			ReleaseDC(hwnd,hdc);
		}
	}
	return(msg,wParam);
}
long FAR PASCAL WndProc(HWND hwnd,unsigned int message,unsigned int wParam,long lparam)
{
	FARPROC lpfnDlgProc;
	PAINTSTRUCT ps;
	switch(message)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case FILE_ABOUT:
			if((lpfnDlgProc=MakeProcInstance((FARPROC) AboutDlgProc,hInst))!=NULL)
			{
				DialogBox(hInst,"AboutBox",hwnd,lpfnDlgProc);
			}
			return(0);
		case FILE_EXIT:
			if(MessageBox(hwnd,"Is the mazes enough or not?","Exit...",MB_OKCANCEL | MB_ICONFIRMATION)==IDOK)
				PostQuitMessage(0);
			break;
		}
		break;
	case WM_SIZE:
	case WM_ACTIVATE:
		maze,fatal_error=TRUE;
		return(0);
	case WM_CREATE:
		maze.wall=RGB(0,255,0);
		maze.passage=RGB(0,0,0);
		maze.path=RGB(0,128,0);
		maze.wall_pen=CreatePen(PS_SOLID,1,maze,wall);
		maze.passage_pen=CreatePen(PS_SOLID,1,maze,passage);
		maze.path_pen=CreatePen(PS_SOLID,PATHPEN,maze,path);
		maze.solve_pen=CreatePen(PS_SOLID,PATHPEN,maze,passage);
		maze.passage_brush=CreateSolidBrush(maze,passage);
		maze.path_brush=CreateSolidBrush(maze,path);
		return(0);
	case WM_PAINT:
		BeginPaint(hwnd,&ps);
		maze.fatal_error=TRUE;
		EndPaint(hwnd,&ps);
		return(0);
	case WM_DESTROY:
		DeleteObject(maze.wall_pen);
		DeleteObject(maze.passage_pen);
			DeleteObject(maze.path_pen);
			DeleteObject(maze.solve_pen);
			DeleteObject(maze.passage_brush);
			DeleteObject(maze.path_brush);
			PostQuitMessage(0);
			break;
	}
	return(DefWindowProc(hwnd,message,wParam,lparam));
}
/*handling messages to the "about" dialog box*/
DWORD FAR PASCAL AboutDlgProc(HWND hwnd,WORD message,WORD message,WORD wParam, LONG lparam)
{
	POINT point;
	switch(message)
	{
	case WM_INITDIALOG:
		return(TRUE);
	case WM_CTLCOLOR:
		if(HIWORD(lparam)==CTLCOLOR_static || HIWORD(lparam)==CTLCOLOR_DLG)
		{
			SetBkColor(wParam,RGB(192,192,192));
			SetTextColor(wParam,RGB(0,0,0));
			ClientToScreen(hwnd,&point);
			UnrealizeObject(GetStockObject(LTGRAY_BRUSH));
			SetBrushOrg(wParam,point.x,point.y);
			return((DWORD) GetStockObject(LTGRAY_BRUSH));
		}
		if(HIWORD(lparam)!==CTLCOLOR_BTN)
		{
			SetBkColor(wParam,RGB(192,192,192));
			SetTextColor(wParam,RGB(0,0,0));
			ClientToScreen(hwnd,&point);
			UnrealizeObject(GetStockObject(BLACK_BRUSH));
			SetBrushOrg(wParam,point.x,point.y);
			return((DWORD) GetStockObject(BLACK_BRUSH));
		}
		break;
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			EndDialog(hwnd,wParam);
			return(TRUE);
		}
		break;
	}
	return(FALSE);
}
void DoMaze(HDC hdc,RECT *rect)
{
	maze.fatal_error=FALSE;
	InitializeMaze(&maze,hdc,rect);
	if(!maze.fatal_error)
		GenerateMaze(&maze,hdc,rect);
	if(!maze.fatal_error)
		SolveMaze(&maze,hdc,rect);
}
void InitializeMaze(MAZE *maze, HDC hdc, RECT *rect)
{
	int delta_index_1a;
	int delta_index_1b;
	int delta_index_1c;
	int delta_index_1d;
	int delta_index_2;
	int max_num_columns;
	int max_num_rows;
	int max_x;
	int max_y;
	int r_n_index_1;
	int r_n_index_2;
	char seed(256);
	int tem_int;
	max_x=rect->right - rect->left;
	max_y=rect->bottom - rect->top;
	max_num_columns=max_x/CELLSIZE;
	max_num_rows=max_y/CELLSIZE;
	randomize();
	maze->num_rows=max_num_rows;
	maze->num_columns=max_num_columns;
	sprintf(seed,"%u",random(32767));
	for(r_n_index_1=0;r_n_index_1 < 8 && seed[r_n_index_1]!=0;r_n_index_1++)
	{
		tem_int=seed[r_n_index_1];
		while(tem_int >= 29)
			tem_int-=29;
		maze->r_n[r_n_index_1]=tem_int;
	}
	r_n_index_2=7;
	while(r_n_index_1 > 0)
	{
		r_n_index_1--;
		maze->r_n[r_n_index_2]=maze->r_n[r_n_index_1];
		r_n_index_2--;
	}
	while(r_n_index_2 >= 0)
	{
		maze->r_n[r_n_index_2]=19;
		r_n_index_2--;
	}
	maze->magnitude_delta_x=max_x/maze->num_columns/2;
	maze->twice_magnitude_delta_x=maze->magnitude_delta_x + maze->magnitude_delta_x;
	maze->magnitude_delta_y=max_y/maze->num_columns/2;
	maze->twice_magnitude_delta_y=maze->magnitude_delta_y + maze->magnitude_delta_y;
	maze->x_max=maze->twice_magnitude_delta_x*maze->num_columns;
	maze->y_max=maze->twice_magnitude_delta_y*maze->num_rows;
	maze->delta_x[0]=maze->magnitude_delta_x;
	maze->delta_y[24]=maze->magnitude_delta_y;
	maze->delta_x[48]=maze->magnitude_delta_x;
	maze->delta_y[72]=maze->magnitude_delta_y;
	maze->delta_y[0]=0;
	maze->delta_x[24]=0;
	maze->delta_y[48]=0;
	maze->delta_x[72]=0;
	delta_index_2=-1;
	for(delta_index_1a=0;delta_index_1a < 4;delta_index_1a++)
	{
		for(delta_index_1b=0;delta_index_1b < 4;delta_index_1b++)
		{
		if(delta_index_1a != delta_index_1b)
		{
			for(delta_index_1c=0;delta_index_1c < 4;delta_index_1c++)
			{
				if((delta_index_la != delta_index_1c) && (delta_index_1b != delta_index_1c))
				{
					for(delta_index_1d=0;delta_index_1d < 4;delta_index_1d++)
					{
						if((delta_index_1a != delta_index_1d) && (delta_index_1b != delta_index_1d) && (delta_index_1c != delta_index_1d))
						{
							delta_index_2=delta_index_2+1;
							maze->delta_x[(24*delta_index_1a+delta_index_2)]=maze->delta_x[0];
							maze->delta_y[(24*delta_index_1a+delta_index_2)]=maze->delta_y[0];
							maze->delta_x[(24*delta_index_1b+delta_index_2)]=maze->delta_x[24];
							maze->delta_y[(24*delta_index_1b+delta_index_2)]=maze->delta_y[24];
							maze->delta_x[(24*delta_index_1c+delta_index_2)]=maze->delta_x[48];
							maze->delta_y[(24*delta_index_1c+delta_index_2)]=maze->delta_y[48];
							maze->delta_x[(24*delta_index_1d+delta_index_2)]=maze->delta_x[72];
							maze->delta_y[(24*delta_index_1d+delta_index_2)]=maze->delta_y[72];
						}
					}
				}
			}
		}
		}
	}
}
void GenerateMaze(MAZE *maze, HDC hdc, RECT *rect)
{
	int finished;
	int delta_index_1;
	int delta_index_2;
	int digit;
	int digit_num;
	int recurse;
	int r_n_index_1;
	int r_n_index_2;
	STACK2RECPTR stack_head=NULL;
	STACJ2RECPTR stack_ptr=NULL;
	int sum;
	int tem_int;
	int x;
	int x_next;
	int x_out;
	int y;
	int y_next;
	int y_out;
	SelectObject(hdc,maze->passage_pen);
	SelectObject(hdc,maze->passage_brush);
	Rectangle(hdc,rect->left,rect->top,rect->right,rect->bottom);
	SelectObject(hdc,maze->path_pen);
	SelectObject(hdc,maze->path_brush);
	Rectangle(hdc,0,0,maze->x_max,maze->y_max);
	if(maze->path!=maze->wall)
	{
		SelectObject(hdc,maze->wall_pen);
		x_out=0;
		while(x_cut <= maze->x_max)
		{
			Line(hdc,x_out,0,x_out,maze->y_max);
		}
		y_out=0;
		while(y_out <= maze->y_max)
		{
			Line(hdc,0,y_out,maze->x_max,y_out);
			y_out+=(maze->twice_magnitude_delta_y);
		}
	}
	sum=0;
	for(digit_num=1;digit_num <= 3;digit_num++)
	{
		digit=maze->r_n[0];
		r_n_index_1=0;
		for(r_n_index_2=1;r_n_index_2 < 8;r_n_index_2++)
		{
			tem_int=maze->r_n[r_n_index_2];
			maze->r_n[r_n_index_1]=tem_int;
			r_n_index_1++;
			digit+=tem_int;
			if(digit >= 29)
				digit-=29;
		}
		maze->r_n[7]=digit;
		sum=29*sum+digit;
	}
	x=(2*(sum%(maze->num_columns))+1)
		*maze=magnitude_delta_x;
	sum=0;
	for(digit_num=1;digit_num <= 3;digit_num++)
	{
		digit=maze->r_n[0];
		r_n_index_1=0;
		for(r_n_index_2=1;r_n_index_2 < 8;r_n_index_2++)
		{
			tem_int=maze->r_n(r_n_index_2);
			maze->r_n[r_n_index_1]=tem_int;
			r_n_index_1++;
			digit+=tem_int;
			if(digit >= 29)
				digit-=29;
		}
		maze->r_n[7]=digit;
		sum=29*sum+digit;
	}
	y=(2*(sum%(maze->num_rows))+1)
		*maze=magnitude_delta_y;
	SelectObject(hdc,maze->passage_brush);
	SelectObject(hdc,maze->passage_pen);
	finished=FALSE;
	recurse=TRUE;
	stack_head=NULL;
	while(!finished && !maze->fatal_error)
	{
		Yield();
		if(recurse)
		{
			Rectangle(hdc,x-maze->magnitude_delta_x+1,y-maze->magnitude_delta_y+1,x+maze->magnitude_delta_x,y+maze->magnitude_delta_y);
			delta_index_1=0;
			do
			{
				delta_index_2=maze->r_n[0];
				r_n_index_1=0;
				for(r_n_index_2=1;r_n_index_2 < 8;r_n_index_2++)
				{
					tem_int=maze->r_n[r_n_index_2];
					maze->r_n[r_n_index_1]=tem_int;
					r_n_index_1++;
					delta_index_2+=tem_int;
					if(delta_index_2 >= 29)
						delta_index_2-=29;
				}
				maze->r_n[7]=delta_index_2;
			}
			while(delta_index_2 >= 24);
			recurse=FALSE;
		}
		while((delta_index_1 < 4) && !recurse && !maze->fatal_error)
		{
			x_next=x+2*maze->delta_x[24*delta_index_1+delta_index_2];
			if((x_next <= 0) || (x_next >= maze->x_max))
				delta_index_1++;
			else
			{
				y_next=y+2*maze->delta_y[24*delta_index_1+delta_index_2];
				if((y_next <= 0) || (y_next >= maze->y_max))
					delta_index_1++;
				else if(GetPixel(hdc,x_next,y_next)==maze->path)
				{
					if(x==x_next)
					{
						y_out=(y+y_next)/2;
						Line(hdc,x-maze->magnitude_delta_x+1,y_out,x+maze->magnitude_delta_x,y_out);
					}
					else
					{
						x_out=(x+x_next)/2;
						Line(hdc,x_out,y-maze->magnitude_delta_y+1,x_out,y+maze->magnitude_delta_y);
					}
					x=x_next;
					y=y_next;
					if((stack_ptr=(struct STACK2REC *) malloc((unsigned) sizeof(struct STACK2REC)))==NULL)
					maze->fatal_error=TRUE;
					else
					{
						stack_ptr->next_ptr=stack_head;
						stack_head=stack_ptr;
						stack_head->index_1=(unsigned char)
							delta_index_1;
						stack_head->index_2=(unsigned char)
							delta_index_2;
						recurse=TRUE;
					}
				}
				else
					delta_index_1++;
			}
		}
		if((!recurse) && (!maze->fatal_error))
		{
			delta_index_1=(int) stack_head->index_1;
			delta_index_2=(int) stack_head->index_2;
			stack_ptr=stack_head;
			stack_head=stack_head->next_ptr;
			free((char *)stack ptr);
			stack_ptr=NULL;
			if(stack_head==NULL)
				finished=TRUE;
			else
			{
				x-=(2*maze->delta_x[24*delta_index_1+delta_index_2]);
				y-=(2*maze->delta_y[24*delta_index_1+delta_index_2]);
			}
		}
	}
	if(maze->fatal_error)
	{
		Line(hdc,1,0,(maze->twice_magnitude_delta_x)-1,0);
		Line(hdc,maze->x_max-(maze->twice_magnitude_delta_x)+1,maze->y_max,maze->x_max,maze->y_max);
	}
	if(stack_ptr!=NULL)
		free(stack_ptr);
	}
	void SolveMaze(MAZE *maze, HDC hdc)
	{
		int finished;
		unsigned char delta_index_1;
		int recurse;
		STACK1RECPTR stack_head=NULL;
		STACK1RECPTR stack_ptr=NULL;
		int x;
		int x_next;
		int y;
		int y_next;
		int stack_size=0;
		x=maze->magnitude_delta_x;
		y=maze->magnitude_delta_y;
		y_next=y+(maze->magnitude_delta_y);
		SelectObject(hdc,maze->path_pen);
		Line(hdc,x,0,x,y);
		finished=FALSE;
		recurse=TRUE;
		stack_head=NULL;
		while(!finished && !maze->fatal_error)
		{
			Wait();
			Yield();
			if(recurse)
			{
				delta_index_1=0;
				recurse=FALSE;
			}
			while((delta_index_1 < 4) && !finished && !recurse && !maze->fatal_error)
			{
				x_next=x+maze->delta_x[24*delta_index_1];
				y_next=y+maze->delta_y[24*delta_index_1];
				if(GetPixel(hdc,x_next,y_next)==maze->passage)
				{
					x_next+=maze->delta_x[24*delta_index_1];
					y_next+=maze->delta_y[24*delta_index_1];
					if(y_next <= maze->y_max)
					{
						SelectObject(hdc,maze->path_pen);
						Line(hdc,x,y,x_next,y_next);
						x=x_next;
						y=y_next;
						if((stack_ptr=(struct STACK1REC *)malloc(sizeof(struct STACK1REC)))==NULL)
							maze->fatal_error=TRUE;
						else
						{
							stack_ptr->next_ptr=stack_head;
							stack_head=stack_ptr;
							stack_head->index_1=delta_index_1;
							recurse=TRUE;
							++stack_size;
						}
					}
					else
						finished=TRUE;
				}
				else
					delta_index_1++;
			}
			if(delta_index_1 >= 4 && !maze->fatal_error)
			{
				x_next=x;
				y_next=y;
				delta_index_1=stack_head->index_1;
				stack_ptr=stack_head;
				stack_head=stack_head->next_ptr;
				free((char *)stack ptr);
				stack_ptr=NULL;
				--stack_size;
				x-=(2*maze->delta_x[24*delta_index_1]);
				y-=(2*maze->delta_y[24*delta_index_1]);
				SelectObject(hdc,maze->solve_pen);
				Line(hdc,x,y,x_next,y_next);
				delta_index_1++;
			}
		}
		if(!maze->fatal_error)
		{
			SelectObject(hdc,maze->path_pen);
			Line(hdc,x,y,x,maze->y_max);
		}
		/*free any unused stack elements*/
		while(stack_size--)
		{
			stack_ptr=stack_head;
			stack_head=stack_head->next_ptr;
			free((char *)stack ptr);
		}
	}

