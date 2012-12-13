// ConvertUtility.cpp: implementation of the ConvertUtility class.
//
//////////////////////////////////////////////////////////////////////

#include   "../stdafx.h"
#include "ConvertUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConvertUtility::ConvertUtility()
{
    
}

ConvertUtility::~ConvertUtility()
{
    
}

int ConvertUtility::HexStr2Int(CString str,int length){
    char   revstr[10]; 
    int   num[10]; 
    int   count=1; 
    int   result=0; 
    strcpy(revstr,str); 
    for(int i=length-1;i>=0;i--) 
    { 
        if   ((revstr[i]>= '0')   &&   (revstr[i] <= '9')) 
            num[i]=revstr[i]-48; 
        else   if   ((revstr[i]>= 'a')&&   (revstr[i] <= 'f')) 
            num[i]=revstr[i]- 'a '+10; 
        else   if   ((revstr[i]>= 'A')&&(revstr[i] <= 'F')) 
            num[i]=revstr[i]- 'A'+10; 
        else 
            num[i]=0; 
        result=result+num[i]*count; 
        count=count*16;	       
    } 
    return   result; 
}

CString ConvertUtility::Buffer2HexStr(PBYTE buffer,DWORD bufferSize,CString sign){
    CString resultStr;
    CString strValue;
    PBYTE bufferTemp = buffer;
    for(byte i=0;i<=bufferSize-1;i++){
        strValue.Format("%02X",*bufferTemp);
        if(i!=bufferSize-1)
            resultStr += strValue+sign;
        else
            resultStr += strValue;
        bufferTemp++;
    }
    return resultStr;
}


CString ConvertUtility::currentTime(){
    SYSTEMTIME rTime;
    GetLocalTime(&rTime);
    return GetDateTimeString(rTime);
}


CString ConvertUtility::GetDateString(SYSTEMTIME &rDateTime){
    CString strMonth,strDate,strYear;//:string;
    CString strReturn;//:string;
    strYear.Format(_TEXT("%d"),rDateTime.wYear);//  strYear:=intToStr(YearOf(recDate));
    strMonth.Format(_TEXT("%d"),rDateTime.wMonth);// strMonth:=intToStr(MonthOf(recDate));
    if(strMonth.GetLength()==1)
        strMonth=_TEXT("0")+strMonth;// if Length(strMonth)=1 then strMonth:='0'+strMonth;
    strDate.Format(_TEXT("%d"),rDateTime.wDay);// strMonth:=intToStr(MonthOf(recDate));
    if(strDate.GetLength()==1)
        strDate=_TEXT("0")+strDate;// if Length(strMonth)=1 then strMonth:='0'+strMonth;
    strReturn=strYear+strMonth+strDate;
    return(strReturn);
}

CString ConvertUtility::GetDateTimeString(SYSTEMTIME &rDateTime){
    CString strMonth,strDay,strYear,strHour,strMinute,strSecond;//:string;
    CString strReturn;//:string;
    strYear.Format(_TEXT("%d"),rDateTime.wYear);//  strYear:=intToStr(YearOf(recDate));
    
    strMonth.Format(_TEXT("%d"),rDateTime.wMonth);// strMonth:=intToStr(MonthOf(recDate));
    if(strMonth.GetLength()==1)
        strMonth=_TEXT("0")+strMonth;// if Length(strMonth)=1 then strMonth:='0'+strMonth;
    
    strDay.Format(_TEXT("%d"),rDateTime.wDay);// strMonth:=intToStr(MonthOf(recDate));
    if(strDay.GetLength()==1)
        strDay=_TEXT("0")+strDay;// if Length(strMonth)=1 then strMonth:='0'+strMonth;
    
    strHour.Format(_TEXT("%d"),rDateTime.wHour);
    if(strHour.GetLength()==1)
        strHour=_TEXT("0")+strHour;
    
    strMinute.Format(_TEXT("%d"),rDateTime.wMinute);
    if(strMinute.GetLength()==1)
        strMinute=_TEXT("0")+strMinute;
    
    strSecond.Format(_TEXT("%d"),rDateTime.wSecond);
    if(strSecond.GetLength()==1)
        strSecond=_TEXT("0")+strSecond;
    
    
    strReturn=strYear+"-"+strMonth+"-"+strDay+" "+strHour+":"+strMinute+":"+strSecond;
    return(strReturn);
}