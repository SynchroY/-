#include "p33FJ32MC204.h"
#include "SensoredBLDC.h"
int DesiredSpeed;	//给定速度
int ActualSpeed;	//实际速度
int SpeedError;		//速度误差
//误差积分
long SpeedIntegral = 0, SpeedIntegral_n_1 = 0, SpeedProportional = 0;	
long DutyCycle = 0;	//占空比
unsigned int Kps = 20000;	//比例系数	
unsigned int Kis = 2000;	//积分系数	

//ADC中断
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt (void)
{
  	if (Flags.RunMotor)
		//读取ADC采样值，作为给定速度
		DesiredSpeed = ADC1BUF0 * POTMULT;	

	IFS0bits.AD1IF = 0;
}

//IO端口中断void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt (void)
{
	int Hall_Index;
	
	IFS0bits.IC1IF = 0;	
	//读取霍尔传感器输入
	HallValue = (unsigned int)((PORTB >> 1) & 0x0007);	
	
	if (Flags.Direction)
	{
		//根据霍尔传感器输入与旋转方向，设置OVDCON寄存器值
		OVDCON = StateTableFwd[HallValue];
		//设置电角度起始扇区位置
		Hall_Index = HALL_INDEX_F;
	}
	else
	{
		OVDCON = StateTableRev[HallValue];
		Hall_Index = HALL_INDEX_R;
	}
		
	//电角度旋转一周
	if (HallValue == Hall_Index)  
		//判断是否完成机械角度一周
		if (polecount++ == POLEPAIRS)  
		{							
			//读取timer3计时器读数
			timer3value = TMR3;
			//清零timer3计时器读数
			TMR3 = 0;
			//计时器读书求平均，减小随机误差
			timer3avg = ((timer3avg + timer3value) >> 1);
			//重置极数计数器
			polecount = 1;
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

//定时器Timer1中断
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt (void)
{
	//根据时间间隔计算出实际转速
	ActualSpeed = SPEEDMULT/timer3avg;
	//计算转速误差
	SpeedError = DesiredSpeed - ActualSpeed;
	//计算比例环节输出
	SpeedProportional = (int)(((long)Kps*(long)SpeedError) >> 15);
	//计算积分环节输出
	SpeedIntegral = SpeedIntegral_n_1 + (int)(((long)Kis*(long)SpeedError) >> 15);
	
	//积分环节输出做限幅处理
	if (SpeedIntegral < 0)
		SpeedIntegral = 0;
	else if (SpeedIntegral > 32767)
		SpeedIntegral = 32767;
	//将本次积分环节设置为历史值，供下一次继续使用
	SpeedIntegral_n_1 = SpeedIntegral;
	//计算出占空比输出
	DutyCycle = SpeedIntegral + SpeedProportional;
	//占空比输出做限幅处理
	if (DutyCycle < 0)
		DutyCycle = 0;
	else if (DutyCycle > 32767)
		DutyCycle = 32767;
	
	//将占空比输出结果写入PWM模块
	PDC1 = (int)(((long)(PTPER*2)*(long)DutyCycle) >> 15);
	PDC2 = PDC1;
	PDC3 = PDC1;

	//清除中断标志位
	IFS0bits.T1IF = 0;
}
