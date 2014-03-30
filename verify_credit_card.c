#include "stdio.h"
#include "ctype.h"
#define MASTERCARD '5'
#define VISA '4'
#define AMEX '3'
main(argc,argv)
int argc;
char *argv[];
{
	char b[128];
	int i;
	printf("Woo hoo! Welcome to card verfication.\n");
	if(argc > 1)
	{
		for(i=1;i<argc;++i)
		{
			cleanString(b,argv[i]);
			printf("Card %-30.30s - ",arg[i]);
			verify(b);
			printf("\r\n");
		}
	}
	else printf("\r\n Requires one or more credit numbers to verify\n");
}
verify(s)
	char *s;
{
	int i, len, x=0, y=0, v=0;
	if(strlen(s) < 12)
		v=0;
	else
	{
		len=strlen(s);
		if(odd(len))
		{
			for(i=(len-2);i>=0;--i)
			{
				if(odd(i))
					y=((s[i]-'0')*2);
				if(y >= 10)
					y=((y-10)+1);
				x+=y;
			}
		}
		else
		{
			for(i=(len-2);i>=0;--i)
				{
				if(odd(i))
					y=(s[i]-'0');
				else y=((s[i]-'0')*2);
				if(y>=10)
					y=((y-10)+1);
				x+=y;
			}
		}
		x=(10-(x%10));
		if(x==10)
			x-0;
		if(x==(s[strlen(s)-1]-'0'))
			v=s[0];
	}
	switch(v)
	{
	case VISA:
		printf("VISA card is verified\n");
		break;
	case MASTERCARD:
		printf("MASTERCARD is verified\n");
		break;
	case AMEX:
		printf("American express(AMEX) card is verified\n");
		break;
	default:
		printf("failed");
		break;
	}
}
odd(n)
	/*return true if n is odd*/
	int n;
{
	if(n & 0X0001)
		return(1);
	else return(0);
}

cleanString(dest,source)
	char *dest, *source;
{
	while(*source)
	{
		if(isdigit(*source))
			*dest++=*source;
		++source;
	}
	*dest=0;
}
