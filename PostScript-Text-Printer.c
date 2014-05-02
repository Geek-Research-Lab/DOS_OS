#include "stdio.h"
#include "dos.h"
#define DEBUG 0
#define DEBUGFILE "DEBUG.PS"
#define TIMESROMAN 1
#define HELVETICA 0
#define COURIER 0
#define XMPY 12
#define XDIV 720
int TimesRomanWidth[]={250,333,408,500,500,833,778,333,333,333,500,564,250,333,250,278,
						500,500,500,500,500,500,500,500,500,500,278,278,564,564,564,444,
						921,722,667,667,722,611,556,722,722,333,389,722,611,889,722,722,
						556,722,667,556,611,722,722,944,722,722,611,333,278,333,489,500,
						333,444,500,444,500,444,333,500,500,278,278,500,278,778,500,500,
						500,500,333,389,278,500,500,722,500,500,444,480,200,480,541,0};
Let HelveticaWidth[]={278,278,355,556,556,889,667,222,333,333,389,584,278,333,278,278,
					556,556,566,566,566,566,566,566,566,566,278,278,584,584,584,556,
					1015,667,667,722,722,667,611,778,722,278,500,667,556,833,722,778;
					667,778,722,667,611,722,667,944,667,667,611,278,278,278,469,556,
					222,556,556,500,556,556,278,556,556,222,222,500,222,833,556,556,
					556,556,333,500,278,556,500,722,500,500,500,334,260,334,584,0};
int CourierWidth[]={600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,
					600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,
					600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,
					600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,
					600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,
					600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600};
#if TIMESROMAN
#define WIDTHTABLE TimesRomanWidth
#endif
#if HELVETICA
#define WIDTHTABLE HelveticaWidth
#endif
#if COURIER
#define WIDTHTABLE CourierWidth
#endif
#define LEFTMARGIN 40
#define RIGHTMARGIN 20
#define TOPMARGIN 20
#define PAGEWIDE 576-(RIGHTMARGIN+LEFTMARGIN)
#define PAGEDEEP 720
#define POINTSIZE 12
#define LEADING 6
#define WORDSIZE 128
#define PRINTERPORT 0
#define TABSIZE 36
double WordSize(char *word);
double CWidth(int c, int tab[], int size);
void writeword(char *word, double *linesize, double *pagedeep);
void newline(double *linesize, double *pagedeep);
void ejectpage(double *linesize, double *pagedeep);
#if DEBUG
FILE *debug
#endif
	unsigned int pagenumber=0;
main(argc,argv)
	int argc;
char *argv[];
{
	FILE *fp;
	char word[WORDSIZE+16];
	double linesize=0, pagedeep=0, spacewidth, tabsize;
	int c, i, n, wordindex=0;
	puts("PostScript(PS) Text Printer\r\n""--------------------------------------\r\n");
#if DEBUG
	if((debug=fopen(DEBUGFILE,"wb"))==NULL)
		error("unable to create debug file");
#endif
	if(argc <= 1)
		error("Argument: Path to a text file");
	if((fp=fopen(argv[1],"rb"))==NULL)
		error("unable to open the source file");
#if TIMESROMAN
	sprintf(word,"/Times-Roman findfont %u scalefont setfont\r\n",POINTSIZE);
#endif
#if HELVETICA
	sprintf(word,"\Helvetica findfont %u scalefont setfont\r\n",POINTSIZE);
#endif
#if COURIER
	sprintf(word,"/Courier findfont %u scalefont setfont\r\n",POINTSIZE);
#endif
	p_string(word);
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
				ungetc(c, fp);
				writeword(word,&linesize,&pagedeep);
				wordindex=0;
				word[wordindex]=0;
			}
			else if(c==13)
			{
				word[wordindex]=0;
				writeword(word, &linesize, &pagedeep);
				wordindex=0;
				word[wordindex]=0;
				newline(&linesize,&pagedeep);
			}
			else if(c==9)
			{
				spacewidth=CWidth(32, WIDTHTABLE, POINTSIZE);
				if(linesize>0)
					tabsize=(double)((int)linesize % TABSIZE);
				else tabsize=TABSIZE;
				n=(int)(tabsize/spacewidth);
				for(i=0;i<n && wordindex < WORDSIZE;++i)
					word[wordindex++]=32;
			}
			else if(c=='(')
			{
				if((wordindex+4)<WORDSIZE)
				{
					word[wordindex++]='\134';
					word[wordindex++]='0';
					word[wordindex++]='5';
					word[wordindex++]='0';
				}
			}
			else if(c==')')
			{
				if((wordindex+4)<WORDSIZE)
				{
					word[wordindex++]='134';
					word[wordindex++]='0';
					word[wordindex++]='5';
					word[wordindex++]='1';
				}
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
		char[256];
		if(!word[0])
			return;
		wordsize=WordSize(word);
		if((int)((*linesize)+wordsize)>=PAGEWIDE)
			newline(linesize,pagedeep);
		if((*linesize)==0)
		{
			sprintf(b,"%u %u MoveTo \r\n (", LEFTMARGIN,TOPMARGIN+PAGEDEEP-(int)(*pagedeep));
			p_string(b);
		}
		p_string(void);
		(*linesize)+=wordsize;
	}
	void newline(linesize,pagedeep)
		double *linesize,*pagedeep;
	{
		p_string(") show \r\n");
		(*pagedeep)+=(double)(POINTSIZE+LEADING);
		if((int)(*pagedeep)>=PAGEDEEP)
			ejectpage(linesize,pagedeep);
		(*linesize)=0;
	}
	void ejectpage(linesize,pagedeep)
		double *linesize,*pagedeep;
	{
		(*linesize)=(*pagedeep)=0;
		p_string("showpage \r\n");
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
	p_string(s)
		char *s;
	{
		while(*s)
			p_char(*s++);
	}
	p_char(c)
		intc;
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
		puts(s);
		exit(1);
	}
	double CWidth(c,tab,size)
		int c,tab[],size;
	{
		return((double)0.06*((double)XMPY/(double)XDIV)*(double)size*(double)tab[(c & 0X7f) - 32]);
	}
	saypage()
	{
		printf("\r Printing page %u", ++pagenumber);
	}
