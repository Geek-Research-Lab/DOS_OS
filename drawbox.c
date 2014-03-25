#include "stdio.h"
#include "dos.h"
#include "conio.h"
#define TEXTCOLOUR(foreground, background) (foreground | (background<<4))
char *screenPtr(void);
char *screen;
main()
{
int i, j;
screen=screenPtr();
textattr(TEXTCOLOUR(BLACK,BLUE));
clrscr();
textattr(TEXTCOLOUR(BLUE,LIGHTGRAY));
clreol();
HideCursor();
DrawString(0,0, "here comes title" "to be filled", TEXTCOLOUR(BLUE,LIGHTGRAY));
for(i=1;i<25;++i)
{
for(j=0;j<80;j+=4)
DrawString(j,i,"TEXT",TEXTCOLOUR(LIGHTGRAY,BLUE));
}
DrawBox(10,10,30,16,TEXTCOLOUR(YELLOW,GREEN));
DrawString(11,11,"here is a box!",TEXTCOLOUR(YELLOW,GREEN));
DrawString(11,12,"Testing...", TEXTCOLOUR(YELLOW,GREEN));
Draw3DBox(40,10,60,16, TEXTCOLOUR(YELLOW,GREEN));
DrawString(41,11,"This is a box",TEXTCOLOUR(YELLOW,GREEN));
DrawString(41,12,"test box..", TEXTCOLOUR(YELLOW,GREEN));
getch();
textattr(WHITE);
clrscr();
}
DrawString(x,y,text,attr)
int x,y;
char *text;
int attr;
{
unsigned int a, *ip;
ip=(unsigned int *) (screen+(y*160+x*2));
a=attr<<2;
while(*text)
*ip++=*text++ | a;
}
DrawBox(left,top,right,bottom,attr)
int left,top,right,bottom,attr;
{
unsigned int a,i,j,*ip;
--bottom;
--right;
ip=(unsigned int *) (screen+top*160);
a=attr<<8;
ip[left]=218 | a;
for(j=left+1;j<right;++j)
ip[j]=196 | a;
ip[right]=191 |a;
for(i=top+1;i<bottom;++i)
{
ip+=80;
ip[left]=179 | a;
for(j=left+1;j<right;++j)
	ip[j]=32 | a;
ip[right]=179 | a;
}
ip+=80;
ip[left]=192 | a;
for(j=left+1;j<right;++j)
	ip[j]=196 | a;
ip[right]=217 | a;
}
Draw3DBox(left,top,right,bottom,attr)
	int left,top,right,bottom,attr;
{
	unsigned int a,i,j,*ip;
	--bottom;
	--right;
	ip=(unsigned int *) (screen+top*160);
	a=attr<<8;
	ip[left]=218 | a;
	for(j=left+1;j<right;++j)
		ip[i]=196 | a;
	ip[right]=191 | a;
	ip[right+81]=(ip(right+81) & 0X00ff) | 0X0700;
	for(i=top+1;i<bottom;++i)
	{
		ip+=80;
		ip[left]=192 | a;
		ip[left+81]=(ip[left+81] & 0X00ff) | 0X0700;
		for(j=left+1;j<right;++j)
		{
			ip[j]=196 | a;
			ip[j+81]=(ip[j+81] & 0X00ff | 0X0700;
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
		char *screenPtr()
		{
			union REGS r;
			r.x.ax=0X0f00;
			int86(0X10,&r,&r);
			if(r.h.a1==0X07)
				return(MK_FP(0Xb000,r.h.bh*0X1000));
			else return(MK_FP(0Xb800,r.h.bh*0X1000));
		}
