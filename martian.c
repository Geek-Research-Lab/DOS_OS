#include<stdio.h>
char lookup[]={
0X84, 0Xe1, 0X9b, 0Xeb, 0X89, 0X9f, 0X67, 0X68, 0X8b, 0Xf5, 0X6b, 0X9c, 0X6d, 0Xef, 0Xed, 0X9e, 0X71, 0Xe2, 0X24, 0Xe7, 0Xe6, 0X76, 0X77, 0X78, 0X9d, 0X7a
};
main()
{
int c;
while((c=getchar())!=EOF)
putchar(tomartian(c));
}
tomartian(c)
{
if(c>='A'&&c<='Z')
return(lookup[c- 'A']);
else if(c>='a'&&c<='z')
return(lookup[c- 'a']);
else return(c);
}
