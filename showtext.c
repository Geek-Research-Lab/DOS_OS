#include<stdio.h>
#include<alloc.h>
char comfile[]={
0Xeb, 0X03, 0X90, 0X00, 0X00, 0Xb8, 0X00, 0X40, 0Xbb, 0X01, 0X00, 0X2e, 0X8b, 0X0e, 0X03, 0X01, 0Xba, 0X1a, 0X01, 0Xcd, 0X21, 0Xb8, 0X00, 0X4c, 0Xcd, 0X21
};
main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	char *p;
	unsigned long l;
	unsigned int *ip;
	puts("Self-displaying" "text generator""_________________");
	if(argc<=2)
		error("Arguments: text file to read \n" "___COM file to write \n");
	if((fp=fopen(argv[1],"rb"))==NULL)
		error("Error opening the source file");
	fseek(fp, OL, SEEK_END);
	l=ftell(fp);
	fseek(fp, OL, SEEK_SET);
	if(1>32767L)
		error("The source file is a bit huge");
	if((p=malloc(unsigned int)1, fp)!=(unsigned int)1)
		error("can not read the source file");
	fclose(fp);
	ip=(unsigned int *) (comfile+3);
	ip[0]=(unsigned int)1;
	if((fp=fopen(argv[2],"wb"))==NULL)
		error("error creating the destination file");
	if(fwrite(comfile, 1, sizeof(comfile),fp)!=(unsigned int)1)
		error("Error in writing the code");
	fclose(fp);
	puts("all done");
	exit(0);
}
error(s)
	char *s
{
	puts(s);
	puts(1);
}
