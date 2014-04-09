#include<stdio.h>
#include<stdlib.h>
#include<alloc.h>
#include<conio.h>
#include"dos.h"
#include"string.h"
#define TRUE 1
#define FALSE 0
#define SCREENWIDE 80
#define SCREENDEEP 25
#define TEXTCOLOUR(foreground,background) | (foreground | (background<<4))
#define BACKGROUND TEXTCOLOUR(WHITE,BLUE)
#define BANNER TEXTCOLOUR(BLACK,RED)
#define TEXT TEXTCOLOUR(WHITE,BLUE)
#define HOME 0X4700
#define CURSOR_UP 0X4800
#define END 0X4f00
#define CURSOR_DOWN 0X5000
#define PG_UP 0X4900
#define PG_DOWN 0X5100
#define LEFT 2
#define TOP 2
typedef struct
{
	/*field names*/
	char Name[12];
	/*field type*/
	char Type;
	/*field length*/
	int length;
	/*decimal location*/
	int Dec;
}
DBFIELD;
typedef struct
{
	/*file struct version*/
	int Version;
	/*last updated*/
	char update[3];
	/*records present in file*/
	long NumRex;
	/*header length*/
	int HdrLen;
	/*record length*/
	int RecLen;
	/*number of fields present in records*/
	int NumFlds;
	/*number of bytes present*/
	long FileSize;
	/*file holding data*/
	FILE *file;
	/*field data array pointer*/
	DBFIELD (*fld)[];
}
DBFILE;
typedef struct
{
	int Version;
	char update[3];
	long NumRex;
	int HdrLen;
	int RecLen;
	char Reserved[20];
}
DBHEADERMASK;
typedef struct
{
	char Name[11];
	char Type;
	char Reserved[4];
	unsigned char Length;
	unsigned char Dec;
	char Reserved2[14];
}
DBFIELDMASK;
char *screenPtr();
DBFILE *dbfOpen(char *fname);
#define Day_upd update[2] /*last update: day*/
#define Mon_upd update[1] /*last update: month*/
#define Yr_upd update[0] /*last update: year*/
#define F(x) (*x->fld)
char *screen;
main(argc,argv)
	int argc;
char argv[];
{
	DBFILE *dbf;
	char *buf;
	long page=0L,oldpage=-1L;
	int c;
	screen=screenPtr();
	if(argc<=1)
		error("argument: path to a database file");
	if((dbf=dbfOpen(argv[1]))==NULL)
		error("unable to open the file");
	do
	{
		if(page!=oldpage)
			showbanner(page);
		oldpage=page;
		if(page==0L)
			dbfShowHeader(dbf,LEFT,TOP);
		else if(page==1L)
			dbfShowStructure(dbf,LEFT,TOP);
		else
		{
			if((buf=malloc(dbf->RecLen+2))==NULL)
				error("unable to allocate memory");
			if(!dbfReadRec(dbf,page-1,buf))
				DrawString(LEFT,TOP,"error reading the record",TEXT);
			else dbfShowRecord(dbf,page-1,buf,LEFT,TOP);
			free(buf);
		}
		switch(c=GetKey())
		{
		case HOME:
			page=0L;
			break;
		case END:
			page=dbf->NumRex+1L;
			break;
		case CURSOR_UP:
			if(page>0L)
				-page;
			break;
		case CURSOR_DOWN:
			if((page+1L)<-(dbf->NumRex+1L))
				++page;
			break;
		case PG_UP:
			if((page -10L)>0L)
				page-=10L;
			else
				page=0L;
			break;
		case PG_DOWN:
			if((page+10L)<=(dbf->NumRex+1L))
				page+=10L;
			else page=dbf->NumRex+1L;
			break;
		}
	}
	while(c!=27);
}
DBFILE *dbfOpen(fname)
	char *fname;
{
	int i;
	DBFILE *r;
	DBHEADERMASK h;
	DBFIELDMASK FldTmp;
	if((r=(DBFILE *)malloc(sizeof(DBFILE)))==NULL)
		return(NULL);
	if((r->file=fopen(fname,"rb"))==NULL)
	{
		free((char *)r);
		return(NULL);
	}
	if(fread((char *)&h,l,sizeof(DBHEADERMASK),r->file)!=sizeof(DBHEADERMASK))
	{
		free((char *)r);
		return NULL;
	}
	r->Version=h.Version & 0X07;
	r->Day_upd=h.Day_upd;
	r->Mon_upd=h.Mon_upd;
	r->Yr_upd=h.Yr_upd;
	r->NumRex=h.NumRex;
	r->HdrLen=h.HdrLen;
	r->RecLen=h.RecLen;
	r->NumFlds=(h.HdrLen - 33)/32;
	r->FileSize=(long)h.HdrLen+(long)h.RecLen*(long)h.NumRex+1L;
	if(r->Version!=3)
	{
		free((char *)r);
		return NULL;
	}
	if((r->fld=malloc(sizeof(DBFIELD)*r->NumFlds))==NULL)
	{
		free((char *));
		return(NULL);
	}
	for(i=0;i<r->NumFlds;i++)
		if(fread((char *)&FldTmp,1,sizeof(DBFIELDMASK),r->file)!=sizeof(DBFIELDMASK))
		{
			free((char *));
			return(NULL);
		}
		strncpy(F(r) [i].Name,FldTmp.Name,11);
		F(r) [i].Type=FldTmp.Type;
		F(r) [i].Length=FldTmp.Length;
		F(r) [i].Dec=FldTmp.Dec;
}
return(r);
}
dbfClose(dbf);
DBFILE *dbf;
{
	fclose(dbf->file);
	if(dbf->fld!=NULL)
		free(dbf->fld);
	if(dbf!=NULL)
		free(dbf);
}
dbfShowHeader(hdr,x,y)
	DBFILE *hdr;
int x,y;
{
	char b[129],buf[129];
	sprintf(b,"DB Version : %8.8s",itoa(hdr->Version,buf,10));
	DrawString(x,y++,b,TEXT);
	sprintf(b, "Number of records : %8.8s",itoa(hdr->NumRex,buf,10));
	DrawString(x,y++,b,TEXT);
	sprintf(b, "Last date updated : %2.2s",itoa(hdr->Mon_upd,buf,10),itoa(hdr->Day_upd,buf+10,10),itoa(hdr->Yr_upd,buf+20,10));
	DrawString(x,y++,b,TEXT);
	sprintf(b,"header length: %8.8s",itoa(hdr->HdrLen,buf,10);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"record length: %8.8s",itoa(hdr->RecLen,buf,10);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"number of fields: %8.8s",itoa(hdr->NumFlds,buf,10);
	DrawString(x,y++,b,TEXT);
	sprintf(b,"File size: %8.8s", itoa(hdr->FileSize,buf,10);
	DrawString(x,y++,b,TEXT);
}
dbfShowStructure(hdr,x,y)
	DBFILE *dbf;
int x,y;
{
	char *Ty,b[129],buf[129];
	int i;
	DrawString(x,y++,"Field, Field Name, Type Width"",Dec",TEXT);
	for(i=0;i<hdr->NumFlds;i++)
	{
		switch(F(hdr)[i].Type)
		{
		case 'C':
			Ty="character";
			break;
		case 'L':
			Ty="logical";
			break;
		case 'N':
			Ty="number";
			break;
		case 'F':
			Ty="Floating Point";
			break;
		case 'D':
			Ty="date";
			break;
		case 'M':
			Ty="memo";
			break;
		default:
			Ty="unknown";
			break;
		}
		sprintf(b,"%5d %-11.11s%11.11s %3d" "%2d",i+1.F(hdr)[i].Name,Ty,F(hdr)[i].Length,F(hdr)[i].Dec);
		DrawString(x,y++,b,TEXT);
	}
	itoa(hdr->RecLen,buf,10);
	sprintf(b,"total = %4s",buf);
	DrawString(x,y++,b,TEXT);
}
dbfReadRec(hdr,Rec,b)
	DBFILE *hdr;
long Rec;
char *b;
{
	long CurrentPos;
	CurrentPos=(Rec-1)*(long)hdr->RecLen+(long)hdr->HdrLen;
	fseek(hdr->file,CurrentPos,SEEK_SET);
	if(fread(b,1,hdr->RecLen,hdr->file)==hdr->RecLen)
		return(1);
	return(0);
}
dbfShowRecord(hdr,Rec,bf,x,y)
	DBFILE *hdr;
long Rec;
char *bf;
int x,y;
{
	char b[129],buf[129];
	int FPos,i,n=20;
	sprintf(b,"RECORD %3d %c",Rec,*bf);
	DrawString(x,y++,b,TEXT);
	++y;
	FPos=1;
	for(i=0;i<hdr->NumFlds;i++)
	{
		sprintf(b,"%-11.11s",F(hdr)[i].Name);
		DrawString(x,y++,b,TEXT);
		switch(F(hdr)[i].Type)
		{
		case 'D':
			sprintf(b,"%2.2s/%2.2s/%4.4s",bf+FPos+4,bf+Fpos+6,bf+FPos);
			DrawString(x,y++,b,TEXT);
			break;
		case 'L':
			switch(bf[FPos])
			{
			case 'Y':
			case 'T':
				DrawString(x+n,y++,"TRUE",TEXT);
				break;
			case 'N':
			case 'F':
				DrawString(x+n,y++,"FALSE",TEXT);
				break;
			default:
				DrawString(x+n,y++,"UNKNOWN",TEXT);
			}
			break;
		case 'M': /*ignoring memo fields*/
			break;
		default:
			sprintf(buf,"%%%d.%ds",F(hdr)[i].Length,F(hdr)[i].Length);
			sprintf(b,buf,bf+FPos);
			DrawString(x+n,y++,b,TEXT);
		}
		FPos+=F(hdr)[i].Length;
	}
}
showbanner(page)
	unsigned long page;
{
	char b[64];
	ClearScreen(BACKGROUND);
	ClearLine(0, BANNER);
	HideCursor();
	DrawString(LEFT,0,"DBF Viewer Screen",BANNER);
	if(page==0L)
		strcpy(b,"HEADER");
	else if(page==1L)
		strcpy(b,"STRUCTURE");
	else strcpy(b,"RECORD");
	DrawString(60,0,b,BANNER);
}
DrawString(x,y,text,attr);
int x,y;
char *text;
int attr;
{
	unsigned int a, *ip, n=0;
	ip=(unsigned int *) (screen+(y*!60+x*2));
	a=attr<<8;
	while(*text)
		ip[n++]=*text++ | a;
	return(n);
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
ClearLine(y,attr)
	int y,attr;
{
	unsigned int a,i,*ip;
	ip=(unsigned int *)(screen+(y*160));
	a=(attr<<8)|32;
	for(i=0;i<SCREENWIDE;++i)
		*ip++=a;
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
		c=getch()<8;
	return(c);
}


