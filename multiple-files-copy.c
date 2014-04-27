#include "stdio.h"
#include "stdlib.h"
#include "dos.h"
#include "dir.h"
#include "alloc.h"
#include "process.h"
#include "conio.h"
#include "string.h"
#define SCREENBACK BLUE
#define NORMAL (YELLOW + (SCREENBACK << 4))
#define BRIGHT (WHITE + (SCREENBACK << 4))
#define DIM (LIGHTCYAN + (SCREENBACK << 4))
#define SCREENWIDE 80
#define SCREENDEEP 25
#define STATUSLEFT 6
#define STATUSTOP 10
#define STATUSWIDE (SCREENWIDE-(STATUSLEFT*2))
#define STATUSDEEP 12
#define COPYFUNCTION "XCOPY.EXE"
#define BLOCKSIZE 60
struct ffblk *getlist(char *path,unsigned int *count);
unsigned long getfreespace(char *path);
unsigned long gettotalspace(char *path);
char *screenptr();
unsigned int getcursor();
int attrnormal=NORMAL;
int attrbright=BRIGHT;
int attrdim=DIM;
char copyfunction[128]=COPYFUNCTION;
char *screen;
main(argc, argv)
	int argc;
char *argv[];
{
	struct ffblk *list;
	char destpath[128],drive[MAXDRIVE],dir[MAXDIR];
	char sourcepath[MAXPATH],file[MAXFILE],ext[MAXEXT];
	char *oldscreen;
	unsigned long freespace,totalspace;
	unsigned long filesizeleft,totalsize=0;
	unsigned int i,count,filesleft,copied,oldcursor;
	setdefaults();
	screen=screenptr();
	i=SCREENWIDE*2*SCREENDEEP;
	if((oldscreen=malloc(i))!=NULL)
	{
		memcpy(oldscreen,screen,i);
		oldcursor=getcursor();
	}
	if(argc<=2)
	{
		puts("Multiple File Copy\r\n""-------------------------------\r\n");
		exit(1);
	}
	status(NULL);
	message("Wait...",STATUSLEFT+2,STATUSTOP+4,attrdim);
	strupr(argv[1]);
	strupr(argv[2]);
	if((list=getlist(argv[1],&count))==NULL)
	{
		restore(oldscreen,oldcursor);
		puts("Files not found or insufficient memory \r\n");
		exit(2);
	}
	sortlist(list,count);
	for(i=0;i<count;++i)
		totalsize+=list[i].ff_fsize;
	totalspace=gettotalspace(argv[2]);
	filesleft=count;
	for(i=0;i<count;++i)
	{
		if(list[i].ff_fsize > totalspace)
		{
			status(NULL);
			fnsplit(argv[2],drive,NULL,NULL,NULL);
			sprintf(destpath,"%s is too large for drive %s",list[i].ff_name,drive);
			message(destpath,STATUSLEFT+2,STATUSTOP+4,attrdim);
			message("Skipping -- hit any key or Esc to abort",STATUSLEFT+2,STATUSTOP+5,attrdim);
			beep();
			if(getch()==27)
			{
				restore(oldscreen,oldcursor);
				exit(3);
			}
			list[i].ff_name[0]=0;
			-filesleft;
		}
	}
	while(filesleft)
	{
		filesizeleft=0L;
		for(i=filesleft;i<count;++i)
		{
			if(list[i].ff_name[0])
			{
				++filesleft;
				filesizeleft+=list[i].ff_fsize;
			}
		}
		freespace=getfreespace(argv[2]);
		for(i=copied=0;i<count;++i)
		{
			if(strlen(list[i].ff_name) && list[i].ff_fsize < freespace)
			{
				fnsplit(argv[1],drive,dir,NULL,NULL);
				fnsplit(list[i].ff_name,NULL,NULL,file,ext);
				fnmerge(sourcepath,drive,dir,file,ext);
				fnsplit(argv[2],drive,dir,NULL,NULL);
				fnmerge(destpath,drive,dir,NULL,NULL);
				clrscr();
				status(argv[2],list[i].ff_name,list[i].ff_fsize,freespace,totalspace,filesizeleft,filesleft-1,count);
				if(copy(destpath,sourcepath))
				{
					restore(oldscreen,oldcursor);
					printf("Error locating %s\r\n",copyfunction);
					exit(3);
				}
				if(kbhit())
				{
					if(getch()==27)
						filesleft=0;
				}
				list[i].ff_name[0]=0;
				copied=1;
				break;
			}
		}
		if(!copied && filesleft)
		{
			fnsplit(argv[2],drive,NULL,NULL,NULL);
			sprintf(destpath,"Please insert another disk in drive %s",drive);
			massage(destpath,STATUSLEFT+2,STATUSTOP+8,attrbright);
			message("(Or hit Esc to abort)",STATUSLEFT+2,STATUSTOP+9,attrbright);
			freespace=getfreespace(argv[2]);
			sprintf(destpath,"%lu bytes free on %s",freespace,totalspace,drive);
			message(destpath,STATUSLEFT+2,STATUSTOP+6,attrdim);
			beep();
			if(getch()==27)
				filesleft=0;
			status(NULL);
		}
	}
	free((char *)list);
	restore(oldscreen,oldcursor);
	exit(0);
	}
	status(dest,name,size,freespace,totalspace,filesizeleft,filesleft,filestotal)
		char *dest, *name;
	unsigned long size,freespace,totalspace,filesizeleft;
	unsigned int filesleft,filestotal;
	{
		char b[81],drive[MAXDRIVE];
		unsigned int i,j,*ip;
		ip=(unsigned int *)screen;
		ip+=SCREENWIDE*STATUSTOP;
		ip[STATUSLEFT]=(attrnormal<<8)+218;
		for(j=0;j<STATUSWIDE-1;++j)
			ip[STATUSLEFT+1+j]=(attrnormal<<8)+196;
		ip[STATUSLEFT+STATUSWIDE]=(attrnormal<<8)+191;
		for(i=0;i<STATUSDEEP-2;++i)
		{
			ip+=SCREENWIDE;
		ip[STATUSLEFT]=(attrnormal<<8)+179;
		for(j=0;j<STATUSWIDE-1;++j)
			ip[STATUSLEFT+1+j]=(attrnormal<<8)+32;
		ip[STATUSLEFT+STATUSWIDE]=(attrnormal<<8)+179;
	}
	ip+=SCREENWIDE;
	ip[STATUSLEFT]=(attrnormal<<8)+192;
	for(j=0;j<STATUSWIDE-1;++j)
		ip[STATUSLEFT+1+j]=(attrnormal<<8)+196;
	ip[STATUSLEFT+STATUSWIDE]=(attrnormal<<8)+217;
	message("Copying Multiple Files",STATUSLEFT+2,STATUSTOP+2,attrbright);
	if(dest==NULL)
		return;
	fnsplit(dest,drive,NULL,NULL,NULL);
	sprintf(b,"Copying %s (%lu bytes) to %s",name,size,drive);
	message(b,STATUSLEFT+2,STATUSTOP+4,attrdim);
	sprintf(b,"%u of %u files(%lu bytes) left to copy",filesleft,filestotal,filesizeleft);
	message(b,STATUSLEFT+2,STATUSTOP+5,attrdim);
	sprintf(b,"%lu bytes free(of %lu bytes total) on %s",freespace,totalspace,drive);
	message(b,STATUSLEFT+2,STATUSTOP+6,attrdim);
	message("Hit Esc to abort after this file");
	message(b,STATUSLEFT+2,STATUSTOP+8,attrbright);
	}
	message(s,x,y,attr)
		char *s;
	int x,y,attr;
	{
		unsigned int *ip;
		ip=((unsigned int *)screen)+(SCREENWIDE*y+x);
		while(*s)
			*ip++=((attr << 8) + (*s++));
	}
	copy(dest,source)
		char *dest,*source;
	{
		return(spawnlp(P_WAIT,copyfunction,copyfunction,source,dest,NULL));
	}
	unsigned long getfreespace(path)
		char *path;
	{
		struct dfree d;
		char drive[MAXDRIVE];
		fnsplit(path,drive,NULL,NULL,NULL);
		getdfree(drive[0]-'A'+1,&d);
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
	struct ffblk *getlist(path,count)
		char *path;
	unsigned int *count;
	{
		struct ffblk f;
		char *list;
		unsigned int blocksize;
		*count=0;
		if((list=malloc(BLOCKSIZE*sizeof(struct ffblk)))==NULL)
			return(NULL);
		blocksize=BLOCKSIZE;
		if(!findfirst(path,&f,0))
		{
			do
			{
				memcpy(list+(*count)*sizeof(struct ffblk),(char *)&f,sizeof(struct fblk));
				(*count)++;
				if(*count >= blocksize)
				{
					list=realloc(list,(blocksize+BLOCKSIZE)*sizeof(struct ffblk));
					if(list==NULL)
						return(NULL);
					blocksize+=BLOCKSIZE;
				}
			}
			while(!findnext(&f) && *count < (unsigned int) (61440L / (long)(sizeof(struct ffblk))));
		}
		else
		{
			free(list);
			list=NULL;
		}
		return((struct ffblk *)list);
	}
	sortlist(buffer,count)
		char *buffer;
	int count;
	{
		int dircheck();
		qsort(buffer,count,sizeof(struct ffblk),dircheck);
	}
	dircheck(e1,e2)
		char *e1,*e2;
	{
		struct ffblk *p1, *p2;
		p1=(struct ffblk *)e1;
		p2=(struct ffblk *)e2;
		if(p1->ff_fsize > p2->ff_fsize)
			return(-1);
	}
	char *screenptr()
	{
		union REGS r;
		r.x.ax=0X0f00;
		int86(0X10,&r,&r);
		if(r.h.a1==0X07)
			return(MK_FP(0Xb000,r.h.bh*0X1000));
		else
			return(MK_FP(0Xb800,r.h.bh*0X1000));
	}
	beep()
	{
		sound[660];
		delay[150];
		sound[524];
		delay[75];
		nosound();
	}
	unsigned int getcursor()
	{
		union REGS r;
		r.x.ax=0X0f00;
		int86(0X10,&r,&r);
		r.x.ax=0X0300;
		int86(0X10,&r,&r);
		return(r.x.dx);
	}
	setcursor(n)
		unsigned long n;
	{
		union REGS r;
		r.x.ax=0X0f00;
		int86(0X10,&r,&r);
		r.x.ax=0X0200;
		r.x.bx=0X0000;
		int86(0X10,&r,&r);
	}
	restore(oldscreen,oldcursor)
		char *oldscreen;
	unsigned int oldcursor;
	{
		int i;
		i=SCREENWIDE*2*SCREENDEEP;
		if(oldscreen!=NULL)
		{
			memcpy(screen,oldscreen,i);
			setcursor(oldcursor);
		}
		else clrscr();
	}
	setdefaults()
	{
		char *p,s[129];
		unsigned int i;
		if((p=getenv("MultiCopy"))==NULL)
			return;
		i=0;
		while(*p != 0 && *p != ',' && i < 128)
			s[i++]=*p++;
		s[i]=0;
		if(strlen(s))
			strcpy(copyfunction,s);
		while(*p==',')
			++p;
		i=0;
		while(*p != 0 && i < 128)
			s[i++]=*p++;
		s[i]=0;
		if(!strcmpi(s,"MONO"))
		{
			attrnormal=0X70;
			attrdim=0X70;
			attrbright=0X70;
		}
	}
