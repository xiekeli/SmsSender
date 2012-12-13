#if !defined(AFX_DLGEXITVALIDATE_H__00CD3BC8_1A79_455A_9637_899369B4221D__INCLUDED_)
#define AFX_DLGEXITVALIDATE_H__00CD3BC8_1A79_455A_9637_899369B4221D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExitValidate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExitValidate dialog

class CDlgExitValidate : public CDialog
{
// Construction
public:
	CDlgExitValidate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExitValidate)
	enum { IDD = IDD_EXIT_PASSWORD };
	CString	m_Psw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExitValidate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExitValidate)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXITVALIDATE_H__00CD3BC8_1A79_455A_9637_899369B4221D__INCLUDED_)
