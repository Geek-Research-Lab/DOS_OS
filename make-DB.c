#include "stdio.h"
#include "conio.h"
#include "io.h"
#include "math.h"
#include <sys/stat.h>
typedef struct
{
	char manufacturer[25];
	char model[25];
	unsigned int year;
	double displacement;
	long quantity;
}
RECORD;
typedef struct
{
	char sig[8];
	unsigned long count;
}
HEADER;
#define DATABASE_SIG "DATABASE"
main(argc, margv)
	int argc;
char *agrv[];
{
	HEADER header;
	RECORD record;
	FILE *in;
	char b[129], *p;
	int out;
	puts("database creator");
	if(argc <= 2)
		error("arguments: path to a source file\n");
	if((in=fopen(argv[1],"ra"))==NULL)
		error("unable to open source file");
	if((out=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_TWRITE))==NULL)
		error("unable to create destination file");
	memset((char *)&header,0,sizeof(HEADER));
	if(!writeheader(out,&header))
		error("unable to write header");
	do
	{
		if((p=fgets(b,128,in))!=NULL)
		{
			parserecord(&record,b);
			if(!writerecord(out,&record,header,count))
				error("unable to write record");
			++header.count;
		}
	}
	while(p!=NULL);
	if(!writeheader(out,&header))
		error("unable to write header");
	close(out);
	fclose(in);
}
parserecord(r,b)
	RECORD *r;
char *b;
{
	int i;
	char s[129];
	memset((char *)r,0,sizeof(RECORD));
	/*get the manufacturer*/
	for(i=0;*b!=0&&*b!=','&&*b!='\n';++b,++i)
	{
		if(i<24)
			r->manufacturer[i]=*b;
	}
	r->manufacturer[i]=0;
	while(*b==',')++b;
	/*get the model*/
	for(i=0;*b!=0&&*b!=','&&*b!='\n';++b,++i)
	{
		if(i<24)
			r->model[i]=*b;
	}
	r->model[i]=0;
	while(*b==',')++b;
	/*get the year*/
	for(i=0;*b!=0&&*b!=','&&*b!='\n';++b,++i)
	{
		if(i<24)
			s[i]=*b;
	}
	s[i]=0;
	while(*b==',')++b;
	r->year=atoi(s);
	/*get the displacement*/
	for(i=0;*b!=0&&*b!=','&&*b!='\n';++b,++i)
	{
		if(i<24)
			s[i]=*b;
	}
	s[i]=0;
	while(*b==',')++b;
	r->displacement=atof(s);
	/*get the quantity*/
	for(i=0;*b!=0&&*b!=','&&*b!='\n';++b,++i)
	{
		if(i<24)
			s[i]=*b;
	}
	s[i]=0;
	while(*b==',')++b;
	r->quantity=atol(s);
}
writerecord(fh,r,n)
	int fh;
RECORD *r;
unsigned long n;
{
	lseek(fh,(long)sizeof(HEADER)+n*(long)sizeof(RECORD),SEEK_SET);
	if(write(fh,(char *)r,sizeof(RECORD))==sizeof(RECORD))
		return(1);
	else
		return(0);
}
writeheader(fh,h)
	int fh;
HEADER *h;
{
	memcpy(h->sig,DATABASE_SIG,8);
	lseek(fh,0L,SEEK_SET);
	if(write(fh,(char *)h,sizeof(HEADER))==sizeof(HEADER))
		return(1);
	else return(0);
}
error(s)
	char *s;
{
	put(s);
	exit(1);
}
