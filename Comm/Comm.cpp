#include "../stdafx.h"
#include "Comm.h"

typedef struct{
    char* pPort;
    HANDLE hComm;
} ComPort,*PComPort;

int defaltIndex = -1;

ComPort ComPool[COMPOOL_SIZE];

BOOL IsOpened(const char* pPort){
    BOOL result = false;
    for(int i=0;i<COMPOOL_SIZE;i++)
    {
        if(ComPool[i].pPort == NULL)
            return false;
        if(strcmp(ComPool[i].pPort,pPort)==0)
            result = true;
    }
    return result;
}

HANDLE HandleOfPort(const char* pPort,int &index = defaltIndex){
    HANDLE result = NULL;
    index = -1;
    for(int i=0;i<COMPOOL_SIZE;i++)
    {
        if(ComPool[i].pPort!=NULL)
        {
            if(strcmp(ComPool[i].pPort,pPort)==0)
            {
                result = ComPool[i].hComm;
                index = i;
                break;
            }
        }
        
    }
    return result;
}

int searchValidIndex(const char* pPort){
    int result =-1;
    for(int i=0;i<COMPOOL_SIZE;i++)
    {
        if(ComPool[i].hComm == NULL)
        {
            result = i;
            break;
        }
    }
    return result;
}

// 打开串口
// 输入: pPort - 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者
//       nBaudRate - 波特率
//       nParity - 奇偶校验
//       nByteSize - 数据字节宽度
//       nStopBits - 停止位
BOOL OpenComm(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
    if(IsOpened(pPort))
        return true;

	DCB dcb;		// 串口控制块
    
	COMMTIMEOUTS timeouts = {	// 串口超时控制参数
		100,				// 读字符间隔超时时间: 100 ms
		1,					// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
		500,				// 基本的(额外的)读超时时间: 500 ms
		1,					// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
		100};				// 基本的(额外的)写超时时间: 100 ms


	HANDLE hComm = CreateFile(pPort,	// 串口名称或设备路径
			GENERIC_READ | GENERIC_WRITE,	// 读写方式
			0,				// 共享方式：独占
			NULL,			// 默认的安全描述符
			OPEN_EXISTING,	// 创建方式
			0,				// 不需设置文件属性
			NULL);			// 不需参照模板文件
	
	if(hComm == INVALID_HANDLE_VALUE) return FALSE;		// 打开串口失败

	GetCommState(hComm, &dcb);		// 取DCB

	dcb.BaudRate = nBaudRate;
	dcb.ByteSize = nByteSize;
	dcb.Parity = nParity;
	dcb.StopBits = nStopBits;
    dcb.fBinary = TRUE;
    dcb.fDtrControl =RTS_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fOutxCtsFlow = FALSE;

	SetCommState(hComm, &dcb);		// 设置DCB

	SetupComm(hComm, 4096, 1024);	// 设置输入输出缓冲区大小

	SetCommTimeouts(hComm, &timeouts);	// 设置超时

    int index  =searchValidIndex(pPort);
    if((index>=0)&&(index<COMPOOL_SIZE))
    {
        PComPort pComPort = new ComPort();
        pComPort->hComm = hComm;
        pComPort->pPort = (char*)malloc(8*sizeof(char*));
        strcpy(pComPort->pPort,pPort);
        //pComPort->pPort = (char *)pPort;
        ComPool[index] = *pComPort;
    }
	return TRUE;
}

// 关闭串口
BOOL CloseComm(const char* pPort)
{
    BOOL result =false;
    int index = -1;
    HANDLE hComm = HandleOfPort(pPort,index);
    if(hComm != NULL)
    {
	    result =  CloseHandle(hComm);
        if(result)
            ComPool[index].hComm = NULL;
    }
    return result;
}

// 写串口
// 输入: pData - 待写的数据缓冲区指针
//       nLength - 待写的数据长度
// 返回: 实际写入的数据长度
int WriteComm(const char* pPort,void* pData, int nLength)
{
	DWORD dwNumWrite =0 ;	// 串口发出的数据长度
    HANDLE hComm = HandleOfPort(pPort);
    if(hComm != NULL)
    {
        PurgeComm(hComm,PURGE_RXCLEAR);
	    WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);
    }
	return (int)dwNumWrite;
}

// 读串口
// 输入: pData - 待读的数据缓冲区指针
//       nLength - 待读的最大数据长度
// 返回: 实际读出的数据长度
int ReadComm(const char* pPort,void* pData, int nLength)
{
	DWORD dwNumRead = 0;	// 串口收到的数据长度
    HANDLE hComm = HandleOfPort(pPort);
    if(hComm != NULL)
    {
        
	    ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
/*        PurgeComm(hComm, PURGE_TXABORT|
		  PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);*/
    }
	
	return (int)dwNumRead;
}


void InitPool()
{
    for(int i=0;i<COMPOOL_SIZE;i++)
    {
        ComPool[i].hComm = NULL;
        ComPool[i].pPort = NULL;
    }
}