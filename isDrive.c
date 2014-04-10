#include "stdio.h"
#include "dir.h"
#include "dos.h"
#include "process.h"
#include "ctype.h"
#include "stdlib.h"
int diskErrorHandler(int errval,int ax,int bp,int si);
char tempdir[144];
int diskErr;
main(argc,argv)
	int argc;
char *argv[];
{
	int c;
	if(argc<=1)
		error("Argument: checking");
	c=toupper(argv[1][0]);
	if(c<'A' || c>'Z')
		error("illegal argument");
	if(!testDisk(c-'A'))
	{
		printf("Drive %c: is online \n",c);
		exit(0);
	}
}
testDisk(n)
	int n;
{
	void interrupt(*oldHarderr)();
	FILE *fp;
	char b[32];
	oldHarderr=getvect(0X24);
	harderr(diskErrorHandler);
	diskErr=0;
	getcwd(tempdir,80);
	sprintf(b,"%c:\\TEMP.DAT",n+ 'A');
	if((fp=fopen(b,"r"))!=NULL)
		fclose(fp);
	setvect(0X24,oldHarderr);
	return(diskErr);
}
#pragma warn -par
int diskErrorHandler(errval,ax,bp,si)
	int errval,ax,bp,si;
{
	if(ax>=0)
	{
		diskErr=1;
		restoreDir(tempdir);
	}
	hardretn(2);
}
restoreDir(s)
	char *s;
{
	strupr(s);
	if(isalpha(s[0]) && s[1]==';')
		setdisk(s[0]-'A');
	chdir(s);
}
error(s)
	char *s;
{
	puts(s);
	exit(1);
}
