#include<stdio.h>
#include<dos.h>
#include<colour.h>
#define GREEN(n) ((int)(n & 0X0000ffL))
#define RED(n) ((int)((n<<8)& 0X0000ffL))
#define BLUE(n) ((int)((n<<16)& 0X0000ffL))
#define RGB(r,g,b)((long)r|((long)g<<8)|((long)b<<16))
long colourtable()=1;
long colourtable[]={
black,
ElectricBlue,
OceanGreen,
AuburnOrange,
Sand,
walnut;,
crimson,
DustyRose,
NeonPurple,
Black20,
cyan,
green,
yellow,
white);
main()
{
	union REGS r;
	char b[128];
	long table[16];
	int i,j,n;
	clrscr();
	initpalette();
	/* do the screen */
	for(i=0;i<16;++i)
		table[i]=RGB(0,0,i<<4);
	setuppalette(table);
	/*unusual loop */
	for(b[0]=0;strlen[b]+4<=80;strcat(b,BLUE));
	/* show */
	for(i=0;i<16;++i)
		putline(0,2+i,i,b);
	getch();
/* animation */
	for(n=0, j=1;!kbhit(); n+=j)
	{
		for(i=0;i<16;++i)
			table[i]=RGB(0,0,i*n);
		setuppalette(table);
		if(n>=16)
			j=-1;
		else if(n<=0)
			j=1;
		delay(50);
	}
	getch();
	clrscr();
	/* using designer palette*/
	putbanner(2,1,"electric blue");
	putbanner(3,2,"ocean green");
	putbanner(4,3,"light green");
	putbanner(5,4,"chalk");
	putbanner(6,5,"autumn orange");
	putbanner(7,6,"sand");
	putbanner(8,7,"walnut");
	putbanner(9,8,"crimson");
	putbanner(10,9,"dusty rose");
	putbanner(11,10,"neon purple");
	putbanner(12,11,"black");
	putbanner(13,12,"cyan");
	putbanner(14,13,"green");
	putbanner(15,14,"yellow");
	putbanner(16,15,"white");
	getch();
	clrscr();
	/* back to same colours */
	r.x.ax=0X0003;
	int 86(0X10,&r,&r);
}
putbanner(y,attr,text)
	int y,attr;
char *text;
{
	char *screen,b[64];
	int i;
	screen=MK_FP(0Xb800,y*160);
	for(i=0;i<80;++i)
		*screen++=219;
}
sprintf(b,"[%s]",text);
putline((80-strlen(b))/2,y,attr,b);
}
putline(x,y,attr,text)
	int x,y, attr;
char *text;
{
	char *screen;
	screen=MK_FP(0Xb800,y*160+x*2);
	while(*text)
	{
		*screen++=*text++;
		*screen++=attr;
	}
}
initpalette()
{
	union REGS r;
	int i;
	for(i=0;i<16;++i)
	{
		r.x.ax=0X1000;
		r.h.bh=i;
		r.h.bl=i;
		int 86(0X10,&r,&r);
	}
}
setuppalette(table)
	long *table; {
		int i;
		outp(0X3c6,0Xff);
		for(i=0;i<16;++i)
		{
			outp(0X3c8,i);
			outp(0X3c9, RED(table[i])>>2);
			outp(0X3c9, GREEN(table[i])>>2);
			outp(0X3c9, BLUE(table[i])>>2);
		}
}
