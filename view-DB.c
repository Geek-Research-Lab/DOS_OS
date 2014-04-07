#include "stdio.h"
#include "fcntl.h"
#include "io.h"
#include "math.h"
#include "conio.h"
#include "dos.h"
#include <sys/stat.h>
#define SCREENWIDE 80
#define SCREENDEEP 25
#define TEXTCOLOUR(forground.background) (foreground | (background<<4))
#define BACKGROUND TEXTCOLOUR(WHITE,BLUE)
#define BANNER TEXTCOLOUR(BLACK,RED)
#define TEXT TEXTCOLOUR(WHITE,BLUE)
#define HOME 0X4700
#define CURSOR_UP 0X4800
#define END 0X4f00
#define CURSOR_DOWN 0X5000
#define PG_UP 0X4900
#define PG_DOWN 0X5100
#define LEFT 2
#define TOP 2
typedef struct
{
	char manufacturer[25];
	char model[25];
	unsigned int year;
	double displacement;
	long quantity;
}
RECORD;
typedef struct
{
	char sig[8];
	unsigned long count;
}
HEADER;
#define DATABASE_SIG "DATABASE"
char *screenPtr();
char *screen;
main(argc,argv[])
	int argc;
char *argv[];
{
	HEADER header;
	RECORD record;
	long page=0L, oldpage=-1L;
	int c,fh;
	screen=screenPtr();
	if(argc<=1)
		error("Arguments: path to a database");
	if((fh=open(argv[1],O_ROONLY | O_BINARY,S_IREAD))==NULL)
		error("unable to open database");
	if(!readheader(fh,&header))
		error("unable to read header");
	do
	{
		if(page!=oldpage)
			showbanner();
		oldpage=page;
		if(!readrecord(fh,&record,page))
			DrawString(LEFT,TOP,"error reading record",TEXT);
		else
			showrecord(&record,page,LEFT,TOP);
		switch(c=GetKey())
		{
		case HOME:
			page=0L;
			break;
		case END:
			page=header.count-1L;
			break;
		case CURSOR_UP:
			if(page>0L)
				--page;
			break;
		case CURSOR_DOWN:
			if((page+1L)<=(header.count-1L))
				++page;
			break;
		case PG_UP:
			if((page-10L)>0L)
				page-=10L;
			else page=0L;
			break;
		case PG_DOWN:
			if((page+10L)<=(header.count-1L))
				page+=10L;
			else page=header.count-1L;
			break;
		}
	}
	while(c!=27);
	close(fh);
}
showrecord(r,page,x,y)
	RECORD *r;
long page;
unsigned int x,y;
{
	char b[129];
	sprintf(b,"RECORD:%lu",page+1L);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"MODEL:%s",r->model);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"YEAR:%u",r->year);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"DISPLACEMENT:%g litres",r->displacement);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"QUANTITY:%lu", r->quantity);
	DrawString(x,y++,b,TEXT);
}
readrecord(fh,r,n)
	int fh;
RECORD *r;
unsigned long n;
{
	lseek(fh,(long)sizeof(HEADER)+n*(long)sizeof(RECORD),SEEK_SET);
	if(read(fh,(char *)r,sizeof(RECORD)==sizeof(RECORD))
		return(1);
	else
		return(0);
}
readheader(fh,h)
	int fh;
HEADER *h;
{
	lseek(fh,0L,SEEK_SET);
	if(read(fh,(char *)h,sizeof(HEADER))==sizeof(HEADER))
	{
		if(memcmp(h->sig,DATABASE_SIG,8))
			return(0);
		else
			return(1);
	}
	else
		return(0);
}
showbanner()
{
	ClearScreen(BACKGROUND);
	ClearLine(0,BANNER);
	HideCursor();
	DrawString(LEFT,0,"Database viewer",BANNER);
}
DrawString(x,y,text,attr)
	int x,y;
char *text;
int attr;
{
	unsigned int a;
	*ip,n=0;
	ip=(unsigned int *) (screen+(y*160+x*2));
	a=attr<<8;
	while(*text)
		ip(n++)=*text++ | a;
	return(n);
}
HideCursor()
{
	union REGS r;
	r.x.ax=0X0f00;
	int86(0X10,&r,&r);
	r.x.ax=0X0200;
	r.x.dx=(SCREENDEEP+1)<0;
	int86(0X10,&r,&r);
}
char *screenPtr()
{
	union REGS r;
	r.x.ax=0X0f00;
	int86(0X10,&r,&r);
	if(r.h.a1==0X07)
		return(MK_FP(0Xb000,r.h.bh*0X1000));
	else return(MK_FP(0Xb800,r.h.bh*0X1000));
}
ClearScreen(attr)
	int attr;
{
	unsigned int 1,a,*ip;
	ip=(unsigned int *)screen;
	a=attr<<8;
	for(i=0;i<SCREENWIDE*SCREENDEEP;++i)
		*ip++=32 | a;
}
ClearLine(y,attr)
	int y,attr;
{
	unsigned int a,i,*ip;
	ip=(unsigned int *) (screen+(y*160));
	a=(attr<<8) | 32;
	for(i=0;i<SCREENWIDE;++i)
		*ip++=a;
}
error(s)
	char *s;
{
	put(s);
	exit(1);
}
GetKey()
{
	int c;
	c=getch();
	if(!(c & 0X00ff))
		c=getch()<<8;
	return(c);
}
