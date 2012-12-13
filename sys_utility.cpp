// sys_utility.cpp: implementation of the SysUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sys_utility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysUtility::SysUtility()
{

}

SysUtility::~SysUtility()
{

}

HMODULE SysUtility::GetCurrentModule() 
{ 
#if   _MSC_VER   <   1300         //   earlier   than   .NET   compiler   (VC   6.0) 

    //   Here 's   a   trick   that   will   get   you   the   handle   of   the   module 
    //   you 're   running   in   without   any   a-priori   knowledge: 
    //   http://www.dotnet247.com/247reference/msgs/13/65259.aspx 

    MEMORY_BASIC_INFORMATION   mbi; 
    static   int   dummy; 
    VirtualQuery(   &dummy,   &mbi,   sizeof(mbi)   ); 

    return   reinterpret_cast <HMODULE> (mbi.AllocationBase); 

#else         //   VC   7.0 

    //   from   ATL   7.0   sources 

    return   reinterpret_cast <HMODULE> (&__ImageBase); 
#endif 
}



CString SysUtility::getBasePath(){
    char *p = NULL;
    TCHAR FilePath[MAX_PATH+1];
    GetModuleFileName(NULL,FilePath,MAX_PATH);

    p=strrchr(FilePath, '\\');   
    *p='\0';
    
    return FilePath;
}

CString SysUtility::getTempPath()
{
    CString tempPath=getBasePath()+_TEXT("Temp\\");
    CFileFind m_FileFind;
    if(!m_FileFind.FindFile(tempPath)) //路径不存在则创建该路径
        CreateDirectory(tempPath,NULL);
    return tempPath;
}

CString SysUtility::getImagePath()
{
    CString tempPath=getBasePath()+_TEXT("Images\\");
    CFileFind m_FileFind;
    if(!m_FileFind.FindFile(tempPath)) //路径不存在则创建该路径
        CreateDirectory(tempPath,NULL);
    return tempPath;
}

CString SysUtility::getTempBmp()
{
    return getTempPath()+"temp.bmp";
}

CString SysUtility::getTempJpg()
{
    return getTempPath()+"temp.jpg";
}
