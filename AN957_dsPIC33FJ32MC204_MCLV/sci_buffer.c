/*
 * sci_buffer.c
 *
 *  Created on: 2018?3?31?
 *      Author: xhy
 *
 * SCI_BUFFER???????
 *
 */

#include "sci_buffer.h"
#include "p33FJ32MC204.h"



//
//???SCI_BUFFER??
//*pMemory : ???????SCI_BUFFER???????
SCI_Buffer_Handle SCI_Buffer_init(void *pMemory)
{
    //??SCI_BUFFER???????????????
    SCI_Buffer_Handle sci_buffer_handle;
    sci_buffer_handle = (SCI_Buffer_Handle)pMemory;

    //???BUFFER????
    sci_buffer_handle->RX_Buffer_Index = 0;
    sci_buffer_handle->TX_Buffer_Head_Index = 0;
    sci_buffer_handle->TX_Buffer_Tail_Index = 0;

    return(sci_buffer_handle);
}

//
//?RX_BUFFER???????
//sci_buffer_handle : ??SCI_BUFFER????????
//RX_Char : ??RX_BUFFER??????
int RX_Buffer_WriteChar(SCI_Buffer_Handle sci_buffer_handle, int RX_Char)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //?????RX_BUFFER
    sci_buffer->RX_Buffer[sci_buffer->RX_Buffer_Index] = RX_Char;
    //RX_BUFFER????
    sci_buffer->RX_Buffer_Index ++;
    //??RX_BUFFER????
    if ( sci_buffer->RX_Buffer_Index == RX_BUFFER_LEN )
    {
        //???????????
        TX_Buffer_WriteString(sci_buffer, "WARNING! RX_Buffer Just Overflowed! A Larger RX_Buffer is Required!");
        return(-1);
    }
    return(0);
}

//
//??RX_BUFFER???????
//sci_buffer_handle : ??SCI_BUFFER????????
int RX_Buffer_DeleteLastChar(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //?RX_BUFFER?????????????
    if ( sci_buffer->RX_Buffer_Index != 0 )
    {
        sci_buffer->RX_Buffer_Index --;
    }

    return(0);
}

//
//??RX_BUFFER???n??
//sci_buffer_handle : ??SCI_BUFFER????????
//n : ??????
int RX_Buffer_DeleteLastNChars(SCI_Buffer_Handle sci_buffer_handle, int n)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    int i;

    //??n???RX_BUFFER??n?
    for (i=0; i < n; i++)
    {
        if ( sci_buffer->RX_Buffer_Index != 0 )
        {
            sci_buffer->RX_Buffer_Index --;
        }
        else
        {
            break;
        }
    }

    return(0);
}

//
//??RX_BUFFER
//sci_buffer_handle : ??SCI_BUFFER????????
void RX_Buffer_Flush(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //?RX_BUFFER????0
    sci_buffer->RX_Buffer_Index = 0;
}

//
//??RX_BUFFER??????
//sci_buffer_handle : ??SCI_BUFFER????????
char* RX_Buffer_PickLine(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    return((char *)sci_buffer->RX_Buffer);
}

//
//?TX_BUFFER???????
//sci_buffer_handle : ??SCI_BUFFER????????
//TX_Char : ???TX_BUFFER???
int TX_Buffer_WriteChar(SCI_Buffer_Handle sci_buffer_handle, int TX_Char)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //???TX_BUFFER?????
    TX_Buffer_OverflowPreCheck(sci_buffer, 1);
    //???????????
    sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Tail_Index % TX_BUFFER_LEN] = TX_Char;
    //??TX_BUFFER???
    sci_buffer->TX_Buffer_Tail_Index ++;
    //??TX_FIFO???????SCI_TX????
    if (U1STAbits.TRMT == 1)
    {
        TX_Buffer_Export2TX_FIFO(sci_buffer, 0);
    }
    IEC0bits.U1TXIE = 1;
    return(0);
}

//
//?TX_BUFFER???n???
//sci_buffer_handle : ??SCI_BUFFER????????
//TX_msg : ???TX_BUFFER????
//len : ???????
int TX_Buffer_WriteChars(SCI_Buffer_Handle sci_buffer_handle, char *TX_msg, int len)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    int i;

    //???TX_BUFFER?????
    TX_Buffer_OverflowPreCheck(sci_buffer, len);
    //??len????len?????????TX_BUFFER???
    for (i=0; i<len; i++)
    {
        sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Tail_Index % TX_BUFFER_LEN] = TX_msg[i];
        sci_buffer->TX_Buffer_Tail_Index ++;
    }
    if (U1STAbits.TRMT == 1)
    {
        TX_Buffer_Export2TX_FIFO(sci_buffer, 0);
    }
    //??TX_FIFO???????SCI_TX????
    IEC0bits.U1TXIE = 1;
    return(0);
}

//
//?TX_BUFFER???n???
//sci_buffer_handle : ??SCI_BUFFER????????
//TX_msg : ???TX_BUFFER????
int TX_Buffer_WriteString(SCI_Buffer_Handle sci_buffer_handle, const char *TX_msg)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    const char *pChar;
    pChar = TX_msg;
    //???TX_BUFFER?????
    if ( TX_Buffer_OverflowPreCheck(sci_buffer, strlen(TX_msg)) == -2 )
    {
        //???????????
        return(-1);
    }
    //??????????TX_BUFFER
    while ( *pChar )
    {
        sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Tail_Index % TX_BUFFER_LEN] = *pChar;
        sci_buffer->TX_Buffer_Tail_Index ++;
        pChar ++;
    }
    if (U1STAbits.TRMT == 1)
    {
        TX_Buffer_Export2TX_FIFO(sci_buffer, 0);
    }
    //??TX_FIFO???????SCI_TX????
    IEC0bits.U1TXIE = 1;
    return(0);
}

//
//TX_BUFFER?????
//sci_buffer_handle : ??SCI_BUFFER????????
//len : ???????
int TX_Buffer_OverflowPreCheck(SCI_Buffer_Handle sci_buffer_handle, int len)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //???????????????TX_BUFFER??????????????????
    if ( len > TX_BUFFER_LEN )
    {
        TX_Buffer_WriteString(sci_buffer, "Too Long Text! A Larger TX_BUFFER is Required!\r\n");
        return(-2);
    }

    //????TX_BUFFER???????????????????????????????????TX_BUFFER???????????????????????TX_BUFFER?????????????????????
    if ( ( sci_buffer->TX_Buffer_Tail_Index + len ) >= ( sci_buffer->TX_Buffer_Head_Index + TX_BUFFER_LEN ) )
    {
        while ( ( sci_buffer->TX_Buffer_Tail_Index + 52 ) == ( sci_buffer->TX_Buffer_Head_Index + TX_BUFFER_LEN ) ) {}
        TX_Buffer_WriteString(sci_buffer, "A Block Just Occurred! A Larger TX_BUFFER is Required!\r\n");
        while ( ( sci_buffer->TX_Buffer_Tail_Index + len ) == ( sci_buffer->TX_Buffer_Head_Index + TX_BUFFER_LEN ) ) {}
        return(-1);
    }
    return(0);
}

//
//?TX_BUFFER??????TX_FIFO
//sci_buffer_handle : ??SCI_BUFFER????????
//TX_FIFO_Level : ???TX_FIFO?????????????TX_BUFFER??TX_FIFO??????
int TX_Buffer_Export2TX_FIFO(SCI_Buffer_Handle sci_buffer_handle, int TX_FIFO_Level)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //??????????TX_FIFO?????
    int i = 4 - TX_FIFO_Level;
    //????i????TX_FIFO?????TX_BUFFER???
    while ( ( --i >= 0 ) && ( sci_buffer->TX_Buffer_Head_Index < sci_buffer->TX_Buffer_Tail_Index) )
    {
       U1TXREG = sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Head_Index];
       // U1TXREG = 'V';
        //???????????????????????TX_BUFER??
        if ( ++ (sci_buffer->TX_Buffer_Head_Index) >= TX_BUFFER_LEN )
        {
            sci_buffer->TX_Buffer_Head_Index = sci_buffer->TX_Buffer_Head_Index - TX_BUFFER_LEN;
            sci_buffer->TX_Buffer_Tail_Index = sci_buffer->TX_Buffer_Tail_Index - TX_BUFFER_LEN;
        }
    }

    return(0);
}

//
//??TX_BUFFER????
//sci_buffer_handle : ??SCI_BUFFER????????
int TX_Buffer_isEmpty(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //???????????TX_BUFFER??
    if ( sci_buffer->TX_Buffer_Head_Index == sci_buffer->TX_Buffer_Tail_Index )
    {
        return(1);
    }
    return(0);
}



