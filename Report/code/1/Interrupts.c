#include "p33FJ32MC204.h"
#include "SensoredBLDC.h"

//ADC中断
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt (void)
{
  	if (Flags.RunMotor)
	{
		//读取ADC采样得到的电位器分压值，作为占空比
		P1DC1 =  (ADC1BUF0 >> 1);	
		P1DC2 = P1DC1;				
		P1DC3 = P1DC1;				
	}

	IFS0bits.AD1IF = 0;
}

//IO端口中断，当霍尔传感器输入变化时改变OVDCON寄存器的值void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt (void)
{
	
	IFS0bits.IC1IF = 0;	
	HallValue = (unsigned int)((PORTB >> 1) & 0x0007);
	
	if (Flags.Direction)
	{
		OVDCON = StateTableFwd[HallValue];
	}
	else
	{
		OVDCON = StateTableRev[HallValue];
	}

}

//IO端口中断，当霍尔传感器输入变化时改变OVDCON寄存器的值void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt (void)
{
	IFS0bits.IC2IF = 0;	
	HallValue = (unsigned int)((PORTB >> 1) & 0x0007);	

	if (Flags.Direction)
		OVDCON = StateTableFwd[HallValue];
	else
		OVDCON = StateTableRev[HallValue];
}

//IO端口中断，当霍尔传感器输入变化时改变OVDCON寄存器的值void __attribute__((interrupt, no_auto_psv)) _IC7Interrupt (void)
{	
	IFS1bits.IC7IF = 0;	
	HallValue = (unsigned int)((PORTB >> 1) & 0x0007);	
	if (Flags.Direction)
		OVDCON = StateTableFwd[HallValue];
	else
		OVDCON = StateTableRev[HallValue];
}

