/*Simple PCX reader*/
#include "stdio.h"
#include "stdlib.h"
#include "alloc.h"
#include "dos.h"
#define pixels2bytes(n) ((n+7)/8)
#define plane(n)
{
	outp(0X3c4,2);
	outp(0X3c5,(n));
}
typedef struct
{
	char manufacturer;
	char version;
	char encoding;
	char bits_per_pixel;
	int xmin,ymin;
	int xmax,ymax;
	int hres;
	int vres;
	char palette[48];
	char reserved;
	char colour_planes;
	int bytes_per_line;
	int palette_type;
	char filler[58];
}
PCXHEAD;
main(argc,argv)
	int argc;
char *argv[];
{
	FILE *fp;
	PCXHEAD pcx;
	union REGS r;
	char *screen=MK_FP(0Xa000,0X0000);
	char *p,*pr,palette[768];
	unsigned int width,depth,bytes,bits,planebytes;
	unsigned int screenwidth, screendepth, screenbytes, screenmode;
	unsigned int i,j;
	if(argc <= 1)
		error("argument:path to PCX file");
	if((fp=fopen(argv[1],"rb"))==NULL)
		error("unable to open the file");
	if(fread((char *)&pcx,1,sizeof(PCXHEAD),fp)!=sizeof(PCXHEAD))
		error("unable to read the file");
	if(pcx.manufacturer!=10)
		error("this is NOT PCX file");
		if(pcx.bits_per_pixel==1)
			bits=pcx.colour_planes;
		else bits=pcx.bits_per_pixel;
		width=pcx.xmax-pcx.xmin;
		depth=pcx.ymax-pcx.ymin;
		bytes=pcx.bytes_per_line;
		if(bits==1)
		{
			screenwidth=640;
			screendepth=480;
			screenbytes=80;
			screenmode=0X0012;
			memcpy(palette,"\000\000\000\377\377\377",6);
		}
		else if(bits <= 4)
		{
			screenwidth=640;
			screebdepth=480;
			screenbytes=80;
			planebytes=pixels2bytes(width);
			screenmode=0X0012;
			memcpy(palette,pcx.palette,48);
			bytes*=bits;
		}
		else
		{
			screenwidth=320;
			screendepth=200;
			screenbytes=320;
			screenmode=0X0013;
			fseek(fp,-769L,SEEK_END);
			if(fgetc(fp)!=12)
				error("unable to find the palette");
			if(fread(palette,1,768,fp)!=768)
				error("unable to read the palette");
			fseek(fp,(long)sizeof(PCXHEAD),SEEK_SET);
		}
		if((p=malloc(bytes))==NULL)
			error("unable to allocate the memory");
		r.x.ax=screenmode;
		int86(0X10,&r,&r);
		setpalette(palette,bits);
		for(i=0;i<depth && i<screendepth;++i)
		{
			if(readpcxline(p,fp,bytes)!=bytes)
			{
				r.a.ax=0X0003;
				int86(0X10,&r,&r);
				error("PCX file is damaged");
			}
			if(bits==1)
				memcpy(screen,p,min(screenbytes,bytes));
			else if(bits==8)
				memcpy(screen,p,min(screenwidth,bytes));
			else
			{
				pr=p;
				for(j=0;j<bits;++j)
				{
					plane(l<<j);
					memcpy(screen,pr,min(screenbytes,planebytes);
					pr+=planebytes;
				}
				plane(0X0f);
			}
			screen+=screenbytes;
		}
		getch();
		r.x.ax=0X0003;
		int86(0X10,&r,&r);
}
readpcxline(p,fp,bytes)
	char *p;
FILE *fp;
unsigned int bytes;
{
	int n=0,c,i;
	memset(p,0,bytes);
	do
	{
		c=fgetc(fp) & 0Xff;
		if((c & 0Xc0)==0Xc0)
		{
			i=c & 0X3f;
			c=fgetc(fp);
			while(i--)
				p[n++]=c;
		}
		else p[n++]=c;
	}
	while(n<bytes);
	return n;
}
setpalette(p,bits)
	char *p;
int bits;
{
	union REGS r;
	int i,n;
	n=l<<bits;
	if(bits <= 4)
	{
		for(i=0;i<n;++i)
		{
			r.x.ax=0X1000;
			r.h.bh=i;
			r.h.bl=i;
			int86(0X10,&r,&r);
		}
	}
	outp(0X3c6,0Xff);
	for(i=0;i<n;++i)
	{
		outp(0X3c8,i);
		outp(0X3c9,(*p++) >> 2);
		outp(0X3c9,(*p++) >> 2);
		outp(0X3c9,(*p++) >> 2);
	}
}
error(s)
	char *s;
{
	puts(s);
	exit(1);
}
