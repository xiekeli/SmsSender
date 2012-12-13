// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(COMM_H__)
#define COMM_H__

const int COMPOOL_SIZE = 18;

BOOL OpenComm(const char* pPort, int nBaudRate=CBR_9600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=ONESTOPBIT);
BOOL CloseComm(const char* pPort);
int ReadComm(const char* pPort,void* pData, int nLength);
int WriteComm(const char* pPort,void* pData, int nLength);
void InitPool();

#endif // !defined(COMM_H__)
