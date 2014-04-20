#include<stdio.h>
#include<dos.h>
#include<dir.h>
char *formatlong(long l);
main()
{
	unsigned long size=0L, count=0L;
	_stklen=10000;
	puts("Recursive directory tree sixe");
	recurse("\\",&size,&count);
	printf("Total file count:\t%s files\n",formatlong(count));
	printf("Total file size:\t%s bytes\n",formatlong(size));
}
recurse(s, size, count)
	char *s;
unsigned long *size, *count;
{
	struct ffblk f;
	char path[145];
	int i;
	strcpy(path, s);
	strcat(path, "*.*");
	if(!findfirst(path, &f, 0X0010)
	{
		do
		{
			if(f.ff_attrib & 0X0010 & f.ff_name[0]!='.')
			{
				strcpy(path, s);
	strcat(path, "\\");
	strcat(path, "\\");
	printf("\rChecking %s",path);
	recurse(path, size, count);
	putchar(13);
	for(i=0;i<79;++i)
		putchar(32);
			}
			else
			{
				(*size)+=f.ff_size;
				++(*count);
			}
		}
		while(!findnext(&f));
	}
}
char *formatlong(l)
	long l;
{
	static char b[24];
	char s[16];
	int i,j,n;
	sprintf(s,"%lu",l);
	strrev(s);
	n=strlen(s);
	for(i=j=0;i<n;++i)
	{
		b[j++]=s[i];
		if(!((i+1)%3)&&i<(n-1))
			b[j++]=',';
	}
	b[j]=0;
	strrev(b);
	return(b);
}
