#include "stdio.h"
#include "math.h"
#include "graphics.h"
#define HPLOUTPUT 0
#define DRAGONCURVE 1
#define SIERPINSKICURVE 1
#define CCURVE 1
#define HPLEXP 10
#define HPLOFFX 2500
#define HPLOFFY 1500
#define ANGLE 45
#define SQUARE(n) (n*n)
#define PAUSE()
{
	int n;
	for(n=0;n<5000 && !kbhit();++n)
		delay(1);
}
#if !HPLOUTPUT
#define DRAWLINE(x1,y1,x2,y2) line((int)x1,(int)y1,(int)x2,(int)y2);
#else
#define DRAWLINE(x1,y1,x2,y2) plotline(double)x1,(double)y1,(double)x2,(double)y2);
#endif
CCurve(double x1,double y1,double x2,double y2);
DragonCurve(double x1,double y1,double x2,double y2,double th);
SierpinskiCurve(int n);
double prm[5];
double tmp_angle,sin_angle,cos_angle,posit;
double ox,oy;
#if HPLOUTPUT
FILE *fp=NULL;
int graphicindex=0;
#endif
main()
{
	init();
	do
	{
#if DRAGONCURVE
		for(prm[4]=8;prm[4]>=3 && !kbhit(); prm[4]-=4)
		{
			NewScreen();
			prm[0]=200;
			prm[1]=240;
			prm[2]=450;
			prm[3]=240;
			cos_angle=cos(ANGLE*M_PI/180);
			tmp_angle=sin(ANGLE*M_PI/180);
			DragonCurve(prm[0],prm[1],prm[2],prm[3],-1 * ANGLE);
			PAUSE();
		}
		if(kbhit())
			break;
#endif
#if SIERPINSKICURVE
		prm[0]=32;
		prm[1]=32;
		for(prm[2]=1;prm[2]<7 && !khbit(); prm[2]+=1)
		{
			NewScreen();
			posit=ceil(128/(pow(2,prm[2]-1)*4+2));
			SierpinskiCurve(prm[2]);
			PAUSE();
		}
#endif
#if CCURVE
		for(prm[4]=5;prm[4]>=2 && !kbhit(); prm[4]-=1)
		{
			NewScreen();
			prm[0]=225;
			prm[1]=112;
			prm[2]=412;
			prm[3]=112;
			cos_angle=cos(ANGLE*M_PI/180);
			sin_angle=sin(ANGLE*M_PI/180);
			CCurve(prm[0],prm[1],prm[2],prm[3]);
		}
#endif
#if HPLOUTPUT
		break;
#endif
	}
	while(!kbhit());
	getch();
	deinit();
}
CCurve(x1,y1,x2,y2)
	double x1,y1,x2,y2;
{
	double xd,yd;
	double mx,my;
	double xdr,ydr;
	xd=x2-x1;
	yd=y2-y1;
	if((SQUARE(xd)+SQUARE(yd))<=SQUARE(prm[4])||kbhit())
	{
		DRAWLINE(x1,y1,x2,y2);
	}
	else
	{
		xdr=xd/2/cos_angle;
		ydr=yd/2/cos_angle;
		mx=x1+xdr*cos_angle=ydr*sin_angle;
		my=y1+xdr*sin_angle=ydr*cos_angle;
		CCurve(x1,y1,mx,my);
		CCurve(mx,my,x2,y2);
	}
}
SierpinskiCurve(n)
	int n;
{
	left(n);
	DRAWLINE(prm[0],prm[1],prm[0]+posit,prm[1]+posit);
	prm[0]+=posit;
	prm[1]+=posit;
	top(n);
	DRAWLINE(prm[0],prm[1],prm[0]+posit,prm[1]-posit);
	prm[0]+=posit;
	prm[1]-=posit;
	right(n);
	DRAWLINE(prm[0],prm[1],prm[0]-posit,prm[1]-posit);
	prm[0]-=posit;
	prm[1]-=posit;
	bottom(n);
	DRAWLINE(prm[0],prm[1],prm[0]-posit,prm[1]+posit);
	prm[0]-=posit;
	prm[1]+=posit;
}
left(n)
	int n;
{
	if(n && !kbhit())
	{
		--n;
		left(n);
		DRAWLINE(prm[0],prm[1],prm[0]+posit,prm[1]+posit);
		prm[0]-=posit;
	prm[1]+=posit;
	top(n);
	DRAWLINE(prm[0],prm[1],prm[0],prm[1]+posit*2);
	prm[1]+=floor(posit*2);
	bottom(n);
	DRAWLINE(prm[0],prm[1],prm[0]-posit,prm[1]+posit);
	prm[0]-=posit;
	prm[1]+=posit;
	left(n);
	}
}
top(n)
	int n;
{
	if(n && !kbhit())
	{
		--n;
		top(n);
		DRAWLINE(prm[0],prm[1],prm[0]+posit,prm[1]-posit);
		prm[0]+=posit;
	prm[1]-=posit;
	right(n);
	DRAWLINE(prm[0],prm[1],prm[0]+posit*2,prm[1]);
	prm[0]+=floor(posit*2);
	left(n);
		DRAWLINE(prm[0],prm[1],prm[0]+posit,prm[1]+posit);
		prm[0]+=posit;
	prm[1]+=posit;
	top(n);
	}
}
right(n)
	int n;
{
	if(n && !kbhit())
	{
		--n;
		right(n);
		DRAWLINE(prm[0],prm[1],prm[0]-posit,prm[1]-posit);
		prm[0]-=posit;
	prm[1]-=posit;
	bottom(n);
	DRAWLINE(prm[0],prm[1],prm[0],prm[1]-posit*2);
	prm[1]-=floor(posit*2);
	top(n);
	DRAWLINE(prm[0],prm[1],prm[0]+posit,prm[1]-posit);
		prm[0]+=posit;
	prm[1]-=posit;
	right(n);
	}
}
bottom(n)
	int n;
{
	if(n && !kbhit())
	{
		--n;
		bottom(n);
		DRAWLINE(prm[0],prm[1],prm[0]-posit,prm[1]+posit);
		prm[0]-=posit;
		prm[1]+=posit;
		left(n);
		DRAWLINE(prm[0],prm[1],prm[0]-posit*2,prm[1]);
		prm[0]-=floor(posit*2);
		right(n);
		DRAWLINE(prm[0],prm[1],prm[0]-posit,prm[1]-posit);
		prm[0]-=posit;
		prm[1]-=posit;
		bottom(n);
	}
}
DragonCurve(x1,y1,x2,y2,th)
	double x1,y1,x2,y2,th;
{
	double xd,yd;
	double mx,my;
	double xdr,ydr;
	if(th>0)
		sin_angle=tmp_angle;
	else
		sin_angle=-tmp_angle;
	xd=x2-x1;
	yd=y2-y1;
	if((SQUARE(xd)+SQUARE(yd))<=SQUARE(prm[4])||kbhit())
	{
		DRAWLINE(x1,y1,x2,y2);
	}
	else
	{
		xdr=xd/2/cos_angle;
		ydr=yd/2/cos_angle;
		mx=x1+xdr*cos_angle=ydr*sin_angle;
		my=y1+xdr*sin_angle=ydr*cos_angle;
		DragonCurve(x1,y1,mx,my,ANGLE);
		DragonCurve(mx,my,x2,y2,-1*ANGLE);
	}
}
deinit()
{
#if HPLOUTPUT
	if(fp!=NULL)
	{
		fprintf(fp,"PU; SPG;");
		fclose(fp);
	}
#else
	closegraph();
#endif
}
init()
{
#if !HPLOUTPUT
	int d,m;
	detectedgraph(&d,&m);
	if(d<0)
	{
		puts("error initializing graphics");
		exit(1);
	}
	initgraph("%d",&m," ");
	if(graphresult()<0)
	{
		puts("error initializing graphics");
		exit(1);
	}
	setcolor(getmaxcolor());
#endif
}
NewScreen()
{
#if HPLOUTPUT
	char b[24];
	if(fp!=NULL)
	{
		fprintf(fp,"PU; SP0;");
		fclose(fp);
	}
	fp=NULL;
	sprintf(b,"CURVE%02.2u,PLT",++graphicindex);
	if((fp=fopen(b,"wa"))==NULL)
	{
		printf("error creating %s\n",b);
		exit(1);
	}
	printf("writing %s\n",b);
	ox=oy=0;
#else
	cleardevice();
#endif
}
#if HPLOUTPUT
plotline(x1,y1,x2,y2)
	double x1,x2,y1,y2;
{
	if(x1!=ox||y1!=oy)
		fprintf(fp,"PU,PA,%g,%g;PD,PA,%g,%g;\n",
	(double)HPLOFFX+x1*HPLEXP,
		(double)HPLOFFX+y1*HPLEXP,
		(double)HPLOFFX+x2*HPLEXP,
		(double)HPLOFFX+y2*HPLEXP);
	else
		fprintf(fp,"PA,%g,%g;PD,PA,%g,%g;\n",
	(double)HPLOFFX+x1*HPLEXP,
		(double)HPLOFFX+y1*HPLEXP,
		(double)HPLOFFX+x2*HPLEXP,
		(double)HPLOFFX+y2*HPLEXP);
}
#endif
