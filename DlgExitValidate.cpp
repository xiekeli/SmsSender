// DlgExitValidate.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_Sender.h"
#include "DlgExitValidate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExitValidate dialog


CDlgExitValidate::CDlgExitValidate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExitValidate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExitValidate)
	m_Psw = _T("");
	//}}AFX_DATA_INIT
}


void CDlgExitValidate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExitValidate)
	DDX_Text(pDX, IDC_EDT_PSW, m_Psw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExitValidate, CDialog)
	//{{AFX_MSG_MAP(CDlgExitValidate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExitValidate message handlers

void CDlgExitValidate::OnOK() 
{
	// TODO: Add extra validation here
    UpdateData();
	if(m_Psw == "admin@123")
	    CDialog::OnOK();
    else
        CDialog::OnCancel();
}
