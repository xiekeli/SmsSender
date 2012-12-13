// Config.cpp: implementation of the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMS_Sender.h"
#include "Config.h"
#include "sys_utility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const CString INI_EXT = _TEXT(".ini");

const CString COMM_KEY =_TEXT("COMM_PARAMETER");
const CString SMSCENTERCODE_DEFAULT = "13800571500";
const int COMPOOL_SIZE_DEFAULT = 18;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfig::CConfig()
{
    m_exePath=SysUtility::getBasePath();
    m_fileName =m_exePath+"\\"+AfxGetApp()->m_pszExeName+INI_EXT;

    this->m_BautRate = GetPrivateProfileInt(COMM_KEY,_TEXT("BautRate"),CBR_9600,m_fileName);
    this->m_Smsc = GetParaStr(_TEXT("COMM_PARAMETER"),_TEXT("SMS_CenterCode"),SMSCENTERCODE_DEFAULT);
    this->m_poolSize  = GetPrivateProfileInt(COMM_KEY,_TEXT("PoolSize"),COMPOOL_SIZE_DEFAULT,m_fileName);
}

CConfig::~CConfig()
{
    SetParaInt(_TEXT("COMM_PARAMETER"),_TEXT("BautRate"),m_BautRate);
    SetParaStr(_TEXT("COMM_PARAMETER"),_TEXT("SMS_CenterCode"),m_Smsc);
}

CString CConfig::SmsC()
{
    return m_Smsc;
}


int CConfig::BautRate()
{
    return m_BautRate;
}

int CConfig::PoolSize()
{
    return m_poolSize;
}

CString CConfig::GetParaStr(LPTSTR mainKey,LPTSTR key,CString strDefaul){
    TCHAR buf[255];
    int iReturn=GetPrivateProfileString(mainKey,key,strDefaul,buf,250,m_fileName);
    return buf;
}


void CConfig::SetParaInt(LPTSTR mainKey,LPTSTR key,int intValue){
    CString strValue;
    strValue.Format("%d",intValue);
    WritePrivateProfileString(mainKey,key,strValue,m_fileName);
}

void CConfig::SetParaStr(LPTSTR mainKey,LPTSTR key,CString strValue){
    WritePrivateProfileString(mainKey,key,strValue,m_fileName);
}