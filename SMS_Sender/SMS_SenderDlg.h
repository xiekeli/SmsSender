// SMS_SenderDlg.h : header file
//

#if !defined(AFX_SMS_SENDERDLG_H__CCB92CCF_FAD6_484C_A9BC_F86812E03A42__INCLUDED_)
#define AFX_SMS_SENDERDLG_H__CCB92CCF_FAD6_484C_A9BC_F86812E03A42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sms/SmsTraffic.h"
#include "Dao/dao.h"
#include "Led.h"
#include <Afxmt.h>
#include "liblogger/liblogger.h"
#include "liblogger/file_logger.h"

#include "liblogger/liblogger_levels.h"
// For this file, we choose logs of priority debug and above.
//#define LOG_LEVEL LOG_LEVEL_DEBUG
// The module name for logs done from this file.
//#define LOG_MODULE_NAME "SMS_SenderDlg"



/////////////////////////////////////////////////////////////////////////////
// CSMS_SenderDlg dialog


typedef struct{
    BOOL available;
    CSmsTraffic* pSmsTraffic;
}SMS_Sender,*PSMS_Sender;


const int IDLE_SMS_NUMBER = 5;//空闲短信机阀值，待发短信数小于该值，为空闲

class CSMS_SenderDlg : public CDialog
{
// Construction
public:
	CSMS_SenderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSMS_SenderDlg)
	enum { IDD = IDD_SMS_SENDER_DIALOG };
	CListBox	m_ListBox_Console;
	CString	m_SID;
	CString	m_User;
	CString	m_Password;
	CLed	m_Led8;
	CLed	m_Led7;
    CLed	m_Led6;
	CLed	m_Led5;
	CLed	m_Led4;
    CLed	m_Led3;
	CLed	m_Led2;
	CLed	m_Led1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMS_SenderDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSMS_SenderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnInfo(WPARAM, LPARAM);  //日志信息显示              
	afx_msg void OnClose();
	//}}AFX_MSG
private:
    void Initialize();
    void Finalize();
    void InitComPool();
    void InitLog();
    void AddInfo(CString Info);
    void setLed(int i,BOOL success);
    int getIdleSender();
private:
    TCHAR   szCause[255];
    SMS_Sender m_SmsSenderList[COMPOOL_SIZE];
    SMS_Sender m_idleSmsSenderList[COMPOOL_SIZE];//空闲的短信机数组
    int   max_width; 
    UINT m_timer;//定时器，定时获取待发短信
    CString m_Msg;
    CCriticalSection cs;
    CCriticalSection cs_ms;
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMS_SENDERDLG_H__CCB92CCF_FAD6_484C_A9BC_F86812E03A42__INCLUDED_)
