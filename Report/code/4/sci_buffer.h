/*
 * SCI_BUFFER以结构体形式定义了SCI_BUFFER对象，结构体中开辟了RX_BUFFER和TX_BUFFER空间，以及相应的数据结构指针
 *
 * 还定义了配套的函数，能够实现RX_BUFFER的写入、删除、读取、清零等功能，
 * 和TX_BUFFER的写入（字符、多个字符、字符串）、删除、判断是否为空、输出TX_BUFFER中内容到TX_FIFO等功能
 *
 * 代替UART模块中的RX_FIFO和TX_FIFO，实现更为简洁的程序设计和更高的运行效率，并能实现更为复杂的信息交互功能
 *
 */

#ifndef __SCI_BUFFER_H_
#define __SCI_BUFFER_H_

#include "string.h"
#include "p33FJ32MC204.h"

//定义RX_BUFFER和TX_BUFFER长度
#define RX_BUFFER_LEN 128
#define TX_BUFFER_LEN 128

//定义SCI_BUFFER结构体对象
//依此包含：
//RX_BUFFER
//TX_BUFFER
//RX_BUFFER(尾)指针
//TX_BUFFER头指针
//TX_BUFFER尾指针      从而TX_BUFFER能够实现循环队列的结构
typedef struct _SCI_Buffer_Obj_
{
    volatile int RX_Buffer[RX_BUFFER_LEN];
    volatile int TX_Buffer[TX_BUFFER_LEN];
    volatile int RX_Buffer_Index;
    volatile int TX_Buffer_Head_Index;
    volatile int TX_Buffer_Tail_Index;
}SCI_Buffer_Obj;

//定义SCI_BUFFER结构体指针
typedef struct _SCI_Buffer_Obj_ *SCI_Buffer_Handle;

//初始化SCI_BUFFER结构体指针程序
SCI_Buffer_Handle SCI_Buffer_init(void *pMemory);

//RX_BUFFER写入单个字符
int RX_Buffer_WriteChar(SCI_Buffer_Handle sci_buffer_handle, int RX_Char);
//RX_BUFFER删除末尾单个字符
int RX_Buffer_DeleteLastChar(SCI_Buffer_Handle sci_buffer_handle);
//RX_BUFFER删除末尾n个字符
int RX_Buffer_DeleteLastNChars(SCI_Buffer_Handle sci_buffer_handle, int n);
//RX_BUFFER清空
void RX_Buffer_Flush(SCI_Buffer_Handle sci_buffer_handle);
//RX_BUFFER读取整行数据
char* RX_Buffer_PickLine(SCI_Buffer_Handle sci_buffer_handle);

//TX_BUFFER写入单个字符
int TX_Buffer_WriteChar(SCI_Buffer_Handle sci_buffer_handle, int TX_Char);
//TX_BUFFER写入N个字符
int TX_Buffer_WriteChars(SCI_Buffer_Handle sci_buffer_handle, char *TX_msg, int len);
//TX_BUFFER写入字符串
int TX_Buffer_WriteString(SCI_Buffer_Handle sci_buffer_handle, const char *TX_msg);
//TX_BUFFER溢出预检查
int TX_Buffer_OverflowPreCheck(SCI_Buffer_Handle sci_buffer_handle, int len);
//将TX_BUFFER中的字符导出到TX_FIFO中
int TX_Buffer_Export2TX_FIFO(SCI_Buffer_Handle sci_buffer_handle, int TX_FIFO_Level);
//判断TX_BUFFER是否为空
int TX_Buffer_isEmpty(SCI_Buffer_Handle sci_buffer_handle);

#endif /* SCI_BUFFER_H_ */
