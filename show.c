/* NOTE:
show.c is a C-program that shows how to link data object from an object module created by "prelink.c" --> found in my repository.
Also this program uses a graphic fragment of the sort expected by the PutImage function to draw a bitmap of a girl's eye on screen*/
#include<stdio.h>
#include<graphics.h>
#include<alloc.h>
extern char showeye[];
char *mono2ega(char *source);
main()
{
	char *p;
	int d,m;
	detectgraph(&d,&m);
	if(d<0)
		return(0);
	initgraph(&d,&m," ");
	if(graphresult()<0)
		exit(1);
	if(getmaxcolor()==2)
		putimage(10,10,imagename,NOT_PUT);
	else
	{
		if((p=mono2ega(showeye))!=NULL)
		{
			putimage(10,10,p,NOT_PUT);
			free(p);
		}
	}
	getch();
	closegraph();
}
char *mono2ega(source)
	char *source
{
	char *p;
	int x,y,j,ls,sz;
	x=1+source[0]+(source[1]<<8);
	y=1+source[2]+(source[3]<<8);
	if((sz=imagesize(0,0,x,y))!=-1)
	{
		if((p=malloc(sz))!=NULL)
		{
			memcpy(p,source,4);
			ls=(x+7)/8;
			for(j=0;j<y;++j)
			{
				memcpy=(p+4+((j*4)*ls),source+4+(j*ls),ls);
				memcpy=(p+4+ls+((j*4)*ls),source+4+(j*ls),ls);
				memcpy=(p+4+(ls*2)+((j*4)*ls),source+4+(j*ls),ls);
				memcpy=(p+4+(ls*3)+((j*4)*ls),source+4+(j*ls),ls);
			}
			return(p);
		}
	}
	else
		return(NULL);
}
