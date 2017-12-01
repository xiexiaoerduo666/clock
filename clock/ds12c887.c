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
uchar code sz1[]={"20  -  -      "};									//14 ��ʼ��ʱ����ʾ	
uchar code sz2[]={"  :  :  "};											//8
uchar code aa[10]={"0123456789"};										//����һ��Һ���������ַ�����
uchar code dd[]={"MONTUEWEDTHUFRISATSUN"};								//����һ��������ʾ����
uchar code zz[]={0,1,2,1,0,1,0,1,1,0,1,0,1};							//����һ���������С�ºͶ��µ�����
uchar num,bb;															//����ȫ�ֱ���
uint cc;																//�ñ������ڹ��ܼ����Զ��˳�
char shi,fen,miao,yue,ri,xq,nian1,yue1,ri1,wc,hh,nian,nm,tnnd;			//��������ŵı���
void ds_write(uchar a,uchar b)											//��DS12C887д������
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
/*void set_time()	  //�״��ϵ��ʼ��ʱ�亯��
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

void ys(uint x)															//��ʱ����
{
	uint z,y;
	for(z=x;z>0;z--)
		for(y=110;y>0;y--);
}

void xqsd(char a)														//�����趨����,�ڵ����·ݺ���ݵ�ʱ��ˢ�����ڵ�����
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

void fmq()																//������СС����һ�º���
{
	fm=0;
	ys(100);
	fm=1;
}

void lcdsd(uchar temp)													//�趨�Ӻ���
{
	lcdrs=0;															//��lcdrs=0ʱ,����д��ָ��
	P0=temp;
	ys(5);
	lcden=1;
	ys(5);
	lcden=0;
}

void lcdxs(uchar temp)													//��ʾ�Ӻ���
{
	lcdrs=1;															//��lcdrs=1ʱ,����д����ʾ
	P0=temp;
	ys(5);
	lcden=1;															//lcden�����彫P0�����ݶ���
	ys(5);
	lcden=0;
}

void sr_sj(uchar a,uchar b)												//����ʱ�亯��,��һ������Ϊ���ָ��,�ڶ���Ϊ����
{
	lcdsd(0x80+0x40+a-1);
	lcdxs(aa[b/10]);
	lcdxs(aa[b%10]);
}

void sr_rq(uchar a,uchar b)												//�������ں���
{
	lcdsd(0x80+a-1);
	lcdxs(aa[b/10]);
	lcdxs(aa[b%10]);
}

void sr_xq(uchar a,uchar b)												//��������
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

void key()																//������⺯��
{
	if(s1==0)															//��⹦�ܼ��Ƿ񱻰���	
	{
		ys(5);															//ȥ����ʱ
		if(s1==0)														//�ٴμ�⹦�ܼ��Ƿ񱻰���
		{
			bb++;														//��¼���ܼ������¶��ٴ�
			cc=0;														//���а�������ʱ��cc����
			tnnd=1;														//�а������µ�ʱ����tnnd����1,���˳�ʱ���趨��ʱ����ʱ��оƬд������
			while(!s1)													//�������ּ��
			{
				ys(10);													//ȥ����ʱ
			}
			fmq();														//������СС����һ��
			if(bb==1)													//�����ܼ�������һ��ʱ
			{
				TR0=0;													//�رն�ʱ��
				lcdsd(0x80+0x40+0x0b);									//���ָ��ָ����ĸ�λ
				lcdsd(0x0f);											//��ʾ��겢����˸
			}
			if(bb==2)													//�����ܼ�����������ʱ
			{
				lcdsd(0x80+0x40+0x08);									//���ָ��ָ��ֵĸ�λ
			}
			if(bb==3)													//�����ܼ�����������ʱ
			{
				lcdsd(0x80+0x40+0x05);									//���ָ��ָ��ʱ�ĸ�λ
			}
			if(bb==4)													//�����ܼ��������Ĵ�ʱ
			{
				lcdsd(0x80+0x0a);										//���ָ��ָ���յĸ�λ
			}
			if(bb==5)													//�����ܼ����������ʱ
			{
				lcdsd(0x80+0x07);										//���ָ��ָ���µĸ���
			}
			if(bb==6)													//�����ܼ�����������ʱ
			{
				lcdsd(0x80+0x04);										//���ָ��ָ����ĸ�λ
			}
			if(bb==7)													//�����ܼ��������ߴ�ʱ
			{
				lcdsd(0x0c);											//����ʾ���
				TR0=1;													//������ʱ��
				bb=0;													//��bb����
			}	
		}
	}	   
	if(bb!=0)															//����⵽���ܼ�������ʱ����,���ܼ�⵽�Ӽ���ť
	{
		if(s1&&s2&&s3==1)												//��ʱ�Զ��˳����ܼ�
		{
			cc++;														//ÿ����һ��cc�Լ�һ
			if(cc==65535)												//��cc���ӵ�65535��ʱ��		
			{
				bb=0;													//��bb����
				cc=0;													//��cc����
				lcdsd(0x0c);											//����ʾ���
				TR0=1;													//������ʱ��
			}
		}
		if(s2==0)														//���(��)�Ƿ񱻰���
		{
			ys(5);														//ȥ����ʱ
			if(s2==0)													//�ٴμ��(��)�Ƿ񱻰���
			{
				cc=0;													//���а�������ʱ��cc����
				while(!s2)												//���ּ��
				{
					ys(10);												//ȥ����ʱ
				}
				fmq();													//������СС����һ��
				if(bb==1)												//�����ܼ�������һ��ʱ
				{
					miao++;												//miao��һ
					if(miao==60)										//��miao���ӵ�60��ʱ��������
						miao=0;
					sr_sj(0x0b,miao);									//�ı��������
					lcdsd(0x80+0x40+0x0b);								//�趨���ָ��λ��
				}
				if(bb==2)												//�����ܼ�����������ʱ
				{
					fen++;												//fen��һ
					if(fen==60)											//��fen���ӵ�60��ʱ��������
						fen=0;
					sr_sj(0x08,fen);									//�ı�ֵ�����
					lcdsd(0x80+0x40+0x08);								//�趨���ָ��λ��
				}
				if(bb==3)												//�����ܼ�����������ʱ
				{
					shi++;												//shi��һ
					if(shi==24)											//��shi���ӵ�24��ʱ��������
						shi=0;
					sr_sj(0x05,shi);									//�ı�ʱ������
					lcdsd(0x80+0x40+0x05);								//�趨���ָ��λ��
				}
				if(bb==4)												//�����ܼ�����������ʱ
				{
					hh=ri;												//��¼ri�ĳ�ֵ
					ri++;												//ri��һ
					if((zz[yue]==0)&&(ri==31)==1)						//��С��ri���ӵ���31ʱ,��ԭri��ֵ
					{
						ri=hh;
					}
					if((zz[yue]==1)&&(ri==32)==1)						//������ri���ӵ�32ʱ,��ԭri��ֵ
					{
						ri=hh;
					}
					if((zz[yue]==2)&&(nian%4==0)&&(ri==30)==1)			//���������ri���ӵ�30ʱ,��ԭri��ֵ
					{
						ri=hh;
					}
					if((zz[yue]==2)&&(nian%4!=0)&&(ri==29)==1)			//��ƽ�����ri���ӵ�29ʱ,��ԭri��ֵ
					{
						ri=hh;
					}
					if(hh!=ri)											//��ri�����ı�ʱ
					{
						xq++;											//xq(����)��һ
						if(xq==8)										//��xq���ӵ�8��ʱ������һ
							xq=1;
						ri1=1;											//����ʾ���ں�����ˢ��һ��Һ����ri������
					}
				}
				if(bb==5)												//�����ܼ����������ʱ
				{
					wc=yue;												//�Ƚ�yue��ֵ��¼����
					yue++;												//yue��һ
					yue1=1;												//����ʾ���ں�����ˢ��һ��Һ����yue������
					if(yue==13)											//��yue���ӵ�13ʱ,��ԭyue��ֵ
						yue=wc;
					if((zz[yue]==0)&&(ri>30)==1)						//��С��ri����30ʱ
					{
						yue=wc;											//��ԭyue��ֵ
						yue1=0;											//��ֹҺ����ˢ��
					}
					if((zz[yue]==2)&&(nian%4==0)&&(ri>29)==1)			//���������ri����29ʱ
					{
						yue=wc;											//��ԭyue��ֵ
						yue1=0;											//��ֹҺ����ˢ��
					}
					if((zz[yue]==2)&&(nian%4!=0)&&(ri>28)==1)			//��ƽ�����ri����28ʱ
					{
						yue=wc;											//��ԭyue��ֵ
						yue1=0;											//��ֹҺ����ˢ��
					}
					if(wc!=yue)											//���yue��ֵ�Ƿ����ı�
					{
						if(zz[wc]==0)									//�ı�ǰ��С��
						{
							xq=xq+2;									//���ڼӶ�
						}
						if(zz[wc]==1)									//�ı�ǰ�Ǵ���
						{
							xq=xq+3;									//���ڼ���
						}
						if((zz[wc]==2)&&(nian%4==0)==1)					//�ı�ǰ���������
						{
							xq=xq+1;									//���ڼ�һ
						}
						xqsd(xq);										//�趨һ�����ڵ�ֵ(��Ϊ�п������������ټ���N��)
					}
				}
				if(bb==6)												//�����ܼ�����������ʱ
				{
					nm=nian;											//�Ƚ�nian��ֵ��¼����
					nian++;												//nian��һ
					nian1=1;											//����ʾ���ں�����ˢ��һ��Һ����nian������
					if((zz[yue]==2)&&(nian%4!=0)&&(ri>28)==1)			//��ƽ�����ri����28ʱ
					{
						nian=nm;										//��ԭnian��ֵ
						nian1=0;										//��ֹҺ����ˢ��
					}
					if(nm!=nian)										//���nian��ֵ�����˸ı�
					{
						if(nm%4==0)										//�ı�ǰ������
						{
							xq=xq+2;									//���ڼӶ�
						}
						if(nm%4!=0)										//�ı�ǰ��ƽ��
						{
							xq=xq+1;									//���ڼ�һ
						}
						xqsd(xq);										//�趨һ�����ڵ�ֵ
					}	
				}
			}
		}
		if(s3==0)														//���(��)�Ƿ񱻰���
		{
			ys(5);														//ȥ����ʱ
			if(s3==0)													//�ٴμ����Ƿ񱻰���
			{
				cc=0;
				while(!s3)												//���ּ��
				{
					ys(10);												//ȥ����ʱ
				}
				fmq();													//������СС����һ��
				if(bb==1)												//�����ܼ�������һ��ʱ
				{
					miao--;												//miao��һ
					if(miao<0)											//��miao������ͷ��ʱ��(�ٺ�),�������¸�ֵ
						miao=59;
					sr_sj(0x0b,miao);									//�ı�miao������
					lcdsd(0x80+0x40+0x0b);								//�趨���λ��
				}
				if(bb==2)												//�����ܼ�����������ʱ
				{
					fen--;												//fen��һ
					if(fen<0)											//��fen������ͷ��ʱ��(�ٺ�),�������¸�ֵ
						fen=59;
					sr_sj(0x08,fen);									//�ı�fen������
					lcdsd(0x80+0x40+0x08);								//�趨���λ��
				}
				if(bb==3)												//�����ܼ�����������ʱ
				{
					shi--;												//shi��һ
					if(shi<0)											//��shi������ͷ��ʱ��(�ٺ�),�������¸�ֵ
						shi=23;
					sr_sj(0x05,shi);									//�ı�shi������
					lcdsd(0x80+0x40+0x05);								//�趨���λ��
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

uchar ds_read(uchar a)													//��ȡʱ��оƬ����
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


void xs_rq()															//��ʾ���ں���
{
	if((tnnd==1)&&(bb==0)==1)											//���˳�ʱ���趨ʱ,���Ƚ��趨��ʱ��д��оƬ
	{
		tnnd=0;
		ds_write(0,miao);
		ds_write(2,fen);
		ds_write(4,shi);
	}
	if(bb==0)															//ֻ���ڷ��趨ʱ���ʱ��,��ˢ��Һ���ϵ�ʱ��
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
	if(ri1==1)															//��ri1����1ʱ,ˢ��һ��Һ����ri������
	{
		ri1=0;															//��ri1����
		sr_rq(0x0a,ri);													//ˢ����
		sr_xq(0x0d,xq);													//ˢ������
		lcdsd(0x80+0x0a);												//�趨���λ��
		ds_write(6,xq);													//��оƬд������	
		ds_write(7,ri);													//��оƬд����
	}
	if(yue1==1)															//��yue1����1ʱ,ˢ��һ��Һ����yue������
	{
		yue1=0;															//��yue1����
		sr_rq(0x07,yue);												//ˢ����
		lcdsd(0x80+0x07);												//�趨���λ��
		ds_write(6,xq);
		ds_write(8,yue);												//��оƬд����
	}
	if(nian1==1)														//��nian1����1ʱ,ˢ��һ��Һ����nian������
	{
		nian1=0;														//��nian1����
		sr_rq(0x04,nian);												//ˢ����
		lcdsd(0x80+0x04);												//�趨���λ��
		ds_write(6,xq);
		ds_write(9,nian);												//��оƬд����
	}
}

void csh()																//��ʼ������
{
	uchar a;
//	duan=wei=0;															//�ر�������
	lcden=0;															
//	rd=0;																//���ȸ���ֵ
	bb=0;
	lcdsd(0x38);														//ָ����:����16X2��ʾ,5X7����,8λ���ݽӿ�
	lcdsd(0x0c);														//ָ����:����ʾ,��ʾ���,�����˸
	lcdsd(0x06);														//ָ����:ָ���һ,�������ƶ�
	lcdsd(0x01);														//��ʾ����
	lcdsd(0x80+0x01);													//������ָ��
	for(a=0;a<14;a++)													//��һ����ʾ����
	{
		lcdxs(sz1[a]);
	}
	lcdsd(0x80+0x40+0x04);												//������ָ��
	for(a=0;a<8;a++)													//�ڶ�����ʾʱ��
	{
		lcdxs(sz2[a]);
	}
	miao=ds_read(0);										//ÿ�ο�����ʱ���ȶ�ȡоƬ���������
	fen=ds_read(2);
	shi=ds_read(4);
	xq=ds_read(6);
	ri=ds_read(7);
	yue=ds_read(8);
	nian=ds_read(9);
	sr_rq(0x0a,ri);														//ˢ����
	sr_xq(0x0d,xq);														//ˢ������
	sr_rq(0x07,yue);													//ˢ����											
	sr_rq(0x04,nian);													//ˢ����
	sr_sj(0x05,shi);													//ˢ��Һ��shi
	sr_sj(0x08,fen);													//ˢ��Һ��fen
	sr_sj(0x0b,miao);													//ˢ��Һ��miao
}

void main()																//������
{
    //set_time();
	csh();
	s4=0;																//���ó�ʼ������
	while(1)															//�����ѭ��
	{
		key();															//��ⰴ��
		xs_rq();														//ˢ����ʾ����
	}	
}
