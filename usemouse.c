#include "stdio.h"
#include "dos.h"
#include "conio.h"
#include "alloc.h"
#define TEXTCOLOUR(foreground, background) (foreground | (background << 4))
#define WINDOWSLEFT(p) ((int)p[2])
#define WINDOWSTOP(p) ((int)p[3])
#define WINDOWSWIDE(p) ((int)p[0])
#define WINDOWSDEEP(p) ((int)p[1])
typedef struct
{
	int left,right,top,bottom;
}
RECT;
typedef struct
{
	int x,y;
}
POINT;
char *screenPtr(void);
char *GetScreenText(int l, int t, int width, int depth);
char *OpenWindow(int left, int right, int top, int bottom, int attr);
char *screen;
int alive=1;
main()
{
	union REGS r;
	RECT quit, about;
	POINT point;
	screen=screenPtr();
	textattr(TEXTCOLOUR(BLACK,BLUE));
	clrscr();
	textattr(TEXTCOLOUR(BLUE,LIGHTGRAY));
	clreol();
	DrawString(0,0,"Here comes the title of the screen" "Here we go", TEXTCOLOUR(BLUE,LIGHTGRAY));
	HideCursor();
	r.x.ax=0X0000;
	int86(0X33,&r,&r);
	if(!r.x.ax)
	{
		puts("no mouse driver found");
		exit(1);
	}
	r.x.ax=0X000a;
	r.x.bx=0X0000;
	r.x.cx=0Xf000;
	r.x.dx=0X0f04;
	int86(0X33,&r,&r);
	MouseOn();
	MakeButton(60,20,"Quit",&quit, TEXTCOLOUR(BLUE,LIGHTGRAY));
	MakeButton(40,20,"About",&about, TEXTCOLOUR(BLUE,LIGHTGRAY));
	do
	{
		if(MouseDown(&point))
		{
			if(PointInRect(&point,&quit))
			{
				TrackMouse(&quit);
				alive=0;
			}
			else if(PointInRect(&point,&about))
			{
				TrackMouse(&about);
				DoAbout();
			}
			else putchar(7);
		}
	}
	while(alive);
	MouseOff();
	textattr(WHITE);
	clrscr();
	r.x.ax=0X0000;
	int86(0X33,&r,&r);
}
DoAbout()
{
	POINT point;
	RECT ok;
	char *p;
	int alive;
	if((p=OpenWindow(4,4,60,12,TEXTCOLOUR(BLUE,LIGHTGRAY)))!=NULL)
	{
		DrawString(WINDOWSLEFT(p)+2,WINDOWSTOP(p)+1, "demo mode", TEXTCOLOUR(BLUE, LIGHTGRAY));
		DrawString(WINDOWSLEFT(p)+2,WINDOWSTOP(p)+2, "title", TEXTCOLOUR(BLUE,LIGHTGRAY));
		MakeButton(WINDOWSLEFT(p)+WINDOWSWIDE(p)-8,WINDOWSTOP(p)+4,"Ok",&ok,TEXTCOLOUR(BLUE,LIGHTGRAY));
		do
		{
			if(MouseDown(&point))
			{
				if(PointInRect(&point,&ok))
				{
					TrackMouse(&ok);
					alive=0;
				}
				else putchar(7);
			}
		}
		while(alive);
		CloseWindow(p);
	}
	else putchar(7);
}
char *OpenWindow(left,top,right,bottom,attr)
	int left, right, top, bottom, attr;
{
	char *p;
	if((!p=GetScreenText(left,top,right-left,bottom-top))==NULL)
		return(NULL);
	DrawBox(left,top,right,bottom,attr);
	return(p);
}
CloseWindow(p)
	char *p;
{
	PutScreenText(p);
	free(p);
}
TrackMouse(rect)
	RECT *rect;
{
	POINT point;
	char *p;
	int i,j,x,y;
	MouseOff();
	p=(screen+(rect->top*160+rect->left*2))+1;
	x=(rect->right-rect->left)<<1;
	y=rect->bottom-rect->top;
	for(i=0;i<y;++i)
	{
		for(j=0;j<x;j+=2)
			p[j]=(p[j]>>4) | (p[j]<<4);
		p+=160;
	}
	MouseOn();
}
MakeButton(left,top,text,rect,attr)
	int left,top;
char *text;
RECT *rect;
int attr;
{
	int n;
	n=strlen(text);
	SetRect(rect,left,top,left+4+n,top+3);
	DrawBox(left,top,left+4+n,top+3,attr);
	DrawString(left+2,top+1,text,attr);
}
DrawString(x,y,text,attr)
	int x,y;
char *text;
int attr;
{
	unsigned int a, *ip;
	MouseOff();
	ip=(unsigned int *) (screen+(y*160+x*2));
	a=attr<<8;
	while(*text)
		*ip++=*text++ | a;
	MouseOn();
}
DrawBox(left,top,right,bottom,attr)
	int left,top,right,bottom,attr;
{
	unsigned int a,i,j,*ip;
	MouseOff();
	--bottom;
	--right;
	ip=(unsigned int *) (screen+top*160);
	a=attr<<8;
	ip[left]=218 | a;
	for(j=left+1;j<right;++j)
		ip[j]=32 | a;
	ip[right]=179 | a;
}
ip+=80;
ip[left]=192 | a;
for(j=left+1;j<right;++j)
	ip[j]=196 | a;
ip[right]=217 | a;
MouseOn();
}
MouseOn()
{
	union REGS r;
	r.x.ax=0X0001;
	int86(0X33,&r,&r);
}
MouseOff()
{
	union REGS r;
	r.x.ax=0X0002;
	int86(0X33,&r,&r);
}
MouseDown(p)
	POINT *p;
{
	union REGS r;
	r.x.ax=0X0003;
	int86(0X33,&r,&r);
	p->x=r.x.cx>>3;
	p->y=r.x.dx>>3;
	return(r.x.bx & 0X03);
}
HideCursor()
{
	union REGS r;
	r.x.ax=0X0f00;
	int86(0X10,&r,&r);
	r.x.ax=0X0200;
	r.x.dx=0X1a00;
	int86(0X10,&r,&r);
}
PointInRect(p,r)
	POINT *p;
RECT *r;
{
	if(p->x >= r->left && p->x < r->right && p->y >= r->>top && p->y < r->bottom)
		return(1);
	else return(0);
}
char *screenPtr()
{
	union REGS r;
	r.x.ax=0X0f00;
	int86(0X10,&r,&r);
	if(r.h.al==0X07)
		return(MK_FP(0Xb000,r.h.bh*0X1000));
	else return (MK_FP(0Xb800,r.h.bh*0X1000));
}
SetRect(r,left,top,right,bottom) /*set a rectangle */
	RECT *r;
int left,top,right,bottom;
{
	r->left=left;
	r->top=top;
	r->right=right;
	r->bottom=bottom;
}
char *GetScreenText(l,t,width,depth)
	int l,t,width,depth;
{
	char *p, *sp, *dp;
	int i;
	if((p=malloc(4+(width*depth*2)))==NULL)
		return NULL;
	MouseOff();
	p[0]=width;
	p[1]=depth;
	p[2]=1;
	p[3]=t;
	sp=screen+(160*t)+(l<<1);
	dp=p+4;
	width=width<<!;
	for(i=0;i<depth;++i)
	{
		memcpy(dp, sp, width);
		sp+=160;
		dp+=width;
	}
	MouseOn();
	return(p);
}
PutScreenText(p)
	char *p;
{
	char *dp;
	int i,width,depth,l,t;
	MouseOff();
	width=*p++;
	depth=*p++;
	l=*p++;
	t=*p++;
	dp=screen+(160*t)+(l<<1);
	width=width<<1;
	for(i=0;i<depth;++i)
	{
		memcpy(dp,p,width);
		p+=width;
		dp+=160;
	}
	MouseOn();
}
