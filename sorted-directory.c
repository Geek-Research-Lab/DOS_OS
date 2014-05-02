#include "stdio.h"
#include "dir.h"
#include "dos.h"
#include "alloc.h"
#include "conio.h"
#include "time.h"
#include "stdio.h"
#define TEXTCOLOUR(forground, background) (foreground | (background << 4))
#define BACKGROUND TEXTCOLOUR(WHITE,BLUE)
#define BANNER TEXTCOLOUR(BLACK,RED)
#define FILENAMETEXT TEXTCOLOUR(WHITE,BLUE)
#define FILESIZETEXT TEXTCOLOUR(YELLOW,BLUE)
#define DIRNAMETEXT TEXTCOLOUR(LIGHTGRAY,BLUE)
#define FILETIMETEXT TEXTCOLOUR(CYAN,BLUE)
#define HOME 0X4700
#define CURSOR_UP 0X4800
#define END 0X4f00
#define CURSOR_DOWN 0X5000
#define PG_UP 0X4900
#define PG_DOWN 0X5100
#define SCREENWIDE 80
#define SCREENDEEP 25
#define BLOCKSIZE 128
#define ENTRIESPERLINE 2
#define ENTRYSIZE(SCREENWIDE/ENTRIESPERLINE)
#define PAGESIZE((SCREENDEEP*2)/3)
struct ffblk *GetList(char *path,unsigned int *count,unsigned long *size);
char *formatlong(long l);
char *filetime(struct ffblk *fb);
char *screenPtr();
unsigned long getfreespace(char *path);
unsigned long gettotalspace(char *path);
char *screen;
main(argc, argv)
int argc;
char *argv[];
{
	struct ffblk *list=NULL;
	unsigned long size, freespace=0L, totalspace;
	unsigned int count=0, line=0;
	char drive[MAXDRIVE],dir[MAXDIR],file[MAXFILE],ext[MAXEXT];
	char b[129];
	int c;
	clrscr();
	screen=screenPtr();
	DrawString(0,0,"Wait...",0X07);
	if(argc<=1)
	{
		strcpy(b,"*.*");
	}
	else
	{
		strupr(argv[1]);
		fnsplit(argv[1],drive,dir,file,ext);
		if(file[0]==0)
			strcpy(file,"*");
		if(ext[0]==0)
			strcpy(ext,"*.*");
		fnmerge(b,drive,dir,file,ext);
	}
	list=GetList(b,&count,&size);
	freespace=getfreespace(b);
	totalspace=gettotalspace(b);
	if(list==NULL)
		error("File not found");
	SortList(list,count);
	HideCursor();
	ClearScreen(BACKGROUND);
	ClearLine(0,BANNER);
	DrawString(0,0,"Sorted Directory",BANNER);
	ClearLine(SCREENDEEP-1,BANNER);
	strcpy(b,formatlong(size));
	strcat(b,"USED \376");
	strcat(b,formatlong((long)count));
	strcat(b, "FILES \376");
	strcat(b,formatlong(freespace));
	strcat(b,"FREE \376");
	strcat(b,formatlong(totalspace));
	strcat(b,"TOTAL");
	DrawString((SCREENWIDE-strlen(b))/2,SCREENDEEP-1,b,BANNER);
	do
	{
		ShowPage(list, line, count)
			switch(c=GetKey())
		{
			case HOME:
				line=0;
				break;
			case END:
				line=1+(count+ENTRIESPERLINE) - (SCREENDEEP - 1);
				break;
			case CURSOR_UP:
				if(line > 0)
					-line;
				break;
			case CURSOR_DOWN:
				if(line < ((count/ENTRIESPERLINE)-SCREENDEEP+2))
					++line;
				break;
			case PG_UP:
				if(line > PAGESIZE)
					line-=PAGESIZE;
				else
					line=0;
				break;
			case PG_DOWN:
				if((line+PAGESIZE) < ((count/ENTRIESPERLINE)-SCREENDEEP+1))
					line+=PAGESIZE;
				else
					line=1+(count/ENTRIESPERLINE)-(SCREENDEEP-1);
				break;
		}
	}
	while(c!=27);
	ClearScreen(0X07);
	gotoxy(1,24);
	ShowPage(list,line,count)
		struct ffblk *list;
		unsigned int line,count;
		{
			char *p,b[81];
			int i,j,n;
			list+=(line*ENTRIESPERLINE);
			count-=(line*ENTRIESPERLINE);
			for(i=0;i<SCREENDEEP-2;++i)
			{
				for(j=0;j<ENTRIESPERLINE;++j)
				{
					memset(b,32,ENTRYSIZE);
					b[ENTRYSIZE]=0;
					DrawString(j*ENTRYSIZE,i+1,b,FILENAMETEXT);
				if(count)
				{
					if(list->ff_attrib & 0X0010)
					{
						sprintf(b,"\\%s",list->ff_name);
						DrawString(j*ENTRYSIZE+1,i+1,b,DIRNAMETEXT);
					}
					else
					{
						DrawString(j*ENTRYSIZE,i+1,list->ff_name,FILENAMETEXT);
						n=strlen(p=formatlong(list->ff_fsize));
						DrawString(j*ENTRYSIZE+24-n,i+1,p,FILESIZETEXT);
						DrawString(j*ENTRYSIZE+25,i+1,filetime(list),FILETIMETEXT);
					}
					++list;
					-count;
				}
				}
			}
		}
		struct ffblk *GetList(path,count,size)
			char *path;
		unsigned int *count;
		unsigned long *size;
		{
			struct ffblk f;
			char *list,drive[MAXDRIVE],dir[MAXDIR],b[144];
			unsigned int blocksize;
			*count=0;
			*size=0L;
			if((list=malloc(BLOCKSIZE*sizeof(struct ffblk)))==NULL)
				return(NULL);
			blocksize=BLOCKSIZE;
			if(!findfirst(path,&f,0X0000))
			{
				do
				{
					memcpy(list+(*count)*sizeof(struct ffblk),(char *)&f,sizeof(struct ffblk));
					(*size)+=f.ff_Size;
					(*count)++;
					if(*count >= blocksize)
					{
						list=realloc(list,(blocksize+BLOCKSIZE)*sizeof(struct ffblk));
						if(list==NULL)
							return(NULL);
						blocksize+=BLOCKSIZE;
					}
				}
				while(!findnext(&f) && *count < (unsigned int) (61440L / (long) (sizeof(struct ffblk))));
			}
			fnsplit(path,drive,dir,NULL,NULL);
			fnmerge(b,drive,dir,"*",".*");
			if(!findfirst(b,&f,0X0010))
			{
				do
				{
					if(f.ff_attrib && 0X0010 && f.ff_name[0] != '.')
					{
						memcpy(list+(*count)*sizeof(struct ffblk),(char *)&f,sizeof(struct ffblk));
						(*count)++;
						if(*count >= blocksize)
						{
							list=realloc(list,(blocksize+BLOCKSIZE)*sizeof(struct ffblk));
							if(list==NULL)
								return(NULL);
							blocksize+=BLOCKSIZE;
						}
					}
				}
				while(findnext(&f) && *count < (unsigned int) (61440L / (long) (sizeof(struct ffblk))));
			}
			if(*count==0)
				free(list);
			list=NULL;
		}
		return((struct ffblk *)list);
		}
		SortList(buffer,count)
			struct ffblk *buffer;
			int count;
			{
				int dircheck()
					qsort(buffer,count,sizeof(struct ffblk),diecheck);
			}
			diecheck(e1, e2)
				char *e1, *e2;
			{
				struct ffblk *p1, *p2;
				p1=(struct ffblk *)e1;
				p2=(struct ffblk *)e2;
				return(strcmp(p1->ff_name.p2->ff_name));
			}
			DrawString(x,y,text,attr)
				int x,y;
			char *text;
			int attr;
			{
				unsigned int a, *ip;
				ip=(unsigned int *) (screen+(y*160+x*2));
				a=attr<<8;
				while(*text)
					*ip++=*text++ | a;
			}
			HideCursor()
			{
				union REGS r;
				r.x.ax=0X0f00;
				int86(0X10,&r,&r);
				r.x.ax=0X0200;
				r.x.dx=(SCREENDEEP+1)<<8;
				int86(0X10,&r,&r);
			}
			char *screenPtr()
			{
				union REGS r;
				r.x.ax=0X0f00;
				int86(0X10,&r,&r);
				if(r.h.a1==0X07)
					return(MK_FP(0Xb000,r.h.bh*0X1000));
				else
					return(MK_FP(0Xb800,r.h.bh*0X1000));
			}
			ClearScreen(attr)
				int attr;
			{
				unsigned int i,a,*ip;
				ip=(unsigned int *)screen;
				a=attr<<8;
				for(i=0;i<SCREENWIDE*SCREENDEEP;++i)
					*ip++=32 | a;
			}
			error(s)
				char *s;
			{
				puts(s);
				exit(1);
			}
			GetKey()
			{
				int c;
				c=getch();
				if(!(c & 0X00ff))
					c=getch()<<8;
				return(c);
			}
			ClearLine(y,attr)
				int y,attr;
			{
				unsigned int a,i,*ip;
				ip=(unsigned int *) (screen+(y*160));
				a=(attr<<8) | 32;
				for(i=0;i<SCREENWIDE;++i)
					*ip++=a;
			}
			char *formatlong(1)
				long l;
			{
				static char b[24];
				char s[16];
				int i,j,n;
				sprintf(s,"%lu",l);
				strrev(s);
				n=strlen(s);
				for(i=0;i<n;++i)
				{
					b[j++]=s[i];
					if(!((i+1) % 3) && i < (n-1))
						b[j++]='.';
				}
				b[j]=0;
				strrev(b);
				return(b);
			}
			char *filetime(fb)
				struct ffblk *fb;
				{
					static char b[24];
					struct date dt;
					struct time tm;
					tm.ti_hund=0;
					tm.ti_sec=(char)((fb->ff_ftime & 0x001f)*2);
					tm.ti_min=(char)((fb->ff_ftime>>5) & 0X003f);
					tm.ti_hour=(char)((fb->ff_ftime>>1) & 0X001f);
					dt.da_day=(char)(fb->ff_fdate & 0X001f);
					dt.da_mon=(char)((fb->ff_fdate>>5) & 0X000f);
					dt.da_year=(((fb->ff_fdate>>9) & 0X003f) +80) % 99;
					sprintf(h,"%02d-%02d-%02d-%02d:%02d",dt.da_mon,dt.da_day,dt.da_year,tm.ti_hour,tm.ti_min);
					return(0);
				}
				unsigned long getfreespace(path)
					char *path;
				{
					struct dfree d;
					char drive[MAXDRIVE];
					fnsplit(path,drive,NULL,NULL,NULL);
					getdfreen(drive[0] - 'A'+1, &d);
					return((long)d.df_avail*(long)d.df_bsec*(long)d.df_sclus);
				}
				unsigned long gettotalspace(path)
					char *path;
				{
					struct dfree d;
					char drive[MAXDRIVE];
					fnsplit(path,drive,NULL,NULL,NULL);
					getdfree(drive[0]-'A'+1,&d);
					return((long)d.df_total*(long)d.df_bsec*(long)d.df_sclus);
				}
