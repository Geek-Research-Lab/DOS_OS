#include "stdio.h"
#include "alloc.h"
#define buffersize 0Xfff0
FILE *infile, *outfile;
char *buffer;
unsigned int buffsize=0;
main(argc, argv)
	int argc;
char *argv[];
{
	puts("prelink version");
	if(argc > 3)
	{
		if((buffer=malloc(buffersize))!+NULL)
		{
			if((infile=fopen(argv[1],"rb"))!=NULL)
			{
				if((outfile=fopen(argv[2],"wb"))!=NULL)
				{
					fillbuffer();
					trecord();
					lnames("\004DATA\006DGROUP\005_DATA");
					segmentdef();
					groupdef();
					drecord();
					pnames(argv[3]);
					endmodule();
					fclose(outfile);
				}
				fclose(infile);
			}
			else
				printf("%s not found \n",argv[1]);
			free(buffer);
		}
		else
			puts("memory allocation error");
	}
	else
	{
		puts("prelink <source file> destination file");
	}
}
fillbuffer()
{
	static chsr b[16]="\xa0\x04\x00\x01\x00\x00";
	char *p;
	int c;
	p=buffer+6;
	memcpy(buffer,b,6);
	do
	{
		c=fgetc(infile);
		if(c!=EOF)
		{
			*p++=c;
			++buffsize;
			if(!buffer[1])
				++buffer[2];
		}
		while(c!=EOF && p < buffer+buffersize);
	}
	endmodule() /*module end record */
	{
		static char b[16]="\x8a\x02\x00\x00";
		checksum(b);
		wrecord(b);
	}
	segmentdef() /*segment definition record*/
	{
		static char b[16]="\x98\x07\x00\x48\x00\x00\x04\x02\x01";
		b[4]=buffsize & 0Xff;
		b[5]=buffsize >> 8;
		checksum(b);
		wrecord(b);
	}
	groupdef() /*group definition record*/
	{
		static char b[16]="\x9a\x04\x00\x03\xff\x01";
	checksum(b);
	wrecord(b);
}
	pnames(s) /*public names record*/
		char *s;
	{
		static char b[128]="\x90\x00\x00\x00\x01\x00";
		unsigned int i=6;
		while(*s)
		{
			b[i++]=*s++;
			++b[5];
		}
		b[1]=i+1;
		checksum(b);
		wrecord(b);
	}
	drecord() /*data record*/
	{
		checksum(buffer);
	wrecord(buffer);
	}
	trecord() /*t record*/
	{
		static char b[16]="\x80\x03\x00\x01\x41\x3b";
		wrecord(b);
	}
	lnames(s) /*list of names record*/
		char *s;
	{
		static char b[128]="\x96\x00\x00\x00";
		unsigned int i=4;
		while(*s)
			b[i++]=*s++;
		b[1]=i-2;
		checksum(b);
		wrecord(b);
	}
	wrecord(s) /*writing a record*/
		char *s;
	{
		unsigned int i;
		for(i=0;i<3;++i)
			fputc(s[i],outfile);
		for(i=0;i<(unsigned int)(s[1]+(s[2]<<8));++i)
			fputc(s[i+3],outfile);
	}
	checksum(p) /*checksum for record p*/
		char *p;
	{
		unsigned int i, l, c=0;
		l=(p[1]+(p[2]<<8))+2;
		for(i=0;i<1;++i)
			c=c+*p++;
		c=(c^0Xff)+1;
		*p=c;
		return(c);
	}

