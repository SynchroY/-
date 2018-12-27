#include "sci_buffer.h"
#include "p33FJ32MC204.h"


//初始化SCI_BUFFER对象
//*pMemory : 主函数中定义的SCI_BUFFER对象的内存地址
SCI_Buffer_Handle SCI_Buffer_init(void *pMemory)
{
    //创建SCI_BUFFER结构体指针指向该内存空间并返回
    SCI_Buffer_Handle sci_buffer_handle;
    sci_buffer_handle = (SCI_Buffer_Handle)pMemory;

    //初始化BUFFER数组指针
    sci_buffer_handle->RX_Buffer_Index = 0;
    sci_buffer_handle->TX_Buffer_Head_Index = 0;
    sci_buffer_handle->TX_Buffer_Tail_Index = 0;

    return(sci_buffer_handle);
}

//向RX_BUFFER中写入单个字符
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//RX_Char : 要向RX_BUFFER中写入的字符
int RX_Buffer_WriteChar(SCI_Buffer_Handle sci_buffer_handle, int RX_Char)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //将字符写入RX_BUFFER
    sci_buffer->RX_Buffer[sci_buffer->RX_Buffer_Index] = RX_Char;
    //RX_BUFFER指针递增
    sci_buffer->RX_Buffer_Index ++;
    //判断RX_BUFFER是否溢出
    if ( sci_buffer->RX_Buffer_Index == RX_BUFFER_LEN )
    {
        //若溢出，则发送警告信息
        TX_Buffer_WriteString(sci_buffer, "WARNING! RX_Buffer Just Overflowed! A Larger RX_Buffer is Required!");
        return(-1);
    }
    return(0);
}

//删除RX_BUFFER末尾的单个字符
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
int RX_Buffer_DeleteLastChar(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //将RX_BUFFER指针前移，即删除了末尾字符
    if ( sci_buffer->RX_Buffer_Index != 0 )
    {
        sci_buffer->RX_Buffer_Index --;
    }

    return(0);
}

//删除RX_BUFFER末尾的n字符
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//n : 删除的字符数
int RX_Buffer_DeleteLastNChars(SCI_Buffer_Handle sci_buffer_handle, int n)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    int i;

    //循环n次，将RX_BUFFER递减n次
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

//清空RX_BUFFER
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
void RX_Buffer_Flush(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //将RX_BUFFER指针指向0
    sci_buffer->RX_Buffer_Index = 0;
}

//读取RX_BUFFER中的全部数据
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
char* RX_Buffer_PickLine(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    return((char *)sci_buffer->RX_Buffer);
}

//向TX_BUFFER中写入单个字符
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//TX_Char : 要写入TX_BUFFER的字符
int TX_Buffer_WriteChar(SCI_Buffer_Handle sci_buffer_handle, int TX_Char)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //先进行TX_BUFFER溢出预检查
    TX_Buffer_OverflowPreCheck(sci_buffer, 1);
    //预检查完成，写入该字符
    sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Tail_Index % TX_BUFFER_LEN] = TX_Char;
    //递增TX_BUFFER尾指针
    sci_buffer->TX_Buffer_Tail_Index ++;
    //使能TX_FIFO中断，开始通过SCI_TX发送字符
    if (U1STAbits.TRMT == 1)
    {
        TX_Buffer_Export2TX_FIFO(sci_buffer, 0);
    }
    IEC0bits.U1TXIE = 1;
    return(0);
}

//向TX_BUFFER中写入n个字符
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//TX_msg : 要写入TX_BUFFER的字符串
//len : 写入字符的个数
int TX_Buffer_WriteChars(SCI_Buffer_Handle sci_buffer_handle, char *TX_msg, int len)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    int i;

    //先进行TX_BUFFER溢出预检查
    TX_Buffer_OverflowPreCheck(sci_buffer, len);
    //循环len次，写入len个字符，并依此递增TX_BUFFER尾指针
    for (i=0; i<len; i++)
    {
        sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Tail_Index % TX_BUFFER_LEN] = TX_msg[i];
        sci_buffer->TX_Buffer_Tail_Index ++;
    }
    if (U1STAbits.TRMT == 1)
    {
        TX_Buffer_Export2TX_FIFO(sci_buffer, 0);
    }
    //使能TX_FIFO中断，开始通过SCI_TX发送字符
    IEC0bits.U1TXIE = 1;
    return(0);
}

//向TX_BUFFER中写入n个字符
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//TX_msg : 要写入TX_BUFFER的字符串
int TX_Buffer_WriteString(SCI_Buffer_Handle sci_buffer_handle, const char *TX_msg)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    const char *pChar;
    pChar = TX_msg;
    //先进行TX_BUFFER溢出预检测
    if ( TX_Buffer_OverflowPreCheck(sci_buffer, strlen(TX_msg)) == -2 )
    {
        //长度过长，返回错误信息
        return(-1);
    }
    //依此写入整个字符串到TX_BUFFER
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
    //使能TX_FIFO中断，开始通过SCI_TX发送字符
    IEC0bits.U1TXIE = 1;
    return(0);
}

//TX_BUFFER溢出预检测
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//len : 写入字符的个数
int TX_Buffer_OverflowPreCheck(SCI_Buffer_Handle sci_buffer_handle, int len)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //如果写入字符的长度已经大于整个TX_BUFFER的大小，则直接输出错误信息并返回错误
    if ( len > TX_BUFFER_LEN )
    {
        TX_Buffer_WriteString(sci_buffer, "Too Long Text! A Larger TX_BUFFER is Required!\r\n");
        return(-2);
    }

    //如果当前TX_BUFFER剩余大小不能容纳需要写入的字符串，则为了输出数据完整，暂时以阻塞式等待TX_BUFFER中有足够的空间
	//并发出错误信息，提醒使用者扩大TX_BUFFER大小，以免程序总是运行在阻塞模式，降低效率
    if ( ( sci_buffer->TX_Buffer_Tail_Index + len ) >= ( sci_buffer->TX_Buffer_Head_Index + TX_BUFFER_LEN ) )
    {
        while ( ( sci_buffer->TX_Buffer_Tail_Index + 52 ) == ( sci_buffer->TX_Buffer_Head_Index + TX_BUFFER_LEN ) ) {}
        TX_Buffer_WriteString(sci_buffer, "A Block Just Occurred! A Larger TX_BUFFER is Required!\r\n");
        while ( ( sci_buffer->TX_Buffer_Tail_Index + len ) == ( sci_buffer->TX_Buffer_Head_Index + TX_BUFFER_LEN ) ) {}
        return(-1);
    }
    return(0);
}

//将TX_BUFFER中的内容写入TX_FIFO
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
//TX_FIFO_Level : 设定的TX_FIFO的中断深度，对应一次可以从TX_BUFFER写入TX_FIFO中的字符数量
int TX_Buffer_Export2TX_FIFO(SCI_Buffer_Handle sci_buffer_handle, int TX_FIFO_Level)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //计算可以本次可以写入TX_FIFO的字符数量
    int i = 4 - TX_FIFO_Level;
    //依此写入i个字符到TX_FIFO中，并更新TX_BUFFER头指针
    while ( ( --i >= 0 ) && ( sci_buffer->TX_Buffer_Head_Index < sci_buffer->TX_Buffer_Tail_Index) )
    {
       U1TXREG = sci_buffer->TX_Buffer[sci_buffer->TX_Buffer_Head_Index];

       //循环队列的已进行了一次循环，则头尾指针同时减去TX_BUFER长度
       if ( ++ (sci_buffer->TX_Buffer_Head_Index) >= TX_BUFFER_LEN )
       {
           sci_buffer->TX_Buffer_Head_Index = sci_buffer->TX_Buffer_Head_Index - TX_BUFFER_LEN;
           sci_buffer->TX_Buffer_Tail_Index = sci_buffer->TX_Buffer_Tail_Index - TX_BUFFER_LEN;
       }
    }

    return(0);
}

//判断TX_BUFFER是否为空
//sci_buffer_handle : 指向SCI_BUFFER结构体对象的指针
int TX_Buffer_isEmpty(SCI_Buffer_Handle sci_buffer_handle)
{
    SCI_Buffer_Obj *sci_buffer = (SCI_Buffer_Obj *)sci_buffer_handle;

    //若头指针等于尾指针，则TX_BUFFER为空
    if ( sci_buffer->TX_Buffer_Head_Index == sci_buffer->TX_Buffer_Tail_Index )
    {
        return(1);
    }
    return(0);
}



