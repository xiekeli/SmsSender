// SMS_Sender.h : main header file for the SMS_SENDER application
//

#if !defined(AFX_SMS_SENDER_H__77993A09_1062_40CD_845E_792A9C32F293__INCLUDED_)
#define AFX_SMS_SENDER_H__77993A09_1062_40CD_845E_792A9C32F293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSMS_SenderApp:
// See SMS_Sender.cpp for the implementation of this class
//

class CSMS_SenderApp : public CWinApp
{
public:
	CSMS_SenderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMS_SenderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSMS_SenderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMS_SENDER_H__77993A09_1062_40CD_845E_792A9C32F293__INCLUDED_)
