#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit lcdrs=P1^0;
sbit lcden=P1^1;
sbit s1=P1^4;
sbit s2=P1^5;
sbit s3=P1^6; 
sbit s4=P1^7;
sbit fm=P2^0;
sbit cs=P2^1;
sbit as=P2^2;
sbit wr=P2^3;
sbit ds=P2^4;
sbit irq=P2^5;
uchar code sz1[]={"20  -  -      "};									//14 初始化时候显示	
uchar code sz2[]={"  :  :  "};											//8
uchar code aa[10]={"0123456789"};										//定义一个液晶的数字字符数组
uchar code dd[]={"MONTUEWEDTHUFRISATSUN"};								//定义一个星期显示数组
uchar code zz[]={0,1,2,1,0,1,0,1,1,0,1,0,1};							//定义一个区别大月小月和二月的数组
uchar num,bb;															//定义全局变量
uint cc;																//该变量用于功能键的自动退出
char shi,fen,miao,yue,ri,xq,nian1,yue1,ri1,wc,hh,nian,nm,tnnd;			//定义带符号的变量
void ds_write(uchar a,uchar b)											//向DS12C887写入数据
{	
	cs=0;
	as=1;
	ds=1;
	wr=1;
	P0=a;
	as=0;
	wr=0;
	P0=b;
	wr=1;
	as=1;
	cs=1;
}
/*void set_time()	  //首次上电初始化时间函数
{
    ds_write(1,0);
	ds_write(2,0);
	ds_write(3,0);
	ds_write(4,0);
	ds_write(5,0);
	ds_write(6,0);
	ds_write(7,0);
	ds_write(8,0);
    ds_write(9,0);
  
} */

void ys(uint x)															//延时函数
{
	uint z,y;
	for(z=x;z>0;z--)
		for(y=110;y>0;y--);
}

void xqsd(char a)														//星期设定函数,在调整月份和年份的时候刷新星期的数字
{
	if(a==8)
		xq=1;
	else if(a==9)
		xq=2;
	else if(a==10)
		xq=3;
	else if(a==11)
		xq=4;
	else if(a==12)
		xq=5;
	else if(a==13)
		xq=6;
	else if(a==0)
		xq=7;
	else if(a==-1)
		xq=6;
	else if(a==-2)
		xq=5;
	else if(a==-3)
		xq=4;
	else if(a==-4)
		xq=3;
	else if(a==-5)
		xq=2;
	else if(a==-6)
		xq=1;
	ri1=1;
}

void fmq()																//蜂鸣器小小的响一下函数
{
	fm=0;
	ys(100);
	fm=1;
}

void lcdsd(uchar temp)													//设定子函数
{
	lcdrs=0;															//当lcdrs=0时,允许写入指令
	P0=temp;
	ys(5);
	lcden=1;
	ys(5);
	lcden=0;
}

void lcdxs(uchar temp)													//显示子函数
{
	lcdrs=1;															//当lcdrs=1时,允许写入显示
	P0=temp;
	ys(5);
	lcden=1;															//lcden高脉冲将P0口数据读走
	ys(5);
	lcden=0;
}

void sr_sj(uchar a,uchar b)												//输入时间函数,第一个参数为光标指针,第二个为数字
{
	lcdsd(0x80+0x40+a-1);
	lcdxs(aa[b/10]);
	lcdxs(aa[b%10]);
}

void sr_rq(uchar a,uchar b)												//输入日期函数
{
	lcdsd(0x80+a-1);
	lcdxs(aa[b/10]);
	lcdxs(aa[b%10]);
}

void sr_xq(uchar a,uchar b)												//输入星期
{		
	lcdsd(0x80+a-1);
	if(b==1)
		b=0;
	else if(b==2)
		b=3;	
	else if(b==3)
		b=6;
	else if(b==4)
		b=9;
	else if(b==5)
		b=12;
	else if(b==6)
		b=15;
	else if(b==7)
		b=18;
	lcdxs(dd[b]);
	lcdxs(dd[b+1]);	
	lcdxs(dd[b+2]);	
}

void key()																//按键检测函数
{
	if(s1==0)															//检测功能键是否被按下	
	{
		ys(5);															//去抖延时
		if(s1==0)														//再次检测功能键是否被按下
		{
			bb++;														//记录功能键被按下多少次
			cc=0;														//当有按键按下时将cc清零
			tnnd=1;														//有按键按下的时候让tnnd等于1,等退出时间设定的时候向时钟芯片写入数据
			while(!s1)													//按键松手检测
			{
				ys(10);													//去抖延时
			}
			fmq();														//蜂鸣器小小的响一下
			if(bb==1)													//当功能键被按下一次时
			{
				TR0=0;													//关闭定时器
				lcdsd(0x80+0x40+0x0b);									//光标指针指向秒的个位
				lcdsd(0x0f);											//显示光标并且闪烁
			}
			if(bb==2)													//当功能键被按下两次时
			{
				lcdsd(0x80+0x40+0x08);									//光标指针指向分的个位
			}
			if(bb==3)													//当功能键被按下三次时
			{
				lcdsd(0x80+0x40+0x05);									//光标指针指向时的个位
			}
			if(bb==4)													//当功能键被按下四次时
			{
				lcdsd(0x80+0x0a);										//光标指针指向日的个位
			}
			if(bb==5)													//当功能键被按下五次时
			{
				lcdsd(0x80+0x07);										//光标指针指向月的个月
			}
			if(bb==6)													//当功能键被按下六次时
			{
				lcdsd(0x80+0x04);										//光标指针指向年的个位
			}
			if(bb==7)													//当功能键被按下七次时
			{
				lcdsd(0x0c);											//不显示光标
				TR0=1;													//启动定时器
				bb=0;													//将bb清零
			}	
		}
	}	   
	if(bb!=0)															//当检测到功能键被按下时进入,才能检测到加减按钮
	{
		if(s1&&s2&&s3==1)												//定时自动退出功能键
		{
			cc++;														//每进入一次cc自加一
			if(cc==65535)												//当cc被加到65535的时候		
			{
				bb=0;													//将bb清零
				cc=0;													//将cc清零
				lcdsd(0x0c);											//不显示光标
				TR0=1;													//启动定时器
			}
		}
		if(s2==0)														//检测(加)是否被按下
		{
			ys(5);														//去抖延时
			if(s2==0)													//再次检测(加)是否被按下
			{
				cc=0;													//当有按键按下时将cc清零
				while(!s2)												//松手检测
				{
					ys(10);												//去抖延时
				}
				fmq();													//蜂鸣器小小的响一下
				if(bb==1)												//当功能键被按下一次时
				{
					miao++;												//miao加一
					if(miao==60)										//当miao被加到60的时候将其清零
						miao=0;
					sr_sj(0x0b,miao);									//改变秒的数字
					lcdsd(0x80+0x40+0x0b);								//设定光标指针位置
				}
				if(bb==2)												//当功能键被按下两次时
				{
					fen++;												//fen加一
					if(fen==60)											//当fen被加到60的时候将其清零
						fen=0;
					sr_sj(0x08,fen);									//改变分的数字
					lcdsd(0x80+0x40+0x08);								//设定光标指针位置
				}
				if(bb==3)												//当功能键被按下三次时
				{
					shi++;												//shi加一
					if(shi==24)											//当shi被加到24的时候将其清零
						shi=0;
					sr_sj(0x05,shi);									//改变时的数字
					lcdsd(0x80+0x40+0x05);								//设定光标指针位置
				}
				if(bb==4)												//当功能键被按下三次时
				{
					hh=ri;												//记录ri的初值
					ri++;												//ri加一
					if((zz[yue]==0)&&(ri==31)==1)						//当小月ri被加到了31时,还原ri的值
					{
						ri=hh;
					}
					if((zz[yue]==1)&&(ri==32)==1)						//当大月ri被加到32时,还原ri的值
					{
						ri=hh;
					}
					if((zz[yue]==2)&&(nian%4==0)&&(ri==30)==1)			//当闰年二月ri被加到30时,还原ri的值
					{
						ri=hh;
					}
					if((zz[yue]==2)&&(nian%4!=0)&&(ri==29)==1)			//当平年二月ri被加到29时,还原ri的值
					{
						ri=hh;
					}
					if(hh!=ri)											//当ri发生改变时
					{
						xq++;											//xq(星期)加一
						if(xq==8)										//当xq被加到8的时候将其置一
							xq=1;
						ri1=1;											//在显示日期函数中刷新一下液晶上ri的数字
					}
				}
				if(bb==5)												//当功能键被按下五次时
				{
					wc=yue;												//先将yue的值记录下来
					yue++;												//yue加一
					yue1=1;												//在显示日期函数中刷新一下液晶上yue的数字
					if(yue==13)											//当yue被加到13时,还原yue的值
						yue=wc;
					if((zz[yue]==0)&&(ri>30)==1)						//当小月ri大于30时
					{
						yue=wc;											//还原yue的值
						yue1=0;											//禁止液晶的刷新
					}
					if((zz[yue]==2)&&(nian%4==0)&&(ri>29)==1)			//当闰年二月ri大于29时
					{
						yue=wc;											//还原yue的值
						yue1=0;											//禁止液晶的刷新
					}
					if((zz[yue]==2)&&(nian%4!=0)&&(ri>28)==1)			//当平年二月ri大于28时
					{
						yue=wc;											//还原yue的值
						yue1=0;											//禁止液晶的刷新
					}
					if(wc!=yue)											//检测yue的值是否发生改变
					{
						if(zz[wc]==0)									//改变前是小月
						{
							xq=xq+2;									//星期加二
						}
						if(zz[wc]==1)									//改变前是大月
						{
							xq=xq+3;									//星期加三
						}
						if((zz[wc]==2)&&(nian%4==0)==1)					//改变前是闰年二月
						{
							xq=xq+1;									//星期加一
						}
						xqsd(xq);										//设定一下星期的值(因为有可能是星期天再加上N天)
					}
				}
				if(bb==6)												//当功能键被按下六次时
				{
					nm=nian;											//先将nian的值记录下来
					nian++;												//nian加一
					nian1=1;											//在显示日期函数中刷新一下液晶上nian的数字
					if((zz[yue]==2)&&(nian%4!=0)&&(ri>28)==1)			//当平年二月ri大于28时
					{
						nian=nm;										//还原nian的值
						nian1=0;										//禁止液晶的刷新
					}
					if(nm!=nian)										//如果nian的值发生了改变
					{
						if(nm%4==0)										//改变前是闰年
						{
							xq=xq+2;									//星期加二
						}
						if(nm%4!=0)										//改变前是平年
						{
							xq=xq+1;									//星期加一
						}
						xqsd(xq);										//设定一下星期的值
					}	
				}
			}
		}
		if(s3==0)														//检测(减)是否被按下
		{
			ys(5);														//去抖延时
			if(s3==0)													//再次检测减是否被按下
			{
				cc=0;
				while(!s3)												//松手检测
				{
					ys(10);												//去抖延时
				}
				fmq();													//蜂鸣器小小的响一下
				if(bb==1)												//当功能键被按下一次时
				{
					miao--;												//miao减一
					if(miao<0)											//当miao被减过头的时候(嘿嘿),给它重新赋值
						miao=59;
					sr_sj(0x0b,miao);									//改变miao的数字
					lcdsd(0x80+0x40+0x0b);								//设定光标位置
				}
				if(bb==2)												//当功能键被按下两次时
				{
					fen--;												//fen减一
					if(fen<0)											//当fen被减过头的时候(嘿嘿),给它重新赋值
						fen=59;
					sr_sj(0x08,fen);									//改变fen的数字
					lcdsd(0x80+0x40+0x08);								//设定光标位置
				}
				if(bb==3)												//当功能键被按下三次时
				{
					shi--;												//shi减一
					if(shi<0)											//当shi被剪过头的时候(嘿嘿),给它重新赋值
						shi=23;
					sr_sj(0x05,shi);									//改变shi的数字
					lcdsd(0x80+0x40+0x05);								//设定光标位置
				}
				if(bb==4)
				{
					hh=ri;
					ri--;
					if((zz[yue]==0)&&(ri==0)==1)
					{
						ri=hh;
					}
					if((zz[yue]==1)&&(ri==0)==1)
					{
						ri=hh;
					}
					if((zz[yue]==2)&&(nian%4==0)&&(ri==0)==1)
					{
						ri=hh;
					}
					if((zz[yue]==2)&&(nian%4!=0)&&(ri==0)==1)
					{
						ri=hh;
					}
					if(hh!=ri)
					{
						xq--;
						if(xq==0)
							xq=7;
						ri1=1;	
					}
				}
				if(bb==5)
				{
					wc=yue;
					yue--;
					yue1=1;
					if(yue==0)
						yue=wc;
					if((zz[yue]==0)&&(ri>30)==1)
					{
						yue=wc;
						yue1=0;
					}
					if((zz[yue]==2)&&(nian%4==0)&&(ri>29)==1)
					{
						yue=wc;
						yue1=0;
					}
					if((zz[yue]==2)&&(nian%4!=0)&&(ri>28)==1)
					{
						yue=wc;
						yue1=0;
					}
					if(wc!=yue)
					{
						if(zz[yue]==0)
						{
							xq=xq-2;
						}
						if(zz[yue]==1)
						{
							xq=xq-3;
						}
						if((zz[yue]==2)&&(nian%4==0)==1)
						{
							xq=xq-1;
						}
						xqsd(xq);
					}
				}
				if(bb==6)
				{
					nm=nian;
					nian--;
					nian1=1;
					if((zz[yue]==2)&&(nian%4!=0)&&(ri>28)==1)
					{
						nian=nm;
						nian1=0;
					}
					if(nm!=nian)
					{
						if(nian%4==0)
						{
							xq=xq-2;
						}
						if(nian%4!=0)
						{
							xq=xq-1;
						}
						xqsd(xq);
					}
				}
			}
		}
	}
}

uchar ds_read(uchar a)													//读取时钟芯片数据
{
	uchar bbb;
	as=1;
	ds=1;
	wr=1;
	cs=0;
	P0=a;
	as=0;
	ds=0;
	P0=0xff;
	bbb=P0;
	as=1;
	ds=1;
	cs=1;
	return bbb;
}


void xs_rq()															//显示日期函数
{
	if((tnnd==1)&&(bb==0)==1)											//当退出时间设定时,首先将设定的时间写入芯片
	{
		tnnd=0;
		ds_write(0,miao);
		ds_write(2,fen);
		ds_write(4,shi);
	}
	if(bb==0)															//只有在非设定时间的时候,才刷新液晶上的时间
	{
		miao=ds_read(0);
		fen=ds_read(2);
		shi=ds_read(4);
		xq=ds_read(6);
		ri=ds_read(7);
		yue=ds_read(8);
		nian=ds_read(9);
		ys(10);
		sr_sj(0x0b,miao);
		sr_sj(0x08,fen);
		sr_sj(0x05,shi);
		sr_xq(0x0d,xq);	
		sr_rq(0x0a,ri);
		sr_rq(0x07,yue);
		sr_rq(0x04,nian);
	}
	if(ri1==1)															//当ri1等于1时,刷新一下液晶上ri的数字
	{
		ri1=0;															//将ri1归零
		sr_rq(0x0a,ri);													//刷新日
		sr_xq(0x0d,xq);													//刷新星期
		lcdsd(0x80+0x0a);												//设定光标位置
		ds_write(6,xq);													//向芯片写入星期	
		ds_write(7,ri);													//向芯片写入日
	}
	if(yue1==1)															//当yue1等于1时,刷新一下液晶上yue的数字
	{
		yue1=0;															//将yue1归零
		sr_rq(0x07,yue);												//刷新月
		lcdsd(0x80+0x07);												//设定光标位置
		ds_write(6,xq);
		ds_write(8,yue);												//向芯片写入月
	}
	if(nian1==1)														//当nian1等于1时,刷新一下液晶上nian的数字
	{
		nian1=0;														//将nian1归零
		sr_rq(0x04,nian);												//刷新年
		lcdsd(0x80+0x04);												//设定光标位置
		ds_write(6,xq);
		ds_write(9,nian);												//向芯片写入年
	}
}

void csh()																//初始化函数
{
	uchar a;
//	duan=wei=0;															//关闭锁存器
	lcden=0;															
//	rd=0;																//首先赋初值
	bb=0;
	lcdsd(0x38);														//指令码:设置16X2显示,5X7点阵,8位数据接口
	lcdsd(0x0c);														//指令码:开显示,显示光标,光标闪烁
	lcdsd(0x06);														//指令码:指针加一,整屏不移动
	lcdsd(0x01);														//显示清屏
	lcdsd(0x80+0x01);													//定义光标指针
	for(a=0;a<14;a++)													//第一排显示日期
	{
		lcdxs(sz1[a]);
	}
	lcdsd(0x80+0x40+0x04);												//定义光标指针
	for(a=0;a<8;a++)													//第二排显示时间
	{
		lcdxs(sz2[a]);
	}
	miao=ds_read(0);										//每次开机的时候都先读取芯片里面的数据
	fen=ds_read(2);
	shi=ds_read(4);
	xq=ds_read(6);
	ri=ds_read(7);
	yue=ds_read(8);
	nian=ds_read(9);
	sr_rq(0x0a,ri);														//刷新日
	sr_xq(0x0d,xq);														//刷新星期
	sr_rq(0x07,yue);													//刷新月											
	sr_rq(0x04,nian);													//刷新年
	sr_sj(0x05,shi);													//刷新液晶shi
	sr_sj(0x08,fen);													//刷新液晶fen
	sr_sj(0x0b,miao);													//刷新液晶miao
}

void main()																//主函数
{
    //set_time();
	csh();
	s4=0;																//调用初始化函数
	while(1)															//进入大循环
	{
		key();															//检测按键
		xs_rq();														//刷新显示日期
	}	
}
