// SmsTraffic.cpp: implementation of the CSmsTraffic class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "SmsTraffic.h"
#include"../Dao/dao.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HWND  CSmsTraffic::m_Handle = 0;
const int IDLE_SMS_NUMBER = 5;//空闲短信机阀值，待发短信数小于该值，为空闲
const int SEND_TIMES_FAILED = 2; // 连续发送失败次数（用于判断发送失败）
const int SEND_SMS_FAILED = 2;// 连续发送失败短信数（用于判断SIM停机）

CSmsTraffic::CSmsTraffic()
{
	m_nSendIn = 0;
	m_nSendOut = 0;
	m_nRecvIn = 0;
	m_nRecvOut = 0;
    m_sendTimes = 0;
    m_sendFailedSms = 0;

	m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	InitializeCriticalSection(&m_csSend);
	InitializeCriticalSection(&m_csRecv);

	// 启动子线程
	AfxBeginThread(SmThread, this, THREAD_PRIORITY_NORMAL);
}

CSmsTraffic::~CSmsTraffic()
{
	SetEvent(m_hKillThreadEvent);			// 发出关闭子线程的信号
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);	// 等待子线程关闭

	DeleteCriticalSection(&m_csSend);
	DeleteCriticalSection(&m_csRecv);

	CloseHandle(m_hKillThreadEvent);
	CloseHandle(m_hThreadKilledEvent);
}

// 将一条短消息放入发送队列
void CSmsTraffic::PutSendMessage(SM_PARAM* pparam)
{
	EnterCriticalSection(&m_csSend);
	memcpy(&m_SmSend[m_nSendIn], pparam, sizeof(SM_PARAM));
	m_nSendIn++;
	if (m_nSendIn >= MAX_SM_SEND)  m_nSendIn = 0;
	LeaveCriticalSection(&m_csSend);
}

// 从发送队列中取一条短消息
BOOL CSmsTraffic::GetSendMessage(SM_PARAM* pparam)
{
	BOOL fSuccess = FALSE;

	EnterCriticalSection(&m_csSend);

	if (m_nSendOut != m_nSendIn)
	{
		memcpy(pparam, &m_SmSend[m_nSendOut], sizeof(SM_PARAM));

		m_nSendOut++;
		if (m_nSendOut >= MAX_SM_SEND)  m_nSendOut = 0;

		fSuccess = TRUE;
	}

	LeaveCriticalSection(&m_csSend);

	return fSuccess;
}

// 将短消息放入接收队列
void CSmsTraffic::PutRecvMessage(SM_PARAM* pparam, int nCount)
{
	EnterCriticalSection(&m_csRecv);

	for (int i = 0; i < nCount; i++)
	{
		memcpy(&m_SmRecv[m_nRecvIn], pparam, sizeof(SM_PARAM));
	
		m_nRecvIn++;
		if (m_nRecvIn >= MAX_SM_RECV)  m_nRecvIn = 0;

		pparam++;
	}

	LeaveCriticalSection(&m_csRecv);
}

// 从接收队列中取一条短消息
BOOL CSmsTraffic::GetRecvMessage(SM_PARAM* pparam)
{
	BOOL fSuccess = FALSE;

	EnterCriticalSection(&m_csRecv);

	if (m_nRecvOut != m_nRecvIn)
	{
		memcpy(pparam, &m_SmRecv[m_nRecvOut], sizeof(SM_PARAM));

		m_nRecvOut++;
		if (m_nRecvOut >= MAX_SM_RECV)  m_nRecvOut = 0;

		fSuccess = TRUE;
	}

	LeaveCriticalSection(&m_csRecv);

	return fSuccess;
}

char* CSmsTraffic::Comport(){
    return this->m_pComport;
}
void CSmsTraffic::set_Comport(char* port)
{
    this->m_pComport = port;
}

int CSmsTraffic::getUnSends()
{
    int result = 0;
    EnterCriticalSection(&m_csSend);
    result = m_nSendIn - m_nSendOut+1;
    LeaveCriticalSection(&m_csSend);
    return result;
}

Status CSmsTraffic::getStatus()
{
    if(m_sendFailedSms>=SEND_SMS_FAILED)
        return Halt;
    else
    {
        int unSendSms = m_nSendIn - m_nSendOut+1;
        if(unSendSms >= IDLE_SMS_NUMBER)
            return Busy;
        else
            return Idle;
    }
    
    
}

void CSmsTraffic::set_Handle(HWND handle)
{
    CSmsTraffic::m_Handle = handle;
}

void CSmsTraffic::info(const char* msg1,const char* msg2,const char* msg3,const char* msg4,const char* msg5,const char* msg6,const char* msg7)
{
    char* pMsg = (char*)malloc(256);
    memset(pMsg,0,256);
    pMsg = strcat(pMsg,msg1);
    if(msg2 != NULL)
        pMsg = strcat(pMsg,msg2);
    if(msg3 != NULL)
        pMsg = strcat(pMsg,msg3);
    if(msg4 != NULL)
        pMsg = strcat(pMsg,msg4);
    if(msg5 != NULL)
        pMsg = strcat(pMsg,msg5);
    if(msg6 != NULL)
        pMsg = strcat(pMsg,msg6);
    if(msg7 != NULL)
        pMsg = strcat(pMsg,msg7);
    SendMessage(m_Handle,WM_INFO_MESSAGE,(UINT)pMsg,0);
}    


UINT CSmsTraffic::SmThread(LPVOID lParam)
{

	CSmsTraffic* p=(CSmsTraffic *)lParam;	// this
    char *pPort = (char *)malloc(20);
    strcpy(pPort,p->m_pComport);
	int nMsg;				// 收到短消息条数
	int nDelete;			// 目前正在删除的短消息编号
	SM_BUFF buff;			// 接收短消息列表的缓冲区
	SM_PARAM param[256];	// 发送/接收短消息缓冲区
	CTime tmOrg, tmNow;		// 上次和现在的时间，计算超时用
	enum {
		stBeginRest,				// 开始休息/延时
		stContinueRest,				// 继续休息/延时
		stSendMessageRequest,		// 发送短消息
		stSendMessageResponse,		// 读取短消息列表到缓冲区
		stSendMessageWaitIdle,		// 发送不成功，等待GSM就绪
		stReadMessageRequest,		// 发送读取短消息列表的命令
		stReadMessageResponse,		// 读取短消息列表到缓冲区
		stDeleteMessageRequest,		// 删除短消息
		stDeleteMessageResponse,	// 删除短消息
		stDeleteMessageWaitIdle,	// 删除不成功，等待GSM就绪
		stExitThread				// 退出
	} nState;				// 处理过程的状态

	// 初始状态
	nState = stBeginRest;

	// 发送和接收处理的状态循环
	while (nState != stExitThread)
	{
		switch(nState)
		{
			case stBeginRest:
                info(_TEXT("短信机："),pPort,_TEXT(" 准备就绪..."));
				tmOrg = CTime::GetCurrentTime();
				nState = stContinueRest;
				break;

			case stContinueRest:
				Sleep(300);
				tmNow = CTime::GetCurrentTime();
				if (p->GetSendMessage(&param[0]))
				{
					nState = stSendMessageRequest;	// 有待发短消息，就不休息了
                    info(_TEXT("短信机："),pPort,_TEXT(" 获取短信成功..."));
				}
				else if (tmNow - tmOrg >= 5)		// 待发短消息队列空，休息5秒
				{
					nState = stReadMessageRequest;	// 转到读取短消息状态
				}
				break;

			case stSendMessageRequest:
				gsmSendMessage(pPort,&param[0]);
				memset(&buff, 0, sizeof(buff));
				tmOrg = CTime::GetCurrentTime();
				nState = stSendMessageResponse;
                p->m_sendTimes++;
				break;

			case stSendMessageResponse:
				Sleep(100);
				tmNow = CTime::GetCurrentTime();
				switch (gsmGetResponse(pPort,&buff))
				{
					case GSM_OK: 
                        setStatus(param[0].index,ssSendSuccess,true);
                        info(_TEXT("短信机："),pPort,_TEXT(" 向号码："),param[0].TPA,_TEXT(" 发送消息："),param[0].TP_UD,"成功！");
                        p->m_sendTimes = 0;
                        p->m_sendFailedSms = 0;
						nState = stBeginRest;
						break;
					case GSM_ERR:
						nState = stSendMessageWaitIdle;
                        if(p->m_sendTimes >= SEND_TIMES_FAILED)
                        {
                            setStatus(param[0].index,ssSendFail,true);
                            info(_TEXT("短信机："),pPort,_TEXT(" 向号码："),param[0].TPA,_TEXT(" 发送消息："),param[0].TP_UD,"失败！");
                            p->m_sendTimes = 0;
                            p->m_sendFailedSms++;
                            nState = stBeginRest;
                        }
						break;
					default:
						if (tmNow - tmOrg >= 10)		// 10秒超时
						{
							nState = stSendMessageWaitIdle;
						}
				}
				break;

			case stSendMessageWaitIdle:
				Sleep(500);
				nState = stSendMessageRequest;		
				break;

			case stReadMessageRequest:
				gsmReadMessageList(pPort);
				memset(&buff, 0, sizeof(buff));
				tmOrg = CTime::GetCurrentTime();
				nState = stReadMessageResponse;
				break;

			case stReadMessageResponse:
				Sleep(100);
				tmNow = CTime::GetCurrentTime();
				switch (gsmGetResponse(pPort,&buff))
				{
					case GSM_OK: 
//						TRACE("  GSM_OK %d\n", tmNow - tmOrg);
						nMsg = gsmParseMessageList(param, &buff);
						if (nMsg > 0)
						{
							p->PutRecvMessage(param, nMsg);
                            for(int i=0;i<nMsg;i++)
                            {
                                insertRecvSms(&param[i]);
                                info(_TEXT("短信机："),pPort,_TEXT(" 接收到消息："),param[0].TP_UD);
                            }
							nDelete = 0;
							nState = stDeleteMessageRequest;
						}
						else
						{
							nState = stBeginRest;
						}
						break;
					case GSM_ERR:
//						TRACE("  GSM_ERR %d\n", tmNow - tmOrg);
                        info(_TEXT("短信机："),pPort,_TEXT(" 接收错误"));
						nState = stBeginRest;
						break;
					default:
//						TRACE("  GSM_WAIT %d\n", tmNow - tmOrg);
						if (tmNow - tmOrg >= 15)		// 15秒超时
						{
//							TRACE("  Timeout!\n");
                            //info(_TEXT("短信机："),pPort,_TEXT(" 接收超时"));
							nState = stBeginRest;
						}
				}
				break;

			case stDeleteMessageRequest:
				if (nDelete < nMsg)
				{
					gsmDeleteMessage(pPort,param[nDelete].index);
                    //info(_TEXT("短信机："),pPort,_TEXT("请求清理收件箱"));
					memset(&buff, 0, sizeof(buff));
					tmOrg = CTime::GetCurrentTime();
					nState = stDeleteMessageResponse;
				}
				else
				{
					nState = stBeginRest;
				}
				break;

			case stDeleteMessageResponse:
				Sleep(100);
				tmNow = CTime::GetCurrentTime();
				switch (gsmGetResponse(pPort,&buff))
				{
					case GSM_OK: 
                        info(_TEXT("短信机："),pPort,_TEXT("从收件箱删除1条短信"));
						nDelete++;
						nState = stDeleteMessageRequest;
						break;
					case GSM_ERR:
                        info(_TEXT("短信机："),pPort,_TEXT("删除短信失败"));
						nState = stDeleteMessageWaitIdle;
						break;
					default:
						if (tmNow - tmOrg >= 5)		// 5秒超时
						{
//							TRACE("  Timeout!\n");
							nState = stBeginRest;
						}
				}
				break;

			case stDeleteMessageWaitIdle:
				Sleep(500);
				nState = stDeleteMessageRequest;		// 直到删除成功为止
				break;
		}

		// 检测是否有关闭本线程的信号
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);
		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;
	}

	// 置该线程结束标志
	SetEvent(p->m_hThreadKilledEvent);
    free(pPort);
	return 9999;
}






