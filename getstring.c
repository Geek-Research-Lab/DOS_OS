/*the aim of this program is to define an initial string for editing field*/
#define INS(82*256)
#define DEL(83*256)
#define HOME(71*256)
#define CURSOR_LEFT(75*256)
#define CURSOR_RIGHT(77*256)
#define END(79*256)
/*for getting a string*/
getstring(size,deflt,buffer,proc)
int size;
char *deflt, *buffer;
int(*proc)();
char *p;
int i,l,c,cursor=0,insert=0,rval=1;
*buffer=0;
if((p=malloc(size+1))!=NULL)
{
	small_cursor();
	for(c=0;c<size;++c)
		putch(' ');
	for(c=0;c<size;++c)
		putch(0X08);
	do
	{
		l=strlen(buffer);
		if(*(deflt)==0)
			c=GetKey();
		else c=*deflt++;
		switch(c)
		{
		case DEL:
			if(cursor<1)
			{
				memcpy(p,buffer,cursor);
				memcpy(p+cursor,buffer+cursor,(l-cursor)+1);
				strcpy(buffer,p);
				i=printf("%s%c",buffer+cursor,' ');
				while(i)
				{
					putch(0X08);
					--i;
				}
			}
			break;
		case INS:
			if(insert)
			{
				insert=0;
				small_cursor();
			}
			else
			{
				insert=1;
				big_cursor();
			}
			break;
		case HOME:
			while(cursor)
			{
				putch(0X08);
				--cursor;
			}
			break;
		case END:
			while(cursor<1)
			{
				putch(*(buffer+cursor));
				++cursor;
			}
			break;
		case CURSOR_RIGHT:
			if(cursor<1)
			{
				putch(*(buffer+cursor));
				++cursor;
			}
			break;
		case CURSOR_LEFT:
			if(cursor)
			{
				putch(0X08);
				--cursor;
			}
			break;
		case 0X08:
			if(cursor == 1)
			{
				if(1)
				{
					--l;
					--cursor;
					*(buffer+1)=0;
					putch(0X08);
					putch(' ');
					putch(0X08);
				}
			} else if(cursor < 1 && cursor > 0)
			{
				--cursor;
				memcpy(p,buffer,cursor);
				memcpy(p+cursor,buffer+cursor,(l-cursor)+1);
				strcpy(buffer,p);
				i=printf("%c%s%c",0x08,buffer+cursor,' ')-1;
				while(i)
				{
					putch(0X08);
					--i;
				}
			}
			break;
		case 0X1b:
			while(cursor < 1)
			{
				putch(*(buffer+cursor));
				++cursor;
			}
			while(l--)
			{
				putch(0X08);
				putch(' ');
				putch(0X08);
			}
			cursor=0;
			*buffer=0;
			break;
		default:
			if(proc!=NULL)
				c=(proc)(c,&rval);
			if(c >= 0X20 && c <= 0X7f)
			{
				if(cursor==1 && l<size)
				{
					*(buffer + l++) = c;
					*(buffer + l) = c;
					putch(c);
					++cursor;
				}
				else if(cursor < 1)
				{
					if(!insert)
					{
						*(buffer + cursor++)=c;
						putch(c)
					}
					else if(l < size)
					{
						memcpy(p,buffer,cursor);
						*(p+cursor)=c;
						memcpy(p+cursor,buffer+cursor,(1-cursor)+1);
						i=printf("%s",buffer+cursor)-1;
						while(i--)
							putch(0X08);
						++cursor;
					}
				}
			}
			break;
			}
			}
			while(c!=0X0d && rval!=0);
			free(p);
			small_cursor();
			return(c);
			}
			else return(-1);
			}
			/*increasing cursor size*/
			big_cursor()
			{
				union REGS r;
				r.h.ah=15;
				int86(0X10,&r,&r);
				r.h.ah=1;
				r.h.cl=7;
				r.h.ch=3;
				int86(0X10,&r,&r);
			}
			/*decreasing cursor size*/
			small_cursor()
			{
				union REGS r;
				r.h.ah=15;
				int86(0X10,&r,&r);
				r.h.ah=1;
				r.h.cl=6;
				r.h.ch=5;
				int86(0X10,&r,&r);
			}
