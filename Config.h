// Config.h: interface for the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIG_H__3A312D87_9238_455D_B675_7861F13CB211__INCLUDED_)
#define AFX_CONFIG_H__3A312D87_9238_455D_B675_7861F13CB211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CConfig  
{
public:
	CConfig();
	virtual ~CConfig();

    CString SmsC();
    int BautRate();
    int PoolSize();
private:
    CString m_Smsc;//短信中心号码
    int m_BautRate;//波特率
    int m_poolSize;//串口池大小（有效串口数）

    CString m_exePath,m_fileName;

    CString GetParaStr(LPTSTR mainKey,LPTSTR key,CString strDefaul);
    void SetParaInt(LPTSTR mainKey,LPTSTR key,int intValue);
    
    void SetParaStr(LPTSTR mainKey,LPTSTR key,CString strValue);
    
};

#endif // !defined(AFX_CONFIG_H__3A312D87_9238_455D_B675_7861F13CB211__INCLUDED_)
