//---------------------------------------------------------------------
//
//							 Software License Agreement
//
// The software supplied herewith by Microchip Technology Incorporated 
// (the “Company”) for its PICmicro® Microcontroller is intended and 
// supplied to you, the Company’s customer, for use solely and 
// exclusively on Microchip PICmicro Microcontroller products. The 
// software is owned by the Company and/or its supplier, and is 
// protected under applicable copyright laws. All rights are reserved. 
//  Any use in violation of the foregoing restrictions may subject the 
// user to criminal sanctions under applicable laws, as well as to 
// civil liability for the breach of the terms and conditions of this 
// license.
//
// THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES, 
// WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
// TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
// PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
// IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
//---------------------------------------------------------------------
//	File:		SensoredBLDC.c
//
//	Written By:		Bill Anderson, Microchip Technology
//						
// 
// The following files should be included in the MPLAB project:
//
//		SensoredBLDC.c		-- Main source code file
//		Interrupts.c
//		Init.c
//		SensoredBLDC.h		-- Header file
//		p33FJ256MC710.gld	-- Linker script file
//				
//
//--------------------------------------------------------------------
//
// Revision History
//
// 06/30/07 -- first version 
//---------------------------------------------------------------------- 

#include "p33FJ32MC204.h"
#include "SensoredBLDC.h"
#include "sci_buffer.h"
#include "BLDC_command.h"
#include "cmdline.h"
/******************************************************************************/
/* Configuration bits                                                         */
/******************************************************************************/

_FOSCSEL(FNOSC_FRC);			// Start with FRC will switch to Primary (XT, HS, EC) Oscillator with PLL
_FOSC(FCKSM_CSECMD & POSCMD_XT);	// Clock Switching Enabled and Fail Safe Clock Monitor is disable
    				        // Primary Oscillator Mode: XT Crystal

_FBS (BSS_NO_FLASH & BWRP_WRPROTECT_OFF);
/* no Boot sector and
   write protection disabled */

_FWDT (FWDTEN_OFF);
/* Turn off Watchdog Timer */

_FGS (GSS_OFF & GCP_OFF & GWRP_OFF);
/* Set Code Protection Off for the General Segment */

_FPOR (PWMPIN_ON & HPOL_ON & LPOL_ON & FPWRT_PWR128);
/* PWM mode is Port registers
   PWM high & low active high
   alternate I2C mapped to SDA1/SCL1
   FPOR power on reset 128ms
*/

_FICD (ICS_PGD3 & JTAGEN_OFF);
/* Use PGC3/PGD3 for programming and debugging */

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
int Command_Interface(char *Command_Line);

struct MotorFlags Flags;

unsigned int HallValue;
unsigned int timer3value;
unsigned int timer3avg;
unsigned char polecount;
 
char *UartRPM,UartRPMarray[5];
int RPM, rpmBalance;

/*************************************************************
	Low side driver table is as below.  In the StateLoTableClk
	and the StateLoTableAntiClk tables, the Low side driver is
	PWM while the high side driver is either on or off.  
*************************************************************/

unsigned int StateTableFwd[] = {0x0000, 0x0210, 0x2004, 0x0204,
									0x0801, 0x0810, 0x2001, 0x0000};
unsigned int StateTableRev[] = {0x0000, 0x2001, 0x0810, 0x0801,
									0x0204, 0x2004, 0x0210, 0x0000};



SCI_Buffer_Handle mySciBuffer;
SCI_Buffer_Obj SCI_Buffer;
#define TRUE	1
#define FALSE	0

void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void) {

	//?????RX_FIFO??????
    int data = 0;
   // U1TXREG = 'V';
    //RX_FIFO???????????
    if(U1STAbits.URXDA != 0)
    {
        //????RX_FIFO??????
        data = U1RXREG;
       // U1RXREG = 0;


        //???????????????SSCOM?????????????
        TX_Buffer_WriteChar(mySciBuffer, data);


        //???????????????
        if ( data == '\r' )
        {
            //?????????????SSCOM??????
            TX_Buffer_WriteChar(mySciBuffer, '\n');
            //?RX_BUFFER??????????????????'\0'
            RX_Buffer_WriteChar(mySciBuffer, 0);
            //???????????????????
            //Command_Interface(RX_Buffer_PickLine(mySciBuffer));

                CMD_SPEED();
            
            //????RX_BUFFER???????????
            RX_Buffer_Flush(mySciBuffer);
        }
        else
        {
            //?????????????RX_BUFFER???
            RX_Buffer_WriteChar(mySciBuffer, data);
        }
        //????????????????
        if ( data == '\b' )
        {
            //?????????????????????????????????????????????????????
            TX_Buffer_WriteChars(mySciBuffer, " \b", 2);
            //?RX_BUFFER????????????????????????????????
            RX_Buffer_DeleteLastNChars(mySciBuffer, 2);
        }

    }
        //IEC0bits.U1RXIE = 1;
	U1STAbits.OERR = 0;
	IFS0bits.U1RXIF = 0;
}
void __attribute__ ((interrupt, no_auto_psv)) _U1TXInterrupt(void) {

	//TX_FIFO?????????TX_FIFO?????TX_BUFFER????TX_BUFFER?????TX_FIFO
    TX_Buffer_Export2TX_FIFO(mySciBuffer, 0);
//U1TXREG = 'V';
    //?TX_BUFFER????????TX_FIFO??
    if ( TX_Buffer_isEmpty(mySciBuffer) )
    {
        IEC0bits.U1TXIE = 0;
    }

	IFS0bits.U1TXIF = 0;
}

void InitClock() {
	PLLFBD = 38;	// M = 40
	CLKDIVbits.PLLPOST = 0;	// N1 = 2
	CLKDIVbits.PLLPRE = 0;	// N2 = 2
	OSCTUN = 0;
	RCONbits.SWDTEN = 0;

// Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x01);		// Initiate Clock Switch to
													// FRC with PLL (NOSC=0b001)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
	while (OSCCONbits.COSC != 0b001);	// Wait for Clock switch to occur

	while(OSCCONbits.LOCK != 1) {};
}

void InitUART2() {
	// This is an EXAMPLE, so brutal typing goes into explaining all bit sets

	// The HPC16 board has a DB9 connector wired to UART2, so we will
	// be configuring this port only


	mySciBuffer = SCI_Buffer_init((void *)&SCI_Buffer);

	_RP21R = 3; //U1Tx=RP20
        _U1RXR = 20;//Rx=RP19


	// configure U1MODE
	U1MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	//U1MODEbits.notimplemented;	// Bit14
	U1MODEbits.USIDL = 0;	// Bit13 Continue in Idle
	U1MODEbits.IREN = 0;	// Bit12 No IR translation
	U1MODEbits.RTSMD = 0;	// Bit11 Simplex Mode
	//U1MODEbits.notimplemented;	// Bit10
	U1MODEbits.UEN = 0;		// Bits8,9 TX,RX enabled, CTS,RTS not
	U1MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
	U1MODEbits.LPBACK = 0;	// Bit6 No Loop Back
	U1MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
	U1MODEbits.URXINV = 0;	// Bit4 IdleState = 1  (for dsPIC)
	U1MODEbits.BRGH = 0;	// Bit3 16 clocks per bit period
	U1MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
	U1MODEbits.STSEL = 0;	// Bit0 One Stop Bit

	// Load a value into Baud Rate Generator.  Example is for 9600.
	// See section 19.3.1 of datasheet.
	//  U1BRG = (Fcy/(16*BaudRate))-1
	//  U1BRG = (37M/(16*9600))-1
	//  U1BRG = 240
	U1BRG = 240;	// 40Mhz osc, 9600 Baud

	// Load all values in for U1STA SFR
	U1STAbits.UTXISEL1 = 1;	//Bit15 Int when Char is transferred (1/2 config!)
	U1STAbits.UTXINV = 0;	//Bit14 N/A, IRDA config
	U1STAbits.UTXISEL0 = 0;	//Bit13 Other half of Bit15
	//U1STAbits.notimplemented = 0;	//Bit12
	U1STAbits.UTXBRK = 0;	//Bit11 Disabled
	//U1STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
	U1STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
	U1STAbits.TRMT = 0;	//Bit8 *Read Only bit*
	U1STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
	U1STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
	U1STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
	U1STAbits.PERR = 0;		//Bit3 *Read Only Bit*
	U1STAbits.FERR = 0;		//Bit2 *Read Only Bit*
	U1STAbits.OERR = 0;		//Bit1 *Read Only Bit*
	U1STAbits.URXDA = 0;	//Bit0 *Read Only Bit*

	//IPC7 = 0x4400;	// Mid Range Interrupt Priority level, no urgent reason

	IFS0bits.U1TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC0bits.U1TXIE = 1;	// Enable Transmit Interrupts
	IFS0bits.U1RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC0bits.U1RXIE = 1;	// Enable Recieve Interrupts

	U1MODEbits.UARTEN = 1;	// And turn the peripheral on

	U1STAbits.URXEN = 1;
	U1STAbits.UTXEN = 1;
	// I think I have the thing working now.

        //U1TXREG = 'B';
}

int main(void)
{
	unsigned int i;

	// Configure Oscillator to operate the device at 20Mhz
	// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	// Fosc= 8*10/(2*2)= 20Mhz for 8M input clock

//	PLLFBD =  8;		        // M=10
//	CLKDIVbits.PLLPOST = 0;		// N1=2
//	CLKDIVbits.PLLPRE = 0;		// N2=2
//	__builtin_write_OSCCONH(0x03);
//	__builtin_write_OSCCONL(0x01);
//
//	while(OSCCONbits.COSC != 0b011);
//	// Wait for PLL to lock
//	while(OSCCONbits.LOCK != 1);
InitClock();
InitUART2();
	TRISA |= 0x0100;	// S2 as input RA8
	TRISB |= 0x0010;	// S3 as input RB4

	// Analog pin for POT already initialized in ADC init subroutine

	unlockIO();
	RPINR7bits.IC1R = 0x01;		// IC1 on RP1/RB1
	RPINR7bits.IC2R = 0x02;		// IC2 on RP2/RB2
	RPINR10bits.IC7R = 0x03;	// IC7 on RP3/RB3
	lockIO();
 
	InitADC10();
	InitTMR1();
	InitTMR3();
	timer3avg = 0;
	InitMCPWM();
	InitIC();

        	// This is the PLL settings
		// Initialize UART2 for 9600,8,N,1 TX/RX
	Flags.Direction = 1;				// initialize direction CW

	for(i=0;i<1000;i++);

	while(1)
	{	
		while(S2)					// wait for start key hit
		{
			if (!S3)				// check for direction change
			{
				while (!S3)			// wait till key is released
					DelayNmSec(10);
					Flags.Direction ^= 1;
			}
			Nop();
		}
		while (!S2)					// wait till key is released
			DelayNmSec(10);
			
			// read hall position sensors on PORTD
		HallValue = (unsigned int)((PORTB >> 1) & 0x0007);
		if (Flags.Direction)
			OVDCON = StateTableFwd[HallValue];
		else
			OVDCON = StateTableRev[HallValue];
			
		PWMCON1 = 0x0777;			// enable PWM outputs
		Flags.RunMotor = 1;			// set flag
		T3CONbits.TON = 1;			// start tmr3
		polecount = 1;
		DelayNmSec(100);

		while (Flags.RunMotor)				// while motor is running
		{
			if (!S2)						// if S2 is pressed
				{
				PWMCON1 = 0x0700;			// disable PWM outputs
  				OVDCON = 0x0000;			// overide PWM low.
				Flags.RunMotor = 0;			// reset run flag
				while (!S2)					// wait for key release
					DelayNmSec(10);
				}
			Nop();
		}
	}
}
//---------------------------------------------------------------------
// This is a generic 1ms delay routine to give a 1mS to 65.5 Seconds delay
// For N = 1 the delay is 1 mS, for N = 65535 the delay is 65,535 mS. 
// Note that FCY is used in the computation.  Please make the necessary
// Changes(PLLx4 or PLLx8 etc) to compute the right FCY as in the define
// statement above.

void DelayNmSec(unsigned int N)
{
unsigned int j;
while(N--)
 	for(j=0;j < MILLISEC;j++);
}

/*********************************************************************************** 
 * Function: lockIO
 *
 * Preconditions: None.
 *
 * Overview: This executes the necessary process to set the IOLOCK bit to lock
 * I/O mapping from being modified.
 *
 * Input: None.
 *
 * Output: None.
 *
 *****************************************************************************/
void lockIO(){

asm volatile ("mov #OSCCON,w1 \n"
				"mov #0x46, w2 \n"
				"mov #0x57, w3 \n"
				"mov.b w2,[w1] \n"
				"mov.b w3,[w1] \n"
				"bset OSCCON, #6");
}

/*****************************************************************************
 * Function: unlockIO
 *
 * Preconditions: None.
 *
 * Overview: This executes the necessary process to clear the IOLOCK bit to 
 * allow I/O mapping to be modified.
 *
 * Input: None.
 *
 * Output: None.
 *
 *****************************************************************************/
void unlockIO(){

asm volatile ("mov #OSCCON,w1 \n"
				"mov #0x46, w2 \n"
				"mov #0x57, w3 \n"
				"mov.b w2,[w1] \n"
				"mov.b w3,[w1] \n"
				"bclr OSCCON, #6");
}

//?????????
int Command_Interface(char *Command_Line)
{
    //??????????
    int Command_Result = 0;

    //????????????
    char Command_Name[20];

    //??cmdline?????????????????
    Command_Result = CmdLineProcess(Command_Line, Command_Name);

    //??????????0(???????)
    switch ( Command_Result )
    {
    //??????????????
    case CMDLINE_BAD_CMD:
        //?????????
        TX_Buffer_WriteString(mySciBuffer, "A ERROR Command is Typed in!\r\nYou can Type \"HELP\" Command to See Which Command is Valid.\r\n");
        break;
    //???????????????????
    case CMDLINE_INVALID_ARG:
        //?????????
        TX_Buffer_WriteString(mySciBuffer, "ERROR Arguments are Typed in for Command \"");
        TX_Buffer_WriteString(mySciBuffer, Command_Name);
        TX_Buffer_WriteString(mySciBuffer, "\"!\r\nYou can Type \"HELP ");
        TX_Buffer_WriteString(mySciBuffer, Command_Name);
        TX_Buffer_WriteString(mySciBuffer, "\" Command to See How to Use this Command.\r\n");
        break;
    }
    //??????????????????
    TX_Buffer_WriteChars(mySciBuffer, "\r\n", 2);
    return(0);

}
