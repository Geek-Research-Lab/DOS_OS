#include "stdio.h"
#include "alloc.h"
#include "dos.h"
#define DEBUG 0
#define DEGUBFILE "DEBUG.LJ"
#define TIMESROMAN 1
#define HELVETICA 0
#define COURIER 0
#define XMPY 1
#define XDIV 12
int TimesRomanWidth[]={48,64,64,152,100,188,164,52,76,76,100,200,48,68,48,56,
						100,100,100,100,100,100,100,100,100,100,56,56,200,200,200,100,
						196,152,136,140,160,140,124,164,164,72,88,148,128,188,152,160,
						120,160,152,108,128,164,152,196,152,152,144,64,100,64,200,100,
						52,96,108,88,108,100,64,96,108,56,52,104,56,164,108,108,
						108,108,76,76,64,108,92,136,88,88,88,100,100,100,200,4};
int HelveticaWidth[]={56,64,64,152,108,168,136,52,68,68,100,200,56,64,56,60,
						108,108,108,108,108,108,108,108,108,108,60,60,200,200,200,104,
						200,128,132,144,140,128,116,148,144,52,100,128,108,168,144,152,
						124,156,132,124,116,144,128,180,124,124,120,72,100,72,200,100,
						52,108,116,104,116,108,52,116,112,40,40,104,40,168,112,116,
						116,116,68,100,56,112,96,144,96,100,96,100,100,100,200,4};
int CourierWidth[]={120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
					120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
					120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
					120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
					120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
					120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120};
#if TIMESROMAN
#define WIDTHTABLE TimesRomanWidth
#define FONTFILE "DUTCH12.SFP"
#endif
#if HELVETICA
#define WIDTHTABLE TimesRomanWidth
#define FONTFILE "SWISS12.SFP"
#endif
#if COURIER
#define WIDTHTABLE TimesRomanWidth
#define FONTFILE " "
#endif
#define LEFTMARGIN 100
#define RIGHTMARGIN 100
#define TOPMARGIN 100
#define BOTTOMMARGIN 100
#define PAGEWIDE 2400-(RIGHTMARGIN+LEFTMARGIN)
#define PAGEDEEP 3100-(TOPMARGIN+BOTTOMMARGIN)
#define POINTSIZE 12
#define LEADING 6
#define WORDSIZE 128
#define PRINTERPORT 0
#define TABSIZE 36
#define points2dots(n) ((double)n*4.1667)
double WordSize(char *word);
double CWidth(int c, int tab[], int size);
void writeword(char *word,double *linesize,double *pagedeep);
void newline(double *linesize,double *pagedeep);
void ejectpage(double *linesize,double *pagedeep);
#if DEBUG
FILE *debug;
#endif
unsigned int pagenumber=0;
main(argc,argv)
	int argc;
char *argv[];
{
	FILE *fp;
	char word[WORDSIZE+16];
	double linesize=0,pagedeep=0,spacewidth,tabsize;
	int c,i,n,wordindex=0;
	puts("Simple LaserJet Text Printer \r\n" "-------------------------------- \r\n");
#if DEBUG
	if((debug=fopen(DEBUGFILE,"wb"))==NULL)
		error("unable to create debug file");
#endif
	if(argc<=1)
		error("Argument: Path to a text file");
	if((fp=fopen(argv[1],"rb"))==NULL)
		error("unable to open source file");
	p_string("\033E");
	printf("\r Downloading...");
	if(!DownloadFont(FONTFILE))
		error("unable to download the font");
	saypage();
	do
	{
		if((c=fgetc(fp))!=EOF)
		{
			if(c==0X8d)
				c=0;
			c&=0X7f;
			if(c==32)
			{
				do
				{
					if(wordindex < WORDSIZE)
						word[wordindex++]=c & 0X7f;
					c=fgetc(fp);
				}
				while((c & 0X7f)==32);
				word[wordindex]=0;
				ungetc(c,fp);
				writeword(word,&linesize,&pagedeep);
				wordindex=0;
				word[wordindex]=0;
			}
			else if(c==13)
			{
				word[wordindex]=0;
				writeword(word,&linesize,&pagedeep);
				wordindex=0;
				word[wordindex]=0;
				newline(&linesize,&pagedeep);
			}
			else if(c==9)
			{
				spacewidth=CWidth(32,WIDTHTABLE,POINTSIZE);
				if(linesize > 0)
					tabsize=(double)((int)linesize % (int)points2dots(TABSIZE));
				else tabsize=points2dots(TABSIZE);
				n=(int)(tabsize/spacewidth);
				for(i=0;i<n && wordindex < WORDSIZE;++i)
					word[wordindex++]=32;
			}
			else if(c > 32 && c < 128)
			{
				if(wordindex < WORDSIZE)
					word[wordindex++]=c;
			}
		}
	}
	while(c!=EOF);
	writeword(word,&linesize,&pagedeep);
	wordindex=0;
	ejectpage(&linesize,&pagedeep);
	printf("\r All done... \r\n");
	fclose(fp);
#if DEBUG
	fclose(debug);
#endif
}
void writeword(word,linesize,pagedeep)
	char *word;
double *linesize,*pagedeep;
{
	double wordsize;
	char b[256];
	if(!word[0])
		return;
	wordsize=WordSize(word);
	if((int)((*linesize)+wordsize) >= PAGEWIDE)
		newline(linesize,pagedeep);
	if((*linesize)==0)
	{
		sprintf(b,"\033*p%uX\033*p%uY",LEDTMARGIN,TOPMARGIN+(int)(*pagedeep));
	p_string(b);
	}
p_string(word);
(*linesize)+=wordsize;
}
void newline(linesize,pagedeep)
	double *linesize,*pagedeep;
{
	(*pagedeep)+=(double)points2dots((POINTSIZE+LEADING));
	if((int)(*pagedeep) >= PAGEDEEP)
		ejectpage(linesize,pagedeep);
	(*linesize)=0;
}
void ejectpage(linesize,pagedeep)
	double *linesize,*pagedeep;
{
	(*linesize)=(*pagedeep)=0;
	p_string('\014');
	saypage();
}
double WordSize(word)
	char *word;
{
	double wordsize=0;
	int i,n;
	n=strlen(word);
	for(i=0;i<n;++i)
		wordsize+=CWidth((int)word[i],WIDTHTABLE,POINTSIZE);
	return(wordsize);
}
p_buff(s,n)
	char *s;
unsigned int n;
{
	while(n-)
		p_char(*s++);
}
p_string(s)
	char *s;
{
	while(*s)
		p_char(*s++);
}
p_char(c)
	int c;
{
#if DEBUG
	fputc(c,debug);
#else
	union REGS r;
	do
	{
		r.h.ah=2;
		r.x.dx=PRINTERPORT;
		int86(0X17,&r,&r);
	}
	while(!(r.h.ah & 0X80));
	r.h.ah=0;
	r.h.al=c;
	r.x.dx=PRINTERPORT;
	int86(0X17,&r,&r);
#endif
}
error(s)
	char *s;
{
	put(s);
	exit(1);
}
double CWidth(c,tab,size)
	int c, tab[], size;
{
	return(points2dots((double)0.06*((double)XMPY/(double)XDIV)*(double)size*(double)tab[(c & 0X7f)-32]));
}
saypage()
{
	printf("\r Printing page %u",++pagenumber);
}
DownloadFont(path)
	char *path;
{
	FILE *fp;
	unsigned long size;
	char *p;
	if(!strlen(path))
		return(1);
	if((fp=fopen(path,"rb"))==NULL)
		return(0);
	fseek(fp,0L,SEEK_END);
	size=ftell(fp);
	rewind(fp);
	if(size > 0X0000fff0L)
	{
		fclose(fp);
		return(0);
	}
	if((p=malloc((unsigned int)size))==NULL)
	{
		fclose(fp);
		return(0);
	}
	if(fread(p,1,(unsigned int)size,fp)!=(unsigned int)size)
	{
		free(p);
		fclose(fp);
		return(0);
	}
	p_string("\033*c0F");
	p_string("\033*c8D");
	p_buff(p,(unsigned int)size);
	p_string("\033*c5F");
	p_string("\033&100\033(8X");
	free(p);
	fclose(fp);
	return(1);
}
