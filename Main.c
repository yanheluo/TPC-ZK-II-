#include<conio.h>
#include<stdio.h>
#include"ApiExUsb.h"
#pragma comment(lib,"ApiExUsb.lib")


int KeyTable[]={0x77,0x7b,0x7d,0x7e,0xb7,0xbb,0xbd,0xbe,0xd7,0xdb,0xdd,0xde,0xe7,0xeb,0xed,0xee};
				//0123456789ABCDEf


int LCDTable[]={0xA3B0,0xA3B1,0xA3B2,0xA3B3,0xA3B4,0xA3B5,0xA3B6,0xA3B7,0xA3B8,
				0xA3B9,221,0xA3C2,0xA3C3,0xA3C4,0xA3C5,0xA3C6
				};//0123456789ABCDEf


int KeyValue=100;

int PassWord_setted[]={0,0,0,0,0,0};
int PassWord_set[]={0,0,0,0,0,0};
int PassWord_admin[]={0,0,0,0,0,0};




int Word0[]={0xbf,0xaa,0xbb,0xfa,0xbd,0xe7,0xc3,0xe6};//FIRST VIEW 开机界面 
int Word1[]={0xca,0xe4,0xc8,0xeb,0xbd,0xe7,0xc3,0xe6};//OPENING 输入界面 
int Word2_1[]={0xc3,0xdc,0xc2,0xcb,0xd6,0xd8,0xc9,0xe8,};//Setting 密码重设
int Word2_2[]={0xca,0xe4,0xc8,0xeb,0xd4,0xad,0xc3,0xdc,0xc2,0xcb};//RESETTING 请输入原密码 
int Word3[]={0xb9,0xdc,0xc0,0xed,0xd4,0xb1,0xc4,0xa3,0xca,0xbd};//Admining 管理员模式 
int Wordx[]={0xA3,0XAA};//************
int Message_true[]={0xd5,0xfd,0XC8,0xB7};//正确 
int Message_false[]={0XB4,0XED,0xCE,0xF3};//错误 
int Message_warning[]={0xbe,0xaf,0xb8,0xe6};//警告 

//A 进入开锁界面， B 进入重置密码界面 C 进入管理员模式 D Clear指令  E 中英文界面切换 F 确认键 


int E1[]={0xa3,0xc6,0XA3,0Xd6};//12 f view
int E2[]={0XA3,0XC9,0XA3,0XCE};//in
int E3[]={0xa3,0xd2,0xa3,0xe5,0xa3,0xf3,0xa3,0xe5,0xa3,0xf4};//reset 10
int E4[]={0xa3,0xc1,0xa3,0xe4,0xa3,0xed,0xa3,0xe9,0xa3,0xee};//admin 10
int M1[]={0xa3,0xd2,0xa3,0xe9,0xa3,0xe7,0xa3,0xe8,0xa3,0xf4};//right 10
int M2[]={0xa3,0xd7,0xa3,0xd2,0xa3,0xef,0xa3,0xce,0xa3,0xe7};//wrong 10 
int M3[]={0xa3,0xf7,0xa3,0xc1,0xa3,0xd2,0xa3,0xee};//warn 14
int line[] = {0x70, 0xb0, 0xd0, 0xe0};

int WrongCount=1;
bool language_flag=1;

bool Judge_num(int*,int*);
void PassWord_input(int );
void BEEP();
void Light(bool);
void First_View();
void Language_change(bool);
void PassWord_clear();
void PassWord_setting();

void KeyDown()
{
	printf("Key is putting\n");
	byte data;
	KeyValue = 100;
	while (KeyValue == 100) {
			Sleep(30);
		for (int i = 0; i < 4; i ++ ) {
				Sleep(30);
			PortWriteByte(0x28a, line[i]);
				Sleep(30);
			PortReadByte(0x28a, &data);
			if ((data << 4) == (0xf0)) continue;
			byte key_d = line[i] | data;
			for (int j = 0; j < 16; j ++ ) {
				if (KeyTable[j] == key_d) KeyValue = j;
			}
		}
		printf("keyval = %d\n", KeyValue);
	}
}



void Setup()//写指令入LCD 
{


		PortWriteByte(0x289,0);
		Sleep(2);
		
	
		PortWriteByte(0x289,0x04);
		Sleep(2);
		
		PortWriteByte(0x289,0);
		Sleep(2);

}



void DataSet()//写数据入LCD 
{

		PortWriteByte(0x289,0x01);
		Sleep(50);
		PortWriteByte(0x289,0x05);
		Sleep(50);
		PortWriteByte(0x289,0x01);
		Sleep(50);

}

void PassWord_toset()
{
	int llimit=0;
		
		PortWriteByte(0x288,0x01);
		Setup();
		PortWriteByte(0x288,0x84);
		Setup();
		
	if(language_flag){
		
		for(int i=0;i<8;i++)
		{
		PortWriteByte(0x288,Word1[i]);
		DataSet();
		}	
		}
		else{
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,E2[i]);
		DataSet();
		}	
		}
	
	byte startloc=0x96;
	
	while(llimit<6)
	{
		while(1)
		{
			
		KeyDown();
		
		if(KeyValue==13)
		{
			llimit=0;
			startloc=0x96;

			for(int i=0;i<6;i++)
			PassWord_setted[i]=0;
			PassWord_clear();
		}
		
		if(KeyValue<10)
		{	
		PassWord_setted[llimit]=KeyValue;
		PortWriteByte(0x288,startloc);
		Setup();
		
		for(int i=0;i<2;i++)
		{
			PortWriteByte(0x288,Wordx[i]);
			DataSet();
		}
		break;	
		}
		}
		startloc--;
		llimit++;
			
	}
	First_View();
	
}


void PassWord_setting()
{
		int llimit=0;
		
		PortWriteByte(0x288,0x01);
		Setup();
		PortWriteByte(0x288,0x84);
		Setup();
		
	if(language_flag){
		
		for(int i=0;i<8;i++)
		{
		PortWriteByte(0x288,Word1[i]);
		DataSet();
		}	
		}
		else{
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,E2[i]);
		DataSet();
		}	
		}
	
		byte startloc=0x96;

	while(llimit<6)
	{
		while(1)
		{
		KeyDown();
		if(KeyValue==14)
			First_View();
		if(KeyValue==13)
		{
			llimit=0;
			startloc=0x96;
			for(int i=0;i<6;i++)
			PassWord_set[i]=0;
			PassWord_clear();
		}
		if(KeyValue<10)
		{	
		PassWord_set[llimit]=KeyValue;
		PortWriteByte(0x288,startloc);
		Setup();
		for(int j=0;j<2;j++)
		{
		PortWriteByte(0x288,Wordx[j]);
		DataSet();
		}
		break;
		}
		}
		startloc--;
		
		llimit++;
			
	}	
	while(1)
{
	KeyDown();
	
	if(KeyValue==13)
		PassWord_setting();
	if(KeyValue==14)
		First_View();

	if(KeyValue==15)
{
		
	for(int i=0;i<6;i++)
	{
		printf("%d\n",PassWord_set[i]);
	}
	
		for(int j=0;j<6;j++)
	{
		printf("%d\n",PassWord_setted[j]);
	}

	if(Judge_num(PassWord_setted,PassWord_set))
	{
		
		PortWriteByte(0x288,0x01);
		Setup();
		//位置设定
		PortWriteByte(0x288,0x90);
		Setup();
		//信息提示
		if(language_flag){
		
		for(int i=0;i<4;i++)
		{
			
		PortWriteByte(0x288,Message_true[i]);
		DataSet();
		}	
		
		}
		else{
		for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,M1[i]);
		DataSet();
		}
		}
			
		Light(1);
		Sleep(300);
		
		PassWord_toset();
		
		}

	else
	{
		
		if(WrongCount<2)
		{	
			WrongCount++;
						
			PortWriteByte(0x288,0x01);
			Setup();
			PortWriteByte(0x288,0x90);
			Setup();
			
			if(language_flag){
			
			for(int i=0;i<4;i++)
			{
				PortWriteByte(0x288,Message_warning[i]);
				DataSet();
			}
			}
			else{
				for(int i=0;i<14;i++)
			{
				PortWriteByte(0x288,E3[i]);
				DataSet();
			}
			
			}
			Sleep(1000);
			PassWord_setting();	
		}
		else
		{
		PortWriteByte(0x288,0x01);
		Setup();
		//位置设定
		PortWriteByte(0x288,0x90);
		Setup();
		//信息提示
		if(language_flag){
		
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,Message_false[i]);
		DataSet();
		}	
	}
	else{
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,E2[i]);
		DataSet();
		}	
	}
		Sleep(1000);
		BEEP();
		First_View();
		
		}
								}
											}
														}	
}

void PassWord_reset()
{
	
	PortWriteByte(0x288,0x01);
	Setup();
	//位置设定
	PortWriteByte(0x288,0x80);
	Setup();
	//信息提示
	
	Sleep(2000);
	
	if(language_flag){
	
	for(int i=0;i<8;i++)
	{
	PortWriteByte(0x288,Word2_1[i]);
	DataSet();
	}
	}
	else
	{
	for(int i=0;i<10;i++)
	{
	PortWriteByte(0x288,E3[i]);
	DataSet();
	}
	}	
		Sleep(1000);	
		KeyDown();
		
		if(KeyValue==14)
			First_View();
		else if(KeyValue==15)
			PassWord_setting();
		else 
			PassWord_reset();
		
	
}


void PassWord_clear()
{
	
	PortWriteByte(0x288,0x01);
	Setup();
	PortWriteByte(0x288,0x84);
	Setup();
	if(language_flag){
	
	for(int i=0;i<8;i++)
	{
	PortWriteByte(0x288,Word1[i]);
	DataSet();
	}	
	}
	else
	{
		for(int i=0;i<4;i++)
	{
	PortWriteByte(0x288,E2[i]);
	DataSet();
	}	
	}
	
}





void PassWord_input()
{
		int llimit=0;
		
		PortWriteByte(0x288,0x01);
		Setup();
		PortWriteByte(0x288,0x84);
		Setup();
		
	if(language_flag){
		
		for(int i=0;i<8;i++)
		{
		PortWriteByte(0x288,Word1[i]);
		DataSet();
		}	
		}
		else{
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,E2[i]);
		DataSet();
		}	
		}
	
	byte startloc=0x96;
	
	while(llimit<6)
	{
		while(1)
		{
			
		KeyDown();
		if(KeyValue==14)
			First_View();
		
		if(KeyValue==13)//D 清除指令 
		{
			llimit=0;
			startloc=0x96;

			for(int i=0;i<6;i++)
			PassWord_set[i]=0;
			PassWord_clear();
		}
		
		if(KeyValue<10)
		{	
		PassWord_set[llimit]=KeyValue;
		PortWriteByte(0x288,startloc);
		Setup();
		for(int j=0;j<2;j++)
		{
		PortWriteByte(0x288,Wordx[j]);
		DataSet();
		}
		break;
		}
		
		}
		startloc--;
		llimit++;
		
			
	}
	
	while(1)
{
	KeyDown();
	
	if(KeyValue==13)
		PassWord_input();
	if(KeyValue==14)
		First_View();

	if(KeyValue==15)
{
		
	for(int i=0;i<6;i++)
	{
		printf("%d\n",PassWord_set[i]);
	}
	for(int j=0;j<6;j++)
	{
		printf("%d\n",PassWord_setted[j]);
	}
	if(Judge_num(PassWord_setted,PassWord_set))

	{
		
		PortWriteByte(0x288,0x01);

		Setup();

		//位置设定
		PortWriteByte(0x288,0x93);

		Setup();

		//信息提示
		if(language_flag){
			
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,Message_true[i]);
		DataSet();
		}
		}
		else{
		for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,M1[i]);

		DataSet();

		}

		}
		Light(1);
		Sleep(2000);
		First_View();
		}
	else

	{
		if(WrongCount<2)
		{	
			WrongCount++;
			
			PortWriteByte(0x288,0x01);
			Setup();
			PortWriteByte(0x288,0x93);
			Setup();
			
			if(language_flag){
			
			for(int i=0;i<4;i++)
			{
				PortWriteByte(0x288,Message_warning[i]);
				DataSet();
			}
			}
			else{
				for(int i=0;i<8;i++)
			{
				PortWriteByte(0x288,M3[i]);
				DataSet();
			}
			
			}
			Sleep(2000);
			PassWord_input();
		}
		else
		{
		PortWriteByte(0x288,0x01);
		Setup();
		//位置设定
		PortWriteByte(0x288,0x93);
		Setup();
		//信息提示
		if(language_flag){
		
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,Message_false[i]);
		DataSet();
		}	
	}
	else{
		for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,M2[i]);
		DataSet();
		}	
	}
		Sleep(2000);
		BEEP();
		First_View();
		
		}
								}
											}
														}	
}


void BEEP()//验证修改 
{
	for(int i=0;i<20;i++)
	{
		PortWriteByte(0x289,0x40);
		Sleep(120);
		PortWriteByte(0x289,0);
	}

}

void Light(bool i)//验证修改 
{
	byte data;
	if(i)
	{
	for(int j=0;j<500;j++)
		PortWriteByte(0X289,0X80);
	}

}

void Language_change()
{
	language_flag=!language_flag;
	First_View();	
}



bool Judge_num(int* a,int* b)
{
	for(int i=0;i<6;i++)
	{
	if(a[i]!=b[i])
		return false;
	}
	return true; 
}

void Admin_mode()//连续输入3个9解锁 
{
	int Count=0;
	
	PortWriteByte(0x288,0x01);
		Setup();
	//位置设定
	PortWriteByte(0x288,0x83);//右上角 
		Setup();
		//信息提示
		if(language_flag){
		
		for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,Word3[i]);
		DataSet();
		}	
		}
		else{
			for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,E4[i]);
		DataSet();
		}	
		}
		
	int Rcount=0;
	while(Count<3)
	{
		KeyDown();
		Sleep(300);
		if(KeyValue==9)
		Rcount++;
		if(KeyValue==14)
			First_View();
		Count++;

	}
	
	if(language_flag&&Rcount==3){
		PortWriteByte(0x288,0x01);
		Setup();
	//位置设定
	PortWriteByte(0x288,0x93);//右上角 
		Setup();
		//信息提示
		
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,Message_true[i]);
		DataSet();
		}	
		Light(1);
		Sleep(200);
		}
	else if(!language_flag&&Rcount==3)
	{
		PortWriteByte(0x288,0x01);
		Setup();
	//位置设定
	PortWriteByte(0x288,0x93);//右上角 
		Setup();
		//信息提示
		for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,M1[i]);
		DataSet();
		}
		Light(1);
		Sleep(200);
		}
	else if(language_flag&&Rcount!=3)
	{
		PortWriteByte(0x288,0x01);
		Setup();
	//位置设定
	PortWriteByte(0x288,0x93);//右上角 
		Setup();
		//信息提示
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,Message_false[i]);
		DataSet();
		}	
	}
	else if(!language_flag&&Rcount!=3)
	{
		PortWriteByte(0x288,0x01);
		Setup();
	//位置设定
	PortWriteByte(0x288,0x93);//右上角 
		Setup();
		//信息提示
		for(int i=0;i<10;i++)
		{
		PortWriteByte(0x288,E2[i]);
		DataSet();
		}	
	}
	
	Sleep(2000);		
	First_View();
}

void First_View()
{
	
		WrongCount=1;//每次退回主界面实现错误次数清零 
		PortWriteByte(0x288,0x01);
		Setup();
	//位置设定
		PortWriteByte(0x288,0x84);//右上角 
		Setup();
		//信息提示
	if(language_flag){
			
		for(int i=0;i<8;i++)
		{
		PortWriteByte(0x288,Word0[i]);
		DataSet();
		}
	}
	else{
		
		for(int i=0;i<4;i++)
		{
		PortWriteByte(0x288,E1[i]);
		DataSet();
		}
	}
		
		Sleep(100);
		
		KeyDown();

		if(KeyValue==10) PassWord_input();
		
		else if(KeyValue==12) Admin_mode();

		else if(KeyValue==11) PassWord_reset();

		else if(KeyValue==14) Language_change();
		
		else First_View();
}
void Key_Process()
{
First_View();
}

int main()
{
	if(!Startup())

	{

		printf("Wrong Device!");

	}
	else
	{
		
	PortWriteByte(0x28B,0x81);//8255初始化  1000 0001 A输出 C高4输出 C低4输入 B输出 
	PortWriteByte(0x288,0x0c);
	Setup();
	Key_Process();
	return 0;

	}
}
