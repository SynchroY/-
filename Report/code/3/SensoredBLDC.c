#include "p33FJ32MC204.h"
#include "SensoredBLDC.h"

_FOSCSEL(FNOSC_FRC);		
_FOSC(FCKSM_CSECMD & POSCMD_XT);

_FBS (BSS_NO_FLASH & BWRP_WRPROTECT_OFF);

_FWDT (FWDTEN_OFF);

_FGS (GSS_OFF & GCP_OFF & GWRP_OFF);

_FPOR (PWMPIN_ON & HPOL_ON & LPOL_ON & FPWRT_PWR128);

_FICD (ICS_PGD3 & JTAGEN_OFF);

void InitADC10(void);
void DelayNmSec(unsigned int N);
void InitMCPWM(void);
void InitTMR3(void);
void InitIC(void);
void CalculateDC(void);
void ResetPowerModule(void);
void InitTMR1(void);
void lockIO(void);
void unlockIO(void);

struct MotorFlags Flags;

unsigned int index = 0;

unsigned int StateTableFwd[] = {0x0100, 0x0200, 0x0400, 0x0800,
									0x1000, 0x2000};



int main(void)
{
	unsigned int i;

	//初始化时钟
	PLLFBD =  8;		       
	CLKDIVbits.PLLPOST = 0;		
	CLKDIVbits.PLLPRE = 0;		
	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);

	//初始化锁相环
	while(OSCCONbits.COSC != 0b011);
	while(OSCCONbits.LOCK != 1);
 
	//模块初始化
	InitMCPWM();
	InitIC();		

	//延时等待各模块初始化完成
	for(i=0;i<1000;i++);
	
	//使能PWM输出
	PWMCON1 = 0x0777;	

	//主程序循环
	while(1)
	{	

		if (++index == 6)
			index = 0;
		OVDCON = StateTableFwd[index];

		DelayNmSec(200);
	}
}

//延时函数
void DelayNmSec(unsigned int N)
{
unsigned int j;
while(N--)
 	for(j=0;j < MILLISEC;j++);
}


void lockIO(){

asm volatile ("mov #OSCCON,w1 \n"
				"mov #0x46, w2 \n"
				"mov #0x57, w3 \n"
				"mov.b w2,[w1] \n"
				"mov.b w3,[w1] \n"
				"bset OSCCON, #6");
}

void unlockIO(){

asm volatile ("mov #OSCCON,w1 \n"
				"mov #0x46, w2 \n"
				"mov #0x57, w3 \n"
				"mov.b w2,[w1] \n"
				"mov.b w3,[w1] \n"
				"bclr OSCCON, #6");
}
