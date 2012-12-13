//
//
//  @ TODO : 数据转换工具类
//  @ File Name : ConvertUtility.h
//  @ Date : 2011/9/1
//  @ Author : lijun
//
//
#include "../StdAfx.h"

#if !defined(_UTILITY_CONVERTUTILITY_H_)
#define _UTILITY_CONVERTUTILITY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ConvertUtility  
{
public:
	ConvertUtility();
	virtual ~ConvertUtility();

    static CString Buffer2HexStr(PBYTE buffer,DWORD bufferSize,CString sign);
    static CString currentTime();
    static CString GetDateString(SYSTEMTIME &rDateTime);
    static CString GetDateTimeString(SYSTEMTIME &rDateTime);
    static int HexStr2Int(CString str,int length);

};

#endif 
