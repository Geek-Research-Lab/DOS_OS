#include "stdio.h"
#include "stdlib.h"
#include "dos.h"
#include "conio.h"
#define columnwide 193 /*width of column*/
#define pointsize 4
#define titlesize 4 /*page number size*/
#define MAX 256 /*useful constant*/
#define MAXWIDE 80 /*maximum line width*/
FILE *fp; /*file pointer*/
char footer[MAX+1]=""; /*Footer text*/
chat pathname[MAX+1]=""; /*file to print*/
unsigned int line=0; /*line counter*/
unsigned int page=11; /*page counter*/
unsigned int pagetop=770; /*top of page*/
unsigned int pageleft=18; /*left of page*/
unsigned int tabsize=8; /*width of tab*/
unsigned int pagelines; /*lines in pages*/
unsigned int column; /*column counter*/
unsigned int maxcolumn=3; /*column limits*/
unsigned int printerport=0; /*port to print*/
main(argc,argv)
	int argc;
char *argv[];
{
	char b[MAX+1];
	int c;
	puts("PostScript Small laser text printer");
	pagelines=(pagetop-30)/pointsize;
	if(argc<=1)
		error("arguments: a path to text filr \n" "switches \F<text> establishes a footer\n" "\P<number> sets printer port\n" "\T<number> sets tab size\n");
	for(c=1;c<argc;++c)
	{
		if(arg[c][0]=='/')
			arg(arg[c]);
		else strncpy(pathname,argv[c],MAX);
	}
	if((fp=fopen(pathname,"rb"))==NULL)
		error("error opening the source file");
	do
	{
		startpage();
		printf("Printing page %d.\r \n",page);
		do
		{
			line=0;
			do
			{
				c=getst(b,MAX,fp);
				b[MAXSIZE]=0;
				writeline(b);
			}
			while(c!=EOF && line++ < pagelines);
			++column;
		}
		while(c!=EOF && column <= maxcolumn);
		endpage();
		++page;
	}
	while(c!=EOF);
	fclose(fp);
}
arg(s) /*handle a command line argument*/
	char *s;
{
	switch(toupper(s[1]))
	{
	case 'F':
		strncpy(footer,s+2,MAX);
		break;
	case 'P':
		printerport=atoi(s+2);
		break;
	case 'T':
		tabsize=min(atoi(s+2),16);
		break;
	}
}
writeline(s) /*write a line of text*/
	char *s
{
	char b[MAX];
	sprintf(b,"%d %d moveto(%s) show \r\n",pageleft+(column*columnwide),pagetop-(pointersize*line),s);
	p_string(b);
}
getst(b,l,fp) /*get a line of text from file fp */
	char *b;
int l;
FILE *fp;
{
	int c, r=0;
	-l;
	do
	{
		c=fgetc(fp);
		switch(c & 0X7f)
		{
		case 9: /*tab*/
			do
			{
				*b++=0X20;
				++r;
			}
			while(((r+0) % tabsize) != 0 && r < 1);
			break;
		case 13:
			/*cr*/
			c=0;
			break;
		case '\134':
			*b++='\134';
			*b++='1';
			*b++='3';
			*b++='4';
			r+=4;
			break;
		case '<':
			*b++='\134';
			*b++='0';
			*b++='7';
			*b++='4';
			r+=4;
			break;
		case '>':
			*b++='\134';
			*b++='0';
			*b++='7';
			*b++='6';
			r+=4;
			break;
		case '(':
			*b++='\134';
			*b++='0';
			*b++='5';
			*b++='0';
			r+=4;
			break;
		case ')':
			*b++='\134';
			*b++='0';
			*b++='5';
			*b++='1';
			r+=4;
			break;
		default:
			if((c & 0X7f) >= 32)
			{
				*b++=(c & 0X7f);
				++r;
			}
			break;
		}
	}
	while(r<1 && c!=0 && c!=EOF);
	*b=0;
	return(c);
}
startpage() /*start the page and get the font*/
{
	char b[MAX];
	sprintf(b,"/Courier findfont [%d 0 0 %d 0 0] makefont setfont\r\n",pointsize,pointsize);
	p_string(b);
	p_string("gsave\r\n");
	line=0;
	column=0;
}
endpage() /*end the page and eject it*/
{
	char b[MAX];
	sprintf(b,"/Courier findfont [%d 0 0 %d 0 0] makefont setfont\r\n",titlesize,titlesize);
	p_string(b);
	sprintf(b,"%d 20 moveto(PAGE %d %s) show \r\n",pageleft,page,footer);
	p_string(b);
	p_string("grestore\r\n");
	p_string("showpage\r\n");
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
	union REGS r;
	do
	{
		r.h.ah=2;
		r.x.dx=printerport;
		int86(0X17,&r,&r);
	}
	while(!(r.h.ah & 0X80));
	r.h.ah=0;
	r.h.al=0;
	r.x.dx=printerport;
	int86(0X17,&r,&r);
}
error(s)
	char *s;
{
	puts(s);
	exit(1);
}

