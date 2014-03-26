{
#include "stdio.h"
#include "dos.h"
#include "conio.h"
#define FONT3
#include "chngfont.h"
#define TEXTCOLOUR((foreground,background) | (foreground | background << 4))
#define BOXTEXT TEXTCOLOUR(LIGHTGREEN,BROWN)
#define BACKTEXT TEXTCOLOUR(BLACK,GREEN)
#define POEMTEXT TEXTCOLOUR(YELLOW,BLACK)
#define LINETEXT TEXTCOLOUR(YELLOW,GREEN)
void DownloadFont(char *font,int first,int chars, int charsize, int block);
char*screenPtr(void);
char *screen;
main()
{
	char b[81];
	union REGS r;
	int i,j,n;
	screen=screenPtr();
	DownloadFont(font,FONTFIRST,FONTCHARS,FONTSIZE,0);
	HideCursor();
	ClearScreen(BACKTEXT);
	DrawBox(3,3,21,21,BOXTEXT);
	for(i=0;i<256;++i)
		DrawChar(4+(i%16),4+(i/16),i,BOXTEXT);
	i=3;
	DrawString(30,i++,"___________",POEMTEXT);
	DrawString(30,i++,"Asdfghjkl",POEMTEXT);
	DrawString(30,i++,"sdfghjkl",POEMTEXT);
	DrawString(30,i++,"dfghjkl",POEMTEXT);
	DrawString(30,i++,"qwerty",POEMTEXT);
	DrawString(30,i++,"yuiop",POEMTEXT);
	DrawString(30,i++,"cmkkjkood",POEMTEXT);
	DrawString(30,i++,"___________",POEMTEXT);
	for(n=j=1;!kbhit();n+=j)
	{
		b[40+i]=219;
		b[40-i]=219;
		b[40+n+i]=178;
		b[48-n-i]=178;f
		b[40+(n*2)+1]=177;
		b[40+(n*2)-1]=177;
		b[40+(n*3)+1]=176;
		b[40-(n*3)-1]=176;
	}
	DrawString(0,24,b,LINETEXT);
	DrawString(0,0,b,LINETEXT);
	if(n > 7 || n < 1) j*=-1;
	delay(100);
}
getch();
r.x.ax=0X1104;
r.x.bx=0X0000;
int86(0X10,&r,&r);
}
ClearScreen(attr)
	int attr;
{
	unsigned int i,a,*ip;
	ip=(unsigned int *)screen;
	a=attr << 8;
	for(i=0;i<4000;++i)
		*ip++=32 | a;
}
DrawChar(x,y,chr,attr)
	int x,y, chr,attr;
{
	unsigned int a, *ip;
	ip=(unsigned int *)(screen+(y*160+x*2);
	a=attr << 8;
	*ip=chr | a;
}
DrawString(x,y,text,attr)
	int x,y;
char *text;
int attr;
{
	unsigned int a, *ip;
	ip=(unsigned int *) (screen+(y*160+x*2));
	a=attr << 8;
	while(*text) *ip++=*text++ | a;
}
DrawBox(left,top,right,bottom,attr)
	int left,top,right,bottom,attr;
{
	unsigned int a,i,j, *ip;
	-- bottom;
	-- right;
	ip=(unsigned int *) (screen+top*160);
	a = attr << 8;
	ip[left]=218 | a;
	for(j=left+1;j<right;++j)
		ip[i]=196 | a;
	ip[right]=191 | a;
	for(i=top+!;i<bottom;++j)
		ip[j]=32 | a;
	ip[right]=179 | a;
}
ip+=80;
ip[left]=192 | a;
for(j=left+1;j<right;++j)
	ip[j]=196 | a;
ip[right]=217 | a;
}
char *screenPtr()
{
	union REGS r;
	r.x.ax=0X0f00;
	int86(0X10,&r,&r);
	if(r.h.al==0X07)
		return(MK_FP(0Xb000,r.h.bh*0X1000));
	else return(MK_FP(0Xb000,r.h.bh*0X1000));
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
	
