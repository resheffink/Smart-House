//-----------------------------------------------------------------------------//
//Gate_GUI.c
//----------------------------------------------------------------------------
//  Copyright (C) 2016 BRAUDE COLLAGE  
//  Electronics & Electrical Engineering Department
//  All rights reserved.
//  Owners		:  Reshef Finkestein & Dvir Canfi
//  FILE NAME   :  Gate_GUI.c
//  DATE         :  10 JUN 2018
//  TARGET MCU   :  C8051F020
//  DESCRIPTION  :  GUI for Gate electric House - Master unit
//				  
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "User_Interface_def.h"
#include "string.h"
//**********************************************
//-------- Global constants and variables ------
//**********************************************
unsigned    char     UserCode[4]="0000";
unsigned    char     MasterCode[4]="0000";
unsigned	char 	 reaceived_byte;
int Ready_To_Send=0;
unsigned	char	IL1, IL2;
unsigned	char 	key;
unsigned    char    UART0_Buffer[16];
unsigned    char    DeviceNum[1]=" ";
unsigned char transmited_byte;
sbit GreenLED = P1^5;
sbit RedLED = P1^7;
sbit YellowLED = P1^6;
sbit Buzzer = P1^3;
int i=0;
int b=0;
int BTi=0;
int DataReady = 0;
int HouseOpen = 1;
char ASCIITable2[]="01234";

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void Init_Device(void);
void menu1(void);
void OpenMenu(void);
void Display(int pos);
int GetAndVerifyCode(int Get_Verify ,int Master_User_All);
void Setting(void);
void InterfaceIndication(int BuzzerOnOff,int LED_GreenRed,int duration);
void UART0_BT_ISR(void); //-- ISR for UART0 - BT moodule
void AdminLock(void);
void SendUARTData(char* FunkKey);
void GateCon(void);
void RelayCon(void);
void ElecCon(void);
void BT_decide(void);
void BTRecieve(void);
//------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------
void main()
{
	Init_Device();		// initialize the C8051F020
	LCD_INIT(); 		// initialize the LCD screen
	GreenLED = 0;
	RedLED = 0;
	Buzzer = 0;
	YellowLED = 0;
	OpenMenu();	

}
//--------------------------------------------------------------
void UART0_BT_ISR(void) interrupt 4
{

	if(TI0==1)
	{
		TI0=0;
		SBUF0=transmited_byte;
		while(TI0 == 0);
		TI0=0;
	}
}



void SendUARTData(char* FunkKey)
{
	i=0;
	while(FunkKey[i])
	{
	
		if(TI0==0)
		{
			transmited_byte=FunkKey[i];
			i++;
			TI0=1;
		}
		
	}
	
	i=0;
	while(i<1)
	{
		if(TI0==0)
		{
			transmited_byte=DeviceNum[i];
			i++;
			TI0=1;
		}
	}
	i=0;
	DeviceNum[0]=' ';	
}

void OpenMenu(void)
{
	int temp=0;
	int FailEnter=0;
	LCD_GOTO(0x03);
	LCD_MSG("WELCOME TO"); 
	LCD_GOTO(0x43);
	LCD_MSG("SMART HOME");
	huge_delay(5);
CodeInsert:
	LCD_CLRS();
	if(FailEnter==4)
	{
		int keep=5;
		while(keep--)
		{
			InterfaceIndication(1,0,1);
			LCD_GOTO(0);
			LCD_MSG("  FUCK YOU!!!  ");
			LCD_GOTO(0x40);
			LCD_MSG("SYSTEM LOCKDOWN!!");
			huge_delay(5);	
			LCD_CLRS();
		}
		FailEnter=0;
		AdminLock();
	}
	LCD_GOTO(0x03);
	LCD_MSG("ENTER CODE");
	LCD_GOTO(0x45);
	IL1=0x45;
	if(GetAndVerifyCode(0,3))
	{
		LCD_CLRS();
		LCD_GOTO(0x02);
		LCD_MSG("code correct");
		InterfaceIndication(0,1,1);
		huge_delay(1);		
		menu1();
	}
	else
	{
		LCD_CLRS();
		LCD_GOTO(0x01);
		LCD_MSG("code incorrect");
		temp=3-FailEnter;
		LCD_GOTO(0x40);
		LCD_MSG("Attempts Left:");
		LCD_GOTO(0x4E);
		LCD_DAT(ASCIITable2[temp]);
		InterfaceIndication(0,0,2);
		FailEnter++;
		huge_delay(5);
		goto CodeInsert;	
	}

}

void menu1(void)
{
	int choise=0;
	LCD_CLRS();
	LCD_GOTO(0x03);
	LCD_MSG("Main menu");
	LCD_GOTO(0x40);
	while(1)
	{
		
		Display(choise);
	aa:	KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		while(key==0) goto aa;
		
		if(key=='A' && choise>0) choise--;
		else if(key=='D' && choise<5) choise++;
		else if(key=='1')
		{
			LCD_CLRS();
			LCD_GOTO(0);
			LCD_MSG("SYSTEM LOCKDOWN!!");
			huge_delay(5);
			IL2=0;
			AdminLock();
		}
		else if(key=='2') GateCon();
		else if(key=='3') RelayCon();
		else if(key=='4') ElecCon();
		else if(key=='5') Setting();
		else if(key=='#') OpenMenu();
	}

}

void Display(int pos)
{
	switch (pos)
	{
		case 0:
			LCD_CLRS();
			LCD_GOTO(0x03);
			LCD_MSG("Main menu");
			LCD_GOTO(0x40);
			LCD_MSG("1.Home LockDown");
			return;
		case 1:
			LCD_CLRS();
			LCD_GOTO(0x00);
			LCD_MSG("1.Home LockDown");
			LCD_GOTO(0x40);
			LCD_MSG("2.Gate Control");
			return;
		case 2:
			LCD_CLRS();
			LCD_GOTO(0x00);
			LCD_MSG("2.Gate Control");
			LCD_GOTO(0x40);
			LCD_MSG("3.HPower Control");
			return;
		case 3:
			LCD_CLRS();
			LCD_GOTO(0x00);
			LCD_MSG("3.HPower Control");
			LCD_GOTO(0x40);
			LCD_MSG("4.Elect Control");
			return;
		case 4:
			LCD_CLRS();
			LCD_GOTO(0x00);
			LCD_MSG("4.Elect Control");
			LCD_GOTO(0x40);
			LCD_MSG("5.Settings");
			return;
		case 5:
			LCD_CLRS();
			LCD_GOTO(0x00);
			LCD_MSG("5.Settings");
			LCD_GOTO(0x40);
			LCD_MSG("# - Exit System");
	}
	return;
}


void Setting(void)
{
	Code:
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("ENTER Admin CODE");
	LCD_GOTO(0x45);
	IL1=0x45;
	if(GetAndVerifyCode(0,1))
	{
		LCD_CLRS();
		LCD_GOTO(0x02);
		LCD_MSG("code correct");
		InterfaceIndication(0,1,4);
		huge_delay(10);
		while(1)
		{
			LCD_CLRS();
			LCD_GOTO(0x00);
			LCD_MSG("1.New Admin Code");
			LCD_GOTO(0x40);
			LCD_MSG("2.New User Code");		
			KEY_RELEASE();
			PRESSED_KEY();
			key = GET_KEY();
			key = ASCII_CONV(key);
			if(key=='1')
			{
				LCD_CLRS();
				LCD_GOTO(0x00);
				LCD_MSG("Admin new code");
				LCD_GOTO(0x45);
				IL1=0x45;
				if(!GetAndVerifyCode(1,1)) goto ErrCode;
				InterfaceIndication(0,1,4);
				return;
			}
			if(key=='2')
			{
				LCD_CLRS();
				LCD_GOTO(0x00);
				LCD_MSG("User new code");
				LCD_GOTO(0x45);
				IL1=0x45;
				if(!GetAndVerifyCode(1,2)) goto ErrCode;
				InterfaceIndication(0,1,4);
				goto ExitCond;
			}
			if(key=='#') return;
		}
	}
	else
	{
		ErrCode:
		LCD_CLRS();
		LCD_GOTO(0x01);
		LCD_MSG("code incorrect");
		InterfaceIndication(1,0,2);
		huge_delay(10);	
	}
	ExitCond:
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("# - exit to Main");
	LCD_GOTO(0x40);
	LCD_MSG("Any Key to Cont");
	KEY_RELEASE();
	PRESSED_KEY();
	key = GET_KEY();
	key = ASCII_CONV(key);
	if(key=='#') return;
	else goto Code;

}

int GetAndVerifyCode(int Get_Verify, int Master_User_All)
{
	int i=0;
	unsigned char temp1[4];
	unsigned char temp2[4];
	for(i=0;i<4;i++)
	{
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		IL1++;
		LCD_GOTO(IL1); // set cursor address to 0
		LCD_DAT(key);
		temp1[i]=key;
		huge_delay(1);
	}
	for(i=0;i<4;i++)
		if(!Get_Verify)
		{
			if(MasterCode[i]!=temp1[i] || UserCode[i]!=temp1[i] && Master_User_All==3) return 0;
			else if(UserCode[i]!=temp1[i] && Master_User_All==2) return 0;
			else if(MasterCode[i]!=temp1[i] && Master_User_All==1) return 0;
		}
	if(!Get_Verify) return 1;
	LCD_CLRS();
	LCD_GOTO(0x1);
	LCD_MSG("Enter Again:");
	LCD_GOTO(0x45);
	IL1=0x45;
	for(i=0;i<4;i++)
	{
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		IL1++;
		LCD_GOTO(IL1); // set cursor address to 0
		LCD_DAT(key);
		temp2[i]=key;
		huge_delay(1);
	}
	for(i=0;i<4;i++)
		if(temp2[i]!=temp1[i])
		{
			return 0;
		}
	for(i=0;i<4;i++)
		if(Master_User_All==1) MasterCode[i]=temp2[i];
		else if(Master_User_All==2) UserCode[i]=temp2[i];
	return 1;
}

void InterfaceIndication(int BuzzerOnOff,int LED_GreenRed,int duration)
{
	while(duration--)
	{
		if(BuzzerOnOff)
		{
			Buzzer=1;
		}
		if(LED_GreenRed)
		{ 
			GreenLED = 1;
			RedLED = 0;
		}
		else 
		{
			GreenLED = 0;
			RedLED = 1;
		}
		huge_delay(3);
		GreenLED = 0;
		Buzzer=0;
		huge_delay(3);
	}
	Buzzer=0;
	RedLED = 0;
}

void AdminLock(void)
{
	int AdminEntered=0;
	RedLED = 1;
	HouseOpen = 0;
	i=0;
	DeviceNum[0]=' ';
	SendUARTData("LOCKDOWN       ");
	while(!AdminEntered)
	{
		LCD_CLRS();
		LCD_GOTO(0x00);
		LCD_MSG("ENTER Admin CODE");
		LCD_GOTO(0x40);
		LCD_MSG("To Unlock: ");
		LCD_GOTO(0x4A);
		IL1=0x49;
		if(GetAndVerifyCode(0,1)) AdminEntered=1;
	}
	LCD_CLRS();
	LCD_GOTO(0x02);
	LCD_MSG("SYSTEM OPEN");
	HouseOpen = 1;
	RedLED = 0;
	GreenLED = 1;
	huge_delay(5);
	GreenLED = 0;
	LCD_CLRS();
}

void GateCon(void)
{
	DeviceNum[0]=' ';
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("1.Open Gate");
	LCD_GOTO(0x40);
	LCD_MSG("2.Close Gate");
	while(1)
	{
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		if(key=='1')
		{
			SendUARTData("OPEN_GATE      ");
			return;                       
		}
		if(key=='2')
		{
			SendUARTData("CLOSE_GATE     ");
			return;
		}
		LCD_CLRS();
		LCD_GOTO(0x00);
		LCD_MSG("# - exit to Main");
		LCD_GOTO(0x40);
		LCD_MSG("Any Key to Cont");
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		if(key=='#') return;	
	}
}

void RelayCon(void)
{
		HPElecNum:
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("Appliance Sel:");
	LCD_GOTO(0x40);
	LCD_MSG("Num [1-8]:");
	IL1=0x4A;
	KEY_RELEASE();
	PRESSED_KEY();
	key = GET_KEY();
	key = ASCII_CONV(key);
	LCD_GOTO(IL1);
	LCD_DAT(key);
	if(key=='0' || key=='9')
	{
		LCD_CLRS();
		LCD_GOTO(0x00);
		LCD_MSG("Wrong Num");
		goto HPElecNum;
	}
	DeviceNum[0]=key;
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("1.HP Elect ON");
	LCD_GOTO(0x40);
	LCD_MSG("2.HP Elect OFF");
	while(1)
	{
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		if(key=='1')
		{
			SendUARTData("RELAY_ON       ");
			break;
		}
		if(key=='2')
		{
			SendUARTData("RELAY_OFF      ");
			break;
		}
		if(key=='#') return;
		exitCondHP:
		LCD_CLRS();
		LCD_GOTO(0x00);
		LCD_MSG("# - exit to Main");
		LCD_GOTO(0x40);
		LCD_MSG("Any Key to Cont");
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		if(key=='#') return;	
	}
	return;
}

void ElecCon(void)
{
	ElecNum:
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("Appliance Sel:");
	LCD_GOTO(0x40);
	LCD_MSG("Num [1-4]:");
	IL1=0x4A;
	KEY_RELEASE();
	PRESSED_KEY();
	key = GET_KEY();
	key = ASCII_CONV(key);
	LCD_GOTO(IL1);
	LCD_DAT(key);
	if(key=='0' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9')
	{
		LCD_CLRS();
		LCD_GOTO(0x00);
		LCD_MSG("Wrong Num");
		goto ElecNum;
	}
	DeviceNum[0]=key;
	LCD_CLRS();
	LCD_GOTO(0x00);
	LCD_MSG("1.Electric ON");
	LCD_GOTO(0x40);
	LCD_MSG("2.Electric OFF");
	while(1)
	{
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		if(key=='1')
		{
			SendUARTData("ELECTRONIC_ON  ");
			goto exitCondElec;
		}
		if(key=='2')
		{
			SendUARTData("ELECTRONIC_OFF ");
			goto exitCondElec;
		}
		if(key=='#') return;
		exitCondElec:
		LCD_CLRS();
		LCD_GOTO(0x00);
		LCD_MSG("# - exit to Main");
		LCD_GOTO(0x40);
		LCD_MSG("Any Key to Cont");
		KEY_RELEASE();
		PRESSED_KEY();
		key = GET_KEY();
		key = ASCII_CONV(key);
		if(key=='#') return;
		goto ElecNum;	
	}
	
	return;
}