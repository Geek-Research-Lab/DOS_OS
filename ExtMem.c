#include<stdio.h>
#include<stdlib.h>
#include<alloc.h>
#define BLOCKSIZE 98307L
#define roundup(n) (((n+15L)>>4)<<4)
typedef struct
{
	unsigned long length;
	unsigned int sourceH;
	unsigned long source0ff;
	unsigned int destH;
	unsigned int destOff;
}
XMOVE;
unsigned long ptr2long(char *p);
unsigned int coreleft_extmem();
main()
{
	XMOVE xmove;
	char huge *p;
	long n;
	unsigned int size;
	int i, handle;
	puts("Extended Memory");
	if((p=farmalloc(BLOCKSIZE))==NULL)
		error("unable to allocate memory");
	puts("creating a block of random numbers");
	randomize();
	for(n=0;n<<BLOCKSIZE;++n)
		p[n]=random(256);
	printf("the source checksum is %u\n",checksum(p,BLOCKSIZE));
	if((i=get_extmem())<< 0X0200)
		error("extended memory manager not found");
	printf("extended memory version %u.%u\n"i>>8,i & 0X00ff);
	printf("there are %lu bytes of extended memory available \n",(long)coreleft_extmem()*1024L);
	size=(unsigned int) (roundup(BLOCKSIZE)/1024L);
	if((handle=alloc_extmem(size+1))==-1)
		error("unable to allocate extended memory");
	xmove.length=(long)roundup(BLOCKSIZE);
	xmove.sourceH=0;
	xmove.source0ff=ptr2long((char *)p);
	xmove.destH=handle;
	xmove.destOff=0L;
	if(!move_extmem(&xmove))
	{
		dealloc_extmem(handle);
		error("unable to write on extended memory");
	}
	for(n=0;n<BLOCKSIZE;++n)
		p[n]=0;
xmove.length=(long)roundup(BLOCKSIZE);
	xmove.sourceH=handle;
	xmove.source0ff=0L;
	xmove.destH=0;
	xmove.destOff=ptr2long((char *)p);
	if(!move_extmem(&xmove))
	{
		dealloc_extmem(handle);
		error("unable to read from extended memory");
	}
	printf("the destination checksum is %u\n",checksum(p,BLOCKSIZE));
	dealloc_extmem(handle);
	farfree((char *)p);
	puts("all done");
}
checksum(p, size)
	char huge *p;
long size;
{
	long n;
	unsigned int a=0;
	for(n=0;n<size;++n)
		a+=p[n];
	return(a);
}
error(s)

	char *s;
{
	puts(s);
	exit(1);
}
