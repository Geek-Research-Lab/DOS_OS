/* Working with WordPerfect files */
/* WordPerfect is an older version of Microsoft Word */
/* An existing code of DOS for WordPerfect */

/* Some code snippets
Each line in the text buffer of the linked list is preceded by a datastructure similar to this:-
typedef struct LINK
{
struct LINK *prev;
struct LINK *next;
unsigned int length;
}
LINK;
////
Adding pointer to the buffer, where each line is preceded by LINK object

int i;
lnk=(LINK *)p;
for(i=0;i<<n;++i)
lnk=lnk->>next;
////
How LINK (lnk) works backwards?
int i;
for(i=0;i<<n;++i)
lnk=lnk->>prev;
/// */
/*MAIN PROGRAM */
#include "stdio.h"
#include "conio.h"
#include "dos.h"
#include "alloc.h"
#define HOME 71*256
#define CURSOR 72*256
#define PG_UP 73*256
#define END 79*256
#define CURSOR_DOWN 80*256
#define PG_DOWN 81*256
/*defining screen colours*/
#define SCREENBACK BLUE
#define NORMAL(YELLOW+(SCREENBACK << 4))
#define INVERSE(SCREENBACK+(LIGHTGRAY << 4))
#define BRIGHT(LIGHTCYAN+(SCREENBACK << 4))
/*defining screen dimensions*/
#define SCREENWIDE 80
#define SCREENDEEP 25
#define SCREENTOP 1
/*defining screen size*/
#define WORDSIZE 32
/*defining linked list*/
typedef struct LINK
{
struct LINK *prev;
struct LINK *next;
unsigned int length;
}
LINK;
/*defining some prototypes*/
char *loadfile(char *s);
char *farPtr(char *p, unsigned long 1);
unsigned int *screenPtr();
main(argc, argv)
	int argc;
char *argv[];
{
	char *p;
	clrscr();
	textattr(INVERSE);
	clreol();
	cprintf("WordPerfect viewer");
	textattr(NORMAL);
	if(argc<=1)
		error("path error");
	if((p=loadfile(arg[1]))!=NULL)
		showfile(p);
	farfree(p);
	textattr(0X07);
	clrscr();
}
showfile(p)
	char *p;
{
	LINK *lnk, *nextlnk;
	unsigned int c, i, j, *screenbase, *screen[SCREENDEEP];
	hidecursor();
	nextlnk=lnk=(LINK *)p;
	screenbase=screenPtr();
	for(i=0;i<SCREENDEEP;++i)
		screen[i]=screenbase+SCREENWIDE*i;
	do
	{
		nextlnk=lnk;
		for(i=0;i<SCREENDEEP;++i)
		{
			if(nextlnk==NULL)
				blank(screen[i]);
			else
			{
				showline(screen[i],nextlnk);
				nextlnk=nextlnk->next;
			}
		}
		switch(c=GetKey())
		{
		case HOME:
			while(lnk->prev!=NULL)
				lnk=lnk->prev;
			break;
		case END:
			while(lnk->next!=NULL)
				lnk=lnk->next;
			for(i=0;i<(SCREENDEEP-SCREENTOP);++i)
			{
				if(lnk->prev!=NULL)
					lnk=lnk->prev;
			}
			break;
		case CURSOR_DOWN:
			for(i=0;i<(SCREENDEEP-SCREENTOP);++i)
			{
				if(nextlnk->next!=NULL)
					nextlnk=nextlnk->next;
			}
			if(nextlnk->next!=NULL)
				lnk=lnk->next;
			break;
		case CURSOR_UP:
			if(lnk->prev!=NULL)
				lnk=lnk->prev;
			break;
		case PG_UP:
			for(i=0;i<(SCREENDEEP-SCREENTOP);++i)
				if(lnk->prev!=NULL)
					lnk=lnk->prev;
				else break;
		}
		break;
		case PG_DOWN:
			for(i=0;i<(SCREENDEEP-SCREENTOP);++i)
				nextlnk=lnk;
			for(j=0;j<(SCREENDEEP-SCREENTOP);++j)
			{
				if(nextlnk->next!=NULL)
					nextlnk=nextlnk->next;
			}
			if(nextlnk->next!=NULL)
				break;
			if(lnk->next==NULL)
				break;
			if(lnk->next!=NULL)
				lnk=lnk->next;
			else break;
	}
	break;
}
}
while(c!=27);
	}
	showline(line,lnk)
		unsigned int *line;
	LINK *link;
	{
		char *p;
		int i, c;
		p=(char *)lnk+sizeof(LINK);
		for(i=0;i<SCREENWIDE;++i)
		{
			if(i < lnk->length)
			{
				c=*p++;
				if(c & 0X0080)
					*line++=((c & 0X7f) | (BRIGHT << 8));
				else *line++=(c | (NORMAL << 8));
			}
			else *line++=(32 | (NORMAL << 8));
		}
	}
	blankline(line)
		unsigned int *line;
	{
		int i;
		for(i=0;i<SCREENWIDE;++i)
			*line++=(32 | (NORMAL << 8));
	}
	char *loadfile(s)
		char *s;
	{
		LINK *lnk, *nextlnk;
		FILE *fp;
		char *p, *line, b[WORDSIZE];
		unsigned long freemem, offset;
		unsigned int wordtext=0, effects=0;
		int i, n, c=0;
		freemem=coreleft()-2048L;
		if((p=farmalloc(freemem))==NULL)
		{
			error("can't allocate memory");
			return(NULL);
		}
		if((fp=fopen(s,"rb"))==NULL)
		{
			error("can't open source file");
			farfree(p);
			return(NULL);
		}
		if(fread(b,1,4,fp)!=4 || memcmp(b,"\377WPC",4))
		{
			error("bad file header");
			fclose(fp);
			farfree(p);
			return(NULL);
		}
		if(fread((char *)&offset,1,sizeof(unsigned long),fp)!=sizeof(unsigned long))
		{
			error("bad file offset");
			fclose(fp);
			farfree(p);
			return(NULL);
		}
		fseek(fp,offset,SEEK_SET);
		lnk=(LINK *)p;
		line=farPtr(p,(unsigned long) sizeof(LINK));
		memset((char *) lnk,0,sizeof(LINK));
		do
		{
			do
			{
				if(c > 0X007f && c!=EOF)
				{
					switch(c)
					{
						/*special character */
					case 0X00c0:
						for(i=0;i<3;++i)
							fgetc(fp);
						break;
						/*alignment*/
					case 0X00c1:
						for(i=0;i<8;++i)
							fgetc(fp);
						break;
						/*indent*/
					case 0X00c2:
						for(i=0;i<8;++i)
							fgetc(fp);
						break;
						/*effects on*/
					case 0X00c3:
						effects!=(1<<fgetc(fp));
						fgetc(fp);
						break;
						/*effect off*/
					case 0X00c4:
						effects&=~(1<<fegtc(fp));
						/*block protect*/
					case 0X00c5:
						for(i=0;i<4;++i)
							fgetc(fp);
						break;
						/*end of indent*/
					case 0X00c6:
						for(i=0;i<5;++i)
							fgetc(fp);
						break;
						/*different display*/
					case 0X00c7:
						for(i=0;i<6;++i)
							fgetc(fp);
						break;
						/*subfunction*/
					default:
						if(c <= 0X00d0)
						{
							fgetc(fp);
							n=fgetc(fp)+(fgetc(fp)<<8);
							for(i=0;i<n;++i)
								fgetc(fp);
						}
						break;
					}
				}
				else
				{
					if(c!=EOF && c>32)
					{
						if(effects)
							c|=0X0080;
						b[wordindex++]=c;
					}
				}
				c=fgetc(fp);
				if(c=0X000d)
					c=32;
			}
			while(wordindex < WORDSIZE && c > 32 && c!=EOF);
			while(wordindex < WORDSIZE && c==32)
			{
				if(effects)
					c|=0X0080;
				b[wordindex++]=c;
				c=fgetc(fp);
			}
			/*adding context word to this line*/
			if((ln->length+wordindex)<SCREENWIDE)
			{
				memcpy(line+lnk->length,b,wordindex);
				lnk->length+wordindex;
				freemem-=((long) sizeof(LINK)+(long)wordindex);
			}
			/*Whoa! But, if not... then trying to link to next line*/
			if(c==0X0a||(lnk->length+wordindex)>=SCREENWIDE)
			{
				if(c!=EOF)
				{
					lnk->next=(LINK *)farPtr((char *)lnk,(unsigned long)(lnk->length+sizeof(LINK)));
					nextlnk=lnk->next;
					memset((char *)nextlnk,0,sizeof(LINK));
					nextlnk->prev=lnk;
					lnk=nextlnk;
					line=(char *)lnk+sizeof(LINK);
				}
			}
			wordindex=0;
		}
		while(c!=EOF && freemem > (long)WORDSIZE);
		fclose(fp);
		return(p);
		}
		/* Gonna be done...still coding more lines..lol */
		char *s;
		{
			printf("\r\nERROR:%s\r\n",s);
			exit(1);
		}
		/*returns pointer along with long integer*/
		char *farPtr(p,1);
		char *p;
		unsigned long 1;
		{
			unsigned int seg, off;
			seg=FP_SEG(p);
			off=FP_OFF(p);
			seg+=(off/16);
			off&=0X000f;
			off+=(unsigned int)(l & 0X00fL);
			seg+=(l/16L);
			p=MK_FP(seg,off);
			return(p)
		}
		unsigned int *screenPtr()
		{
			union REGS r;
			r.x.ax=0X0f00;
			int86(0X10,&r,&r);
			if(r.h.a1==0X07)
				return(MK_FP(0Xb000,r.h.bh*0X1000));
			else return(MK_FP(0Xb800,r.h.bh*0X1000));
		}
		GetKey()
		{
			int c;
			c=getch();
			if(!(c & 0X00ff))
				c=getch()<<8;
			return(c);
		}
		hidecursor()
		{
			union REGS r;
			r.x.ax=0X0f00;
			int86(0X10,&r,&r);
			r.x.ax=0X0200;
			r.x.dx=0X1a00;
			int86(0X10,&r,&r);
		}
