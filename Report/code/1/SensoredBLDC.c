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

unsigned int HallValue;
unsigned int timer3value;
unsigned int timer3avg;
unsigned char polecount;
 
char *UartRPM,UartRPMarray[5];
int RPM, rpmBalance;

unsigned int StateTableFwd[] = {0x0000, 0x0210, 0x2004, 0x0204,
									0x0801, 0x0810, 0x2001, 0x0000};
unsigned int StateTableRev[] = {0x0000, 0x2001, 0x0810, 0x0801,
									0x0204, 0x2004, 0x0210, 0x0000};



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

	//设置按钮对应的输入端口
	TRISA |= 0x0100;	
	TRISB |= 0x0010;	
	//设置霍尔传感器对应的输入端口
	unlockIO();
	RPINR7bits.IC1R = 0x01;		
	RPINR7bits.IC2R = 0x02;		
	RPINR10bits.IC7R = 0x03;	
	lockIO();
 
	//模块初始化
	InitADC10();
	InitTMR1();
	InitTMR3();
	timer3avg = 0;
	InitMCPWM();
	InitIC();
	
	//设置初始旋转方向
	Flags.Direction = 1;				

	//延时等待各模块初始化完成
	for(i=0;i<1000;i++);

	//主程序循环
	while(1)
	{	
		//等待S2(启停)按钮被按下
		while(S2)					
		{
			//若S3(反向)按钮被按下
			if (!S3)				
			{
				//等待S3被松开
				while (!S3)			
					DelayNmSec(10);
				//切换旋转方向
				Flags.Direction ^= 1;
			}
			Nop();
		}
		//等待S2按钮被松开
		while (!S2)					
			DelayNmSec(10);
			
		//读取霍尔传感器值
		HallValue = (unsigned int)((PORTB >> 1) & 0x0007);
		//根据旋转方向为OVDCON寄存器写入对应扇区的值
		if (Flags.Direction)
			OVDCON = StateTableFwd[HallValue];
		else
			OVDCON = StateTableRev[HallValue];
			
		//使能PWM输出
		PWMCON1 = 0x0777;			
		//电机运转标志位置1
		Flags.RunMotor = 1;			
		//启动timer3计时
		T3CONbits.TON = 1;			
		//初始化极数计数器
		polecount = 1;
		DelayNmSec(100);

		//当电机处于运转状态，循环等待
		while (Flags.RunMotor)			
		{
			//若S2按钮被按下
			if (!S2)						
				{
				//关闭PWM输出
				PWMCON1 = 0x0700;			
				//OVDCON寄存器置0
  				OVDCON = 0x0000;			
				//电机运转标志位置0
				Flags.RunMotor = 0;			
				//等待S2被松开
				while (!S2)					
					DelayNmSec(10);
				}
			Nop();
		}
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
