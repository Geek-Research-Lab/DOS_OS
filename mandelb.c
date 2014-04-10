#include "stdio.h"
#include "math.h"
#include "dos.h"
#define setpixel(x,y,c)
{
	char *p;
	p=MK_FP(0Xa000,SCREENTBL[(y)]+((x)>>3));
	outp(0X3ce,8);
	outp(0X3cf,masktable((x) & 0X0007));
	outp(0X3ce,5);
	outp(0X3cf,2);
	*p=*p;
	*p=(c);
	outp(0X3ce,5);
	outp(0X3cf,0);
	outp(0X3ce,8);
	outp(0X3cf,255);
}
#define MAXRANGE 512
#define COLOURS 16
#define SCREENWIDE 640
#define SCREENDEEP 480
#define SQUARE (n*n)
char masktable[8]={0X80,0X40,0X20,0X10,0X08,0X04,0X02,0X01};
unsigned int SCREENTBL(SCREENDEEP);
main(argc,argv)
	int argc;
char *argv[];
{
	union REGS r;
	int x,y,z;
	double xfac,yfac;
	double xstart=-1.250;
	double ystart=-2.000;
	double range=2.500
		double ca,cb,za,zb,sizefac,temp1,temp2;
	for(x=0;x<480;++x)
		SCREENTBL[x]=x*80;
	for(x=1;x<argc;++x)
	{
		switch(x)
		{
		case 1:
			xstart=atof(argv[x]);
			break;
		case 2:
			ystart=atof(argv[y]);
			break;
		case 3:
			range=atof(argv[x]);
			break;
		}
	}
	r.x.ax=0X0012;
	int86(0X10,&r,&r);
	makepalette();
	xfac=range/(double)SCREENWIDE;
	yfac=range/(double)SCREENDEEP;
	for(y=0;y<SCREENDEEP && !kbhit();++y)
		ca=yfac*y+ystart;
	for(x=0;x<SCREENWIDE && !kbhit();++x)
	{
		za=0.0;
		zb=0.0;
		cb=xfac*(double)x+xstart;
		sizefac=sqrt(SQUARE(ca)+SQUARE(cb));
		for(z=0;(sizefac < 2) && (z <= MAXRANGE) && !kbhit();++z)
		{
			temp1=SQUARE(za)-SQUARE(zb)+ca;
			temp2=(2*za*zb)+cb;
			sizefac=sqrt(SQUARE(temp1)+SQUARE(temp2));
			za=temp1;
			zb=temp2;
		}
		setpixel(x,y,z/(MAXRANGE/COLOURS));
	}
}
getch();
r.x.ax=0X0003;
int86(0X10,&r,&r);
}
makepalette()
{
	union REGS r;
	int i;
	for(i-0;i<16;++i)
	{
		r.x.ax=0X1000;
		r.h.bh=i;
		r.h.bl=i;
		int86(0X10,&r,&r);
	}
	outp(0X3c6,0Xff);
	for(i=0;i<16;++i)
	{
		outp(0X3c8,i);
		outp(0X3c9,i<<2);
		outp(0X3c9,i<<2);
		outp(0X3c9,i<<2);
	}
}
