
#include "User_Interface_def.h"
#include "string.h"
//#include<reg51.h>
//**********************************************
//-------- Global constants and variables ------
//**********************************************
unsigned	char received_byte;
unsigned	char 	keys[16];
int Index=0,dataredy;
int KeyIndex=0;
sbit P1_0=P1^0;
sbit P1_1=P1^1;
sbit P1_2=P1^2;
sbit P1_3=P1^3;
sbit P1_4=P1^4;
sbit P1_5=P1^5;
sbit P1_6=P1^6;
sbit P1_7=P1^7;
sbit Servomotor_pin=P0^4;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void Init_Device(void);
void HomeLockDown(void);
void servo(int open);
void OpenHouse(void);
void UART1_ISR(void);
void GetData(void);
void GateCon(char* status);
void HPCon(int Device_ID,char* status);
void ELECon(char Device_ID,char* status);
//------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------
void main()
{

	Init_Device();
	SCON1 =SCON1&0xFC;
	P5=(P5 & 0x0F);	
	Index=0;
	
	
	while(1)
	{

		Index=0;
		while(1)
		{
			if(SCON1&0x01 == 1)
			{
				keys[Index]=received_byte;
				SCON1 =SCON1&0xFC;
				Index++;
			}
			if(Index>15)
			{
		 		Index=0;
		 		dataredy=1;
		 		break;
			}
		}
		if(dataredy==1)
		{
			dataredy=0;
			if(!strncmp(keys,"OPEN_GATE",9) || !strncmp(keys,"CLOSE_GATE",10)) GateCon(keys);
			if(!strncmp(keys,"RELAY_ON",8) || !strncmp(keys,"RELAY_OFF",9)) HPCon(keys[15],keys);
	 		if(!strncmp(keys,"ELECTRONIC_ON",13) || !strncmp(keys,"ELECTRONIC_OFF",14)) ELECon(keys[15],keys);
			if(!strncmp(keys,"LOCKDOWN",8)) HomeLockDown();
	
			for(KeyIndex=0;KeyIndex<16;KeyIndex++)
			{
				keys[KeyIndex]='0';
			}	
		}

	}
}
//--------------------------------------------------------------

void UART1_ISR(void) interrupt 20
{
	if (SCON1&0x01 == 1) 
	{
		received_byte = SBUF1;		
	}

}
void GetData(void)
{
	Index=0;
	while(1)
	{
		if(SCON1&0x01 == 1)
		{
			keys[Index]=received_byte;
			Index++;
			SCON1 =SCON1&0xFE;
		}
		if(Index>15)
		{
			Index=0;
			dataredy=1;
			break;
		}
	}
}


void HomeLockDown(void)
{
	HPCon("all_off");
	ELECon("all_off");
	servo(0);
}


void GateCon(char* status)
{
	if (!strncmp(status,"OPEN_GATE",9))
		{
			servo(1);
		}
	else if (!strncmp(status,"CLOSE_GATE",10))
		{
			servo(0);
		}
		dataredy=0;			
	
}

void HPCon(int Device_ID,char* status)
{
	if (!(strncmp(status,"all_off",7)))
	{
		P1=0xFF;
		return;
	}
	switch (Device_ID)
	{
		case '1':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_0=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_0=0;
			}
			return;
		}
		case '2':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_1=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_1=0;
			}
			return;
		}
		case '3':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_2=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_2=0;
			}
			return;
		}
		case '4':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_3=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_3=0;
			}
			return;
		}
		case '5':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_4=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_4=0;
			}
			return;
		}
		case '6':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_5=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_5=0;
			}
			return;
		}
		case '7':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_6=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_6=0;
			}
			return;
		}
		case '8':
		{
			if (!strncmp(status,"RELAY_ON",8))
			{
				P1_7=1;
			}
			if (!strncmp(status,"RELAY_OFF",9))
			{
				P1_7=0;
			}
			return;
		}
	}
}


void ELECon(char Device_ID,char* status)
{
	if (!strncmp(status,"all_off",7))
	{
		P5=0x00;
		dataredy=0;
		return;
	}
	switch (Device_ID)
	{	
		case '1':
		{
			if (!strncmp(status,"ELECTRONIC_ON",13))
			{
				P5=(P5 | 0x1F);	
			}
			if (!strncmp(status,"ELECTRONIC_OFF",14))
			{
				P5=(P5 & 0xEF);	
			}
			return;
		}
		case '2':
		{
			if (!strncmp(status,"ELECTRONIC_ON",13))
			{
				P5=(P5 | 0x2F);	
			}
			if (!strncmp(status,"ELECTRONIC_OFF",14))
			{
				P5=(P5 & 0xDF);	
			}
			return;
		}
		case '3':
		{
			if (!strncmp(status,"ELECTRONIC_ON",13))
			{
				P5=(P5 | 0x4F);	
			}
			if (!strncmp(status,"ELECTRONIC_OFF",14))
			{
				P5=(P5 & 0xBF);	
			}
			return;
		}
		case '4':
		{
			if (!strncmp(status,"ELECTRONIC_ON",13))
			{
				P5=(P5 | 0x8F);	
			}
			if (!strncmp(status,"ELECTRONIC_OFF",14))
			{
				P5=(P5 & 0x7F);	
			}
			return;
		}
	}	
}

void servo(int open)
{
	if(open==1)
	{
		PCA0CPH0=0xF9;
	}
	else
	{
		PCA0CPH0=0xE1;
	} 
}
