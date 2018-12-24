/*
 * 9_4_commands.c
 *
 *  Created on: 2018Äê3ÔÂ31ÈÕ
 *      Author: xhy
 *
 * ¶¨ÒåÁËÃüÁîÐÐ½á¹¹ÌåÁÐ±íºÍ¶ÔÓ¦µÄÃüÁîÊµÏÖº¯Êý
 *
 */

#include "cmdline.h"
#include "BLDC_command.h"


#include "string.h"
#include "sci_buffer.h"
#include <stdio.h>




extern SCI_Buffer_Handle mySciBuffer;
extern int ActualSpeed;

//µ¼ÈëÖ÷º¯ÊýÖÐ¶¨ÒåµÄ´æ´¢µ±Ç°µçÑ¹ÖµºÍÎÂ¶ÈµÄ±äÁ¿
extern float currentVoltage_A[3];
extern float currentTemp;






//
//ÃüÁîÐÐ½á¹¹ÌåÁÐ±í£¬Ä©Î²ÔªËØÐèÒªÎªÒ»¸ö¿Õ½á¹¹Ìå
//Êý×éÖÐµÄÃ¿¸öÔªËØÎªÒ»¸öÃüÁî¶ÔÓ¦µÄ£º
//´æ´¢ÃüÁîÃû³ÆµÄ×Ö·û´®
//Ö¸Ïò¶¨ÒåÃüÁî´¦Àíº¯ÊýµØÖ·µÄÖ¸Õë
//´æ´¢ÃüÁî¼òÒª°ïÖúÐÅÏ¢µÄ×Ö·û´®
//´æ´¢ÃüÁîÏêÏ¸°ïÖúÐÅÏ¢µÄ×Ö·û´®
tCmdLineEntry g_psCmdTable[] =
{
    {"SPEED",     CMD_SPEED,
     " : Turn on the LEDs Specified by a Control Number",
     "Four LEDs on the board will light to display the number N in its BIN base format.\r\nWhere N can be either DEC(0~16) or HEX(0~9,A~F|a~F) base format.\r\nThe return message shows the current state of the four LEDs( # = ON; . = OFF)\r\n"},
  
    { 0, 0, 0，0 }
};

//
//ÊµÏÖÌâÄ¿1µÄLEDÏÔÊ¾ÊäÈëÊý×ÖµÄº¯Êý
int
CMD_SPEED(void)
{
    //
    // Keep the compiler happy.
    //
 //   (void)argc;
 //   (void)argv;
    char ANSWER[50]="";

    sprintf(ANSWER,"The speed is %d rpm.\r\n", ActualSpeed);
    TX_Buffer_WriteString(mySciBuffer, ANSWER);
    return(0);
}



