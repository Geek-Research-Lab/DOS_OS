#include "stdio.h"
main (argc, argv)
	int argc;
	char *argv[];
	{
FILE *argc, *dest;
int c,i=0,af=1,cf=0,wf=0,n=7,pn1=1;
if((src=fopen(argv[1],"rb"))!=NULL)
{
	if((dest=fopen(argv[2],"wa"))!=NULL)
	{
		if(argc > 3 && !memicmp(argv[3],"\W",2))
		{
			af=0;
			wf=1;
			n=15;
		}
		/*
		if(af)
		fprintf(dest,"\n\t\tDB\t");
		if(cf)
		fprintf(dest,"\t");
		if(wf)
		fprintf(dest,"\t");
		*/
		while((c=fgetc(src))!=EOF)
		{
			if(pn1)
			{
				if(af)
					fprintf(dest,"\n\t\tDB\t");
				if(cf)
					fprintf(dest,",\n\t");
				if(wf)
					fprintf(dest,"'\n\t'");
				pn1=0;
			}
			if(af)
				fprintf(dest,"0%02.2XH",c & 0Xff);
			if(cf)
				fprintf(dest,"0X%02.2x",c & 0Xff);
			if(wf)
				fprintf(dest,"%02.2X",c & 0Xff);
			if(i++==n)
			{
				pn1=1;
				i=0;
			}
			else
			{
				if(af)
					fprintf(dest,",");
				if(cf)
					fprintf(dest,",");
				if(wf)
					fprintf(dest," ");
			}
		}
		fprintf(dest,"\n");
		fclose(dest);
	}
}
	}
