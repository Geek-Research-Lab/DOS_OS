#include "stdio.h"
#include "stdlib.h"
#include "dos.h"
#include "graphics.h"
typedef struct
{
	int pitch;
	char name[3];
}
NOTE;
char song[]="c3 c16 o2 b16 o3 c16 r16 o2 c8 c8 g16 f16 e16 g16 o3 c8 c16 e16"
			"d16 c16 d16 r16 o2 d8 d8 d16 c16 o1 b16 o2 d16 g8 g8"
			"o2 a16 b16 o3 c16 o2 b16"
			"a16 g16 a16 g16"
			"f16 e16 f16 e16"
			"d16 c16 d16 c16"
			"o1 b16 a16 g16 a16"
			"b16 o2 c16 d16 e16"
			"f16 g16 e8 c8 c8"
			"o3 c16 o2 b16 o3 o16 r16 c8 c8"
			"c16 o2 b16 a16 c16 o2 b16"
			"a16 o2 g16 a16 g16"
			"f16 o16 f16 e16"
			"d16 c16 d16 c16"
			"o1 b16 a16 g16 a16 b16 o2"
			"c16 d16 e16 f16 g16 e8 c8 c8"
			;
NOTE scale[]={262,"c",
	278,"c#",
	294,"d",
	312,"d#",
	330,"e",
	349,"f",
	371,"f#",
	392,"g",
	416,"g#",
	440,"a",
	468,"a#",
	494,"b"
};
#define SCALESIZE (sizeof(scale)/sizeof(NOTE))
char *songpointer=NULL;
int octave=1;
int duration=4;
int pitch=-1;
int songindex=0;
void interrupt beNoisy();
main()
{
	void interrupt(*oldTimer)();
	oldTimer=getvect(0X1c);
	setvect(0X1c,beNoisy);
	playtune(song);
	DoSomething();
	setvect(0X1c,oldTimer);
	nosound();
}
playtune(song)
	char *song;
{
	disable();
	songindex=0;
	songpointer=song;
	enable();
}
void interrupt beNoisy()
{
	static int count, busy;
	int i,n,gotnote=0;
	if(busy)
		return;
	if(songpointer==NULL)
		return;
	if(count>1)
	{
		--count;
		return;
	}
	busy=1;
	do
	{
		while(songpointer[songindex]==32)
			++songindex;
		switch(n=tolower(songpointer[songindex]))
		{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
			for(i=0;i<SCALESIZE;++i)
			{
				if(n==scale[i].name[0])
				{
					++songindex;
					if(songpointer[songindex]=='#')
					{
						++i;
						++songindex;
					}
					pitch=scale[i].pitch;
					break;
				}
			}
			n=atoi(songpointer+songindex);
			if(n>=1 && n<=32)
				duration=32/n;
			++songindex;
			gotnote=1;
			break;
		case 'r':
			++songindex;
			n=atoi(songpointer+songindex);
			if(n>=1 && n<=32)
				duration=32/n;
			pitch=-1;
			++songindex;
			gotnote=-1;
			break;
		case 'o':
			++songindex;
			n=atoi(songpointer+songindex);
			if(n>=1 && n<=4)
				octave=n;
			pitch=-1;
			++songindex;
			break;
		case 0:
			songindex=0;
			pitch=-1;
			duration=1;
			octave=1;
			gotnote=1;
			break;
		default:
			while(songpointer[songindex]!=32 && songpointer[songindex]!=0)
				++songindex;
			break;
		}
	}
	while(!gotnote);
	if(pitch==-1)
		nosound();
	else
	{
		nosound();
		sound(pitch*(1<<(octave-1)));
	}
	count=duration;
	busy=0;
}
DoSomething()
{
	int screenwide, screendeep;
	int d,m,i,n-0;
	detectgraph(&d,&m);
	if(d<0)
		return(0);
	initgraph(&d,&m,"");
	if(graphresult()<0)
		return(0);
	setcolor(getmaxcolor());
	screenwide=getmaxx();
	screendeep=getmaxy();
	do
	{
		setcolor(n++);
		for(i=0;i<screenwide;i+=4)
		{
			line(screenwide/2,screendeep/2,i,0);
			line(screenwide/2,screendeep/2,i,screendeep-1);
		}
		for(i=0;i<screendeep;i+=4)
		{
			line(screenwide/2,screendeep/2,0,i);
			line(screenwide/2,screendeep/2,screenwide-1,i);
		}
	}
	while(!kbhit());
	getch();
	closegraph();
}
