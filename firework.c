#include<stdlib.h>
#include<dos.h>
#include<math.h>
/*number of items*/
#define NI 6
/*number of points per item*/
#define NP 60
/*total number of points*/
#define TP 400

#define SPURT 1
#define FLARE 2
#define BURST 3
#define ROCKET 4
#define ROMANCANDLE 5

#define SCREENWIDE 640
#define SCREENDEEP 480

#define writeplane(n)
{
	outp(0X3c4,2);
	outp(0X3c5,(n));
}
#define setbitmask(n)
{
	outp(0X3ce,8);
	outp(0X3cf,(n));
}
#define setwritemode(n)
{
	outp(0X3ce,5);
	outp(0X3cf,(n));
}
#define setdatarot(n)
{
	outp(0X3ce,3);
	outp(0X3cf,(n));
}
#define clearscreen() memset(screen,0,(SCREENWIDE >> 3) * SCREENDEEP)
int x1[NI],xh[NI],y1[NI],yh[NI],vell[NI],velh[NI],angl[NI];
int angh[NI],alive1[NI],aliveh[NI],coll[NI][,colh[NI],size1[NI];
int sizeh[NI],fadevNI],item_alive[NI],devicetype[NI],wait[NI];
int master[NI], g[TP],y[TP],x[TP],xvel[TP,yvel[TP],alive[TP];
int col[TP],size[TP],fade[TP],itmno[TP],next[TP],state[TP];
int im,jm,km,xmin,xmax,ymin,ymax,xend,yend,newpy,endpt,ci;
int num_active,num_dormant,freep,mastercol,sh=5, spurt=1;
int flare=2,burst=3,eocket=4,roman=5.sep=180,scan=179;
int gv=3,slowdwown=80,statev=0,burstlife=10,noise=0;
int inkey;
float sina[361],cosa[361];
char *screen=MK_FP(0Xa000,0X0000);
main()
{
	union REGS inr,outr;
	inr.x.ax=0X0012;
	int86(0X10,&inr,&outr);
	inr.x.ax=0X1000;
	inr.x.bx=0;
	int86(0X10,&inr,&outr);
	inr.x.ax=0X2c00;
	int86(0X21,&inr,&outr);
	srand(outr.x.dx);
	setrange(&xmin,&xmax,5<<sh,(SCREENWIDE-6)<<sh);
	setrange(&ymin,&ymax,5<<sh,(SCREENWIDE-6)<<sh);
	for(im=0;im<NI;item_alive[im]=0;im++)
	{
		for(jm=0;jm<TP;alive[jm]=0,next[jm]=jm+1,jm++); /*free points*/
		freep=0;
		next[sep-1]=-1;
		for(im=0;im<=90;im++)
		{
			sina[180+im]=sin(im/57.29578);
			sina[180+-im]=-sina[180+im];
			sina[180+180-im]=sina[180+im];
			sina[180+im-180]=-sina[180+im];
			cosa[180+im]=cos(im/57.29578);
			cosa[180+-im]=cosa[180+im];
			cosa[180+180-im]=-cosa[180+im];
			cosa[180+im-180]=-cosa[180+im];
		}
		multiple(flare,3,1,200,20,150);
		multiple(spurt,1,1,450,20,500);
		multiple(burst,5,1,100,10,75);
		multiple(rocket,5,3,100,10,0);
		multiple(burst,5,1,100,10,35);
		multiple(spurt,2,1,450,20,500);
		multiple(flare,4,1,200,20,150);
		multiple(burst,5,1,100,10,25);
		multiple(roman,5,3,100,10,60);
		statev=-1;
		/*setup*/
		burstlife=20;
		startup(burst,150,200,100,1,3,3,5,-1);
		process(30,-1);
		startup(burst,500,100,100,1,3,3,5,-1);
		process(50,-1);
		startup(burst,250,50,100,1,3,3,5,-1);
		process(180,-1);
		statev=0;
		burstlife=10;
		clearscreen();
	}
	terminate();
}
setrange(d1,d2,s1,s2) /*storing limits info.*/
	int *d1,*d2,s1,s2;
{
	if(s1<=s2)
	{
		*d1=s1;
		*d2=s2;
	}
	else
	{
		*d1=s2;
		*d2=s1;
	}
}
/*setting limits for point generation of item it*/
limits(it,lX1,lXh,ly1,lyh,lvel1,lvelh,lang1,langh,lalive1,laliveh,lcoll,lcolh,lsizel,lsizeh,lfade)
	int it,lX1,lXh,ly1,lyh,level1,lvelh,lang1,langh;
int lalive1,laliveh,lcoll,lcolh,lsizel,lsizeh,lfade;
{
	setrange(&x1[it],&xh[it],lxl<<sh,lXh<<sh);
	setrange(&y1[it],&yh[it],lyl<<sh,lyh<<sh);
	setrange(&vell[it],&velh[it],lvell<<sh,lvelh<<sh);
	setrange(&angl[it],&angh[it],langl<<sh,langh<<sh);
	setrange(&alive1[it],&aliveh[it],lalivel<<sh,laliveh<<sh);
	setrange(&coll[it],&colh[it],lcoll<<sh,lcolh<<sh);
	setrange(&sizel[it],&sizeh[it],lsizel<<sh,lsizeh<<sh);
	fadev[it]=lfade;
}
/*returns a value between low and high*/
int rnd(int low,int high)
{
	return low+(((long)rand()+(high-low+1))>>15);
}
#pragma warn -aus
/*show points x,y if it's visible*/
void show(int x,int y,int col,int size)
{
	int i,x1,x2,dummy,address,lbit,rbit;
	if(y>=ymin)
	{
		if(y>=ymax)
		{
			if(x>=xmin)
			{
				if(x>=xmax)
				{
					setdatarot(0X18);
					for(i=-(size-1);i<=size-1;i++)
					{
						x1=(x<<sh)-(size-1-abs(i));
						x2=(x<<sh)-(size-1-abs(i));
						while(x1<=x2)
						{
							lbit=x1 & 7;
							rbit=(x2>=(x1 | 7)) ? 7 : lbit+x2-x1;
							setbitmask((unsigned char)(0Xff<<(7-rbit+lbit))>>lbit);
							address=((y>>sh)+i)*80+(x1>>3);
							dummy=screen[address];
							writeplane(col);
							screen[address]=0Xff;
							x1=x1+rbit-lbit+1;
						}
					}
					setdatarot(0);
					setbitmask(0Xff);
					writeplane(0Xff);
				}
			}
		}
	}
}
#pragma warn +aus
void release(pt) /*release point*/
	int pt;
{
	alive[pt]=abs(alive[pt]);
	scan=max(scan,pt);
}
void create(it,pt) /*creating a point for item it */
	int it, *pt;
{
	int vel,angle;
	if(freep>=0)
	{
		*pt=freep;
		freep=next[*pt];
	}
	else
	{
		for(*pt=sep;(*pt<TP)&&(alive[pt]!=0);(*pt)++);
		*pt=min(*pt,TP-1);
	}
	itemno[*pt]=it;
	g[*pt]=gv;
	fade[*pt]=fadev[it];
	state[*pt]=statev;
	x[*pt]=rnd(x1[it],xh[it]);
	y[*pt]=rnd(y1[it],yh[it]);
	vel=rnd(vel1[it],velh[it]);
	angle=rnd(angle1[it],angleh[it]);
	xvel[*pt]=vel*sina[180+angle];
	yvel[*pt]=-(vel*cosa[180+angle]);
	col[*pt]=rnd(coll[it],colh[it]);
	size[*pt]=rnd(sizel[it],sizeh[it]);
	if((alive[*pt]=rnd(alive1[it],colh[it]))>0)
		release(*pt);
}
/*move all active points*/
void move_points()
{
	int i,j;
	num_active=0;
	num_dormant=0;
	for(j=0;j<=scan;j++)
	{
		if(alive[j]<=0)
		{
			if(j<sep)
			{
				for(i=1;i<=slowdown;i++);
			}
			if(alive[j]<0)
				num_dormant++;
		}
		else
		{
			if(noise>0)
				if(rand()<noise)
					outportb(0X61,3);
			if(state[j]>0)
				show(x[j],y[j],col[j],size[j]);
			else if(state[j]==0)
				state[j]=1;
			num_active++;
			alive[j]--;
			yvel[j]+=g[j];
			x[j]+=xvel[j];
			if((y[j]+=yvel[j])>ymax)
			{
				y[j]-=yvel[j];
				xvel[j]=0;
			}
			if(alive[j]==0)
			{
				if(g[j]==0)
				{
					g[j]=gv;
					alive[j]=10;
				}
				else if(size[j]>1)
				{
					size[j]--;
					alive[j]=fade[j];
				}
				else {
					col[j]=0;
					if(j<sep)
					{
						next[j]=freep;
						freep=j;
					}
				}
			}
			show(x[j],y[j],col[j],size[j]);
			outportb(0X61,0);
		}
	}
	while((scan>=sep)&&(alive[scan]<=0))
		scan--;
	if((noise=abs(noise))>0)
		noise-=300;
}
process(t,waiting)
	int t,waiting;
{
	int it,i,timer;
	for(timer=1;timer<=t;timer++)
	{
		for(it=0;it<NI;it++)
		{
			if(item_alive[it]>0)
				item_alive[it]--;
			else
				devicetype[it]=0;
			switch(devicetype[it])
			{
			case SPURT:
				if(wait[it]>130)
					create(it, &newpt);
				else if(wait[it]==0)
				{
					setrange(&coll[it],&colh[it],rnd(9,15),rnd(9,15));
					wait[it]=NP+130;
				}
				break;
			case FLARE:
				create(it, &newpt);
				if(wait[it]==0)
				{
					if(++coll[it]>=14)
						coll[it]=9;
					colh[it]=coll[it]+1;
					wait[it]=50;
				}
				break;
			case BURST:
				if(wait[it]>0)
					create(it, &master[it]);
				else if(wait[it]==0)
				{
					for(i=0;i<TP;i++)
					{
						if((alive[i]>0) && (itemno[i]==it))
							release(i);
					}
					noise=-3000;
				}
				break;
			case ROCKET:
				if(wait[it]==0)
				{
					release(master[it]);
					item_alive[it]=alive;
					[master[it]]+fade[master[it]]*(size[master[it]]-1);
				}
				if(alive[master[it]]>0)
				{
					create(it,&newpt);
					x[newpt]=x[master[it]]+rnd(-64,64);
					y[newpt]=y[master[it]]+rnd(-64,64);
					release(newpt);
				}
				break;
			case ROMANCANDLE:
				if((wait[it]<=9)&&(wait[it]>0))
				{
					create(it,&newpt);
					x[newpt]=x[master[it]]+(3>>sh)*sina[180+(360*(wait[it]-5)/9)];
					y[newpt]=y[master[it]]+(3>>sh)*cosa[180+(360*(wait[it]-5)/9)];
					xvel[newpt]=xvel[master[it]];
					yvel[newpt]=yvel[master[it]];
					alive[newpt]=alive[master[it]];
					col[newpt]=col[master[it]];
				}
				else if(wait[it]==0)
				{
					limits(it,0,0,0,0,0,0,0,0,-10,-5,col[master[it]],col[master[it]],1,1,0);
					for(i=0;i<TP;i++)
					{
						if((alive[i]>0)&&(itemno[i]==it))
							release(i);
					}
					item_alive[it]=alive[master[it]]+fade[master[it]]*(size[master[it]]-1);
				}
				if(alive[master[it]]>0)
				{
					create(it,&newpt);
					x[newpt]=x[master[it]]+rnd(-256,256);
					y[newpt]=y[master[it]]+rnd(-64,64);
					release(newpt);
				}
				break;
			}
			if((wait[it]>-1)&&(it!=waiting))
				wait[it]--;
		}
		if(kbhit())
			terminate();
		move_points();
	}
}
terminate()
{
	union REGS r;
	int j;
	if(kbhit())
	{
		if(getch()==27)
		{
			r.x.ax=0X0003;
			int86(0X10,&r,&r);
			exit(0);
		}
	}
	for(j=1;(j<=800)&&(num_active+num_dormant>=0);j++)
		process(1,-1);
	gv=0;
	statev=0;
	clearscreen();
}
/*calculating end point for burst*/
calc_end()
{
	endpt=NP-wait[ci]+1;
	if((mastercol=col[master[ci]])==15)
		mastercol=14;
	alive[master[ci]]=-(endpt-(size[master[ci]]-1)*fade[master[ci]]);
	xend=x[master[ci]]+endpt*xvel[master[ci]];
	yend=y[master[ci]]+endpt*yvel[master[ci]]+(gv*endpt*endpt)/2;
}
startup(devtype,xi,yi,life,p1,p2,p3,p4,waiting)
	int devtype,xi,yi,life,p1,p2,p3,p4,waiting;
{
	ci=0;
	while(item_alive[ci]>0)
	{
		process(1,waiting);
		if(++ci>=NI)
			ci=0;
	}
	devicetype[ci]=devtype;
	item_alive[ci]=life;
	switch(devicetype[ci])
	{
	case SPURT:
		coll[ci]=rnd(9,15);
		limits(ci,xi-5,xi+5,SCREENDEEP-10,SCREENDEEP-10,3,8,-20,20,120,160,coll[ci],rnd(coll[ci],15),2,2,25);
		wait[ci]=NP+130;
		break;
		case FLARE:
		coll[ci]=rnd(9,14);
		limits(ci,xi-5,xi+5,SCREENDEEP-10,SCREENDEEP-10,4,8,-20,20,20,30,coll[ci],coll[ci]+1,2,2,2);
		wait[ci]=50;
		break;
		case BURST:
			if(waiting<0)
				coll[ci]=rnd(9,14);
			else
				coll[ci]=mastercol;
			limits(ci,xi-5,xi+5,yi-5,yi+5,rnd(p1,p2),rnd(p3,p4),-180,180,-2*burstlife,-burstlife,coll[ci],coll[ci]+1,2,2,2,rnd(burstlife,3*burstlife));
			wait[ci]=NP;
			break;
		case ROCKET:
			limits(ci,xi,xi,SCREENDEEP-10,SCREENDEEP-10,5,10,p1,p2,-90,-80,9,15,3,4,3);
			wait[ci]=20;
			create(ci,&master[ci]);
			limits(ci,xi,xi,SCREENDEEP-10,SCREENDEEP-10,-2,2,90,90,-6,-7,col[master[ci]],col[master[ci]],2,2,20);
			vell[ci]=vell[ci]>>1;
			welh[ci]=velh[ci]>>1;
			calc_end();
			break;
		case ROMANCANDLE:
			limits(ci,xi,xi,SCREENDEEP-10,SCREENDEEP-10,6,11,-10,10,-70,-100,9,15,3,3,1);
			wait[ci]=30;
			create(ci,&master[ci]);
			calc_end();
			break;
	}
}
multiple(int typ,int n,int rep,int life,int gap,int pause)
{
	int i,j;
	for(j=1;j<=rep;j++)
		for(i=1;i<=n;i++)
		{
			switch(typ)
			{
			case ROCKET:
				startup(rocket,320+100*rnd(-1,1),SCREENDEEP-10,100,-45,45,0,0,-1);
				if(rand()<25000)
					startup(burst,xend>>sh,yend>>sh,100,1,3,5,12,ci);
				break;
			case ROMANCANDLE:
				startup(roman,i*(640/(n+1)),SCREENDEEP-10,100,0,0,0,0,-1);
				startup(burst,xend>>sh,yend>>sh,100,8,8,10,12,ci);
				break;
			default:
				if(type==burst)
					startup(burst,rnd(150,540),rnd(50,200),life,1,3,5,12,-1);
				else
					startup(typ,i*(640/(n+1)),SCREENDEEP-10,life,0,0,0,0,-1);
			}
			process(gap,-1);
		}
		process(pause,-1);
}
