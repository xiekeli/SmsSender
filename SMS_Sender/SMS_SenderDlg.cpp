// SMS_SenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_Sender.h"
#include "SMS_SenderDlg.h"
#include "Comm/Comm.h"
#include "Utility/ConvertUtility.h"
#include "DlgExitValidate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_SenderDlg dialog

CSMS_SenderDlg::CSMS_SenderDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMS_SenderDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSMS_SenderDlg)
    m_SID = _T("");
    m_User = _T("");
    m_Password = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDI_SMS);
}

void CSMS_SenderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSMS_SenderDlg)
    DDX_Control(pDX, IDC_LIST_CONSOLE, m_ListBox_Console);
    DDX_Control(pDX, IDC_LED_COM1, m_Led1);
    DDX_Control(pDX, IDC_LED_COM2, m_Led2);
    DDX_Control(pDX, IDC_LED_COM3, m_Led3);
    DDX_Control(pDX, IDC_LED_COM4, m_Led4);
    DDX_Control(pDX, IDC_LED_COM5, m_Led5);
    DDX_Control(pDX, IDC_LED_COM6, m_Led6);
    DDX_Control(pDX, IDC_LED_COM7, m_Led7);
    DDX_Control(pDX, IDC_LED_COM8, m_Led8);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_SenderDlg, CDialog)
//{{AFX_MSG_MAP(CSMS_SenderDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_DESTROY()
ON_WM_TIMER()
ON_MESSAGE(WM_INFO_MESSAGE, OnInfo) 
ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_SenderDlg message handlers

BOOL CSMS_SenderDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    CMenu *pmenu   =   AfxGetMainWnd()-> GetSystemMenu(FALSE); 
    
    if(pmenu) 
    {       
        pmenu-> EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_GRAYED); 
    }
    
    // Add "About..." menu item to system menu.
    
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    
    // TODO: Add extra initialization here
    
    Initialize();
    SetTimer(2,1000,NULL);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSMS_SenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSMS_SenderDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSMS_SenderDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CSMS_SenderDlg::Initialize()
{
    for(int i=0;i<COMPOOL_SIZE;i++)
    {
        m_SmsSenderList[i].pSmsTraffic = NULL;
        m_SmsSenderList[i].available = false;
    }
    InitLog();
    InitComPool();
    
    m_timer = this->SetTimer(1,10000,NULL);
    
}


void CSMS_SenderDlg::Finalize()
{
    if(m_SmsSenderList!= NULL)
    {
        for(int i=0;i<COMPOOL_SIZE;i++)
        {
            if(m_SmsSenderList[i].pSmsTraffic != NULL)
                delete m_SmsSenderList[i].pSmsTraffic;
        }
    } 
    DeInitLogger();
}

void CSMS_SenderDlg::setLed(int i,BOOL success)
{
    if((i>8)||(i<0))
        return;
    if(success)
    {
        switch(i)
        {
        case 1:{m_Led1.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 2:{m_Led2.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 3:{m_Led3.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 4:{m_Led4.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 5:{m_Led5.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 6:{m_Led6.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 7:{m_Led7.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        case 8:{m_Led8.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_ON,CLed::LED_ROUND);break;}
        }
    }
    else
    {
        switch(i)
        {
        case 1:{m_Led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 2:{m_Led2.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 3:{m_Led3.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 4:{m_Led4.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 5:{m_Led5.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 6:{m_Led6.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 7:{m_Led7.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        case 8:{m_Led8.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);break;}
        }
    }
    
}

void CSMS_SenderDlg::InitComPool()
{
    try
    {
        
        AddInfo("开始进行串口检测....");
        InitPool();
        for(int i=1;i<=COMPOOL_SIZE;i++)
        {
            char comPort[6];
            sprintf(comPort,"COM%d",i);
            CString port;
            port.Format(_T("%s"), comPort);
            if(OpenComm(comPort))
            {
                AddInfo("检测"+port+"(打开串口)成功");
                if(gsmInit(comPort))
                {
                    SMS_Sender sender;
                    sender.available = true;
                    sender.pSmsTraffic = new CSmsTraffic();
                    sender.pSmsTraffic->set_Comport(comPort);
                    sender.pSmsTraffic->set_Handle(this->m_hWnd);
                    
                    m_SmsSenderList[i] = sender;
                    /*char tt[8];
                    sprintf(tt,"%s",m_SmsSenderList[i].pSmsTraffic);
                    AfxMessageBox(tt);*/
                    
                    setLed(i,true);
                    AddInfo("检测"+port+"(AT命令)成功");
                    
                }
                else{
                    setLed(i,false);
                    AddInfo("检测"+port+"(AT命令)失败");
                }
            }
            else
            {
                AddInfo("检测"+port+"(打开串口)失败");
            }
        }
    }
    catch (CException* e)
    {
        e->GetErrorMessage(szCause,255);
        LogFatal("文件：%s , 第d%行，发生异常信息：s%",__FILE__,__LINE__,szCause);
    }
}

void CSMS_SenderDlg::InitLog()
{
    tFileLoggerInitParams fileInitParams;
    // very important, memset to prevent breaks when new members are
    // added to tFileLoggerInitParams.
    memset(&fileInitParams,0,sizeof(tFileLoggerInitParams));
    fileInitParams.fileOpenMode = AppendMode;
    fileInitParams.fileName = "SMS_Sender.log";
    InitLogger(LogToFile,&fileInitParams);
}

void CSMS_SenderDlg::AddInfo(CString Info){
    cs.Lock();
    CPaintDC   dc(this);  
    CSize   sz; 
    
    m_ListBox_Console.AddString(ConvertUtility::currentTime()+":"+Info); 
    sz=dc.GetTextExtent(Info); 
    if(max_width <sz.cx) 
        max_width=sz.cx; 
    m_ListBox_Console.SendMessage(LB_SETHORIZONTALEXTENT,max_width,0);
     Invalidate();
    cs.Unlock();
}



BOOL CSMS_SenderDlg::DestroyWindow() 
{
    // TODO: Add your specialized code here and/or call the base class
    
    return CDialog::DestroyWindow();
}

void CSMS_SenderDlg::OnDestroy() 
{
    CDialog::OnDestroy();
    
    Finalize();
    
}

void CSMS_SenderDlg::OnTimer(UINT nIDEvent) 
{
    try
    {
        switch (nIDEvent)
        {
        case 1:
            {
                if(m_timer!=nIDEvent)
                    return;
                int smsNumber = 0;
                
                int idleSenderNumber= getIdleSender();//检查空闲短信机，如果没有则后续短信暂不发送
                if (idleSenderNumber==0)
                    return;
                
                PSendSMS* smslist = getSendSms(smsNumber);
                if((smsNumber == 0)||(smslist == NULL))
                    return ;
                
                m_Msg.Format("检测到%d条待发短信",smsNumber);
                AddInfo(m_Msg);
                CString Smsc = "13800571500";
                CString teleCode;
                
                CSmsTraffic* pSender;
                int index=0;
                /*char tt[16];
                sprintf(tt,"%s",pSender);
                AfxMessageBox(tt);*/
                for(int i=0;i<smsNumber;i++)
                {
                              
                    if(index>=idleSenderNumber)
                        index = 0;
                    pSender= m_idleSmsSenderList[index].pSmsTraffic;
                    SM_PARAM SmParam;
                    memset(&SmParam, 0, sizeof(SM_PARAM));
                    
                    // 去掉号码前的"+"
                    if(Smsc[0] == '+')  Smsc = Smsc.Mid(1);
                    teleCode = smslist[i]->TeleCode;
                    if(teleCode[0] == '+')  teleCode = teleCode.Mid(1);
                    
                    // 在号码前加"86"
                    if(Smsc.Left(2) != "86")  Smsc = "86" + Smsc;
                    if(teleCode.Left(2) != "86")  teleCode = "86" + teleCode;
                    
                    // 填充短消息结构
                    strcpy(SmParam.SCA,Smsc);
                    strcpy(SmParam.TPA, teleCode);
                    strcpy(SmParam.TP_UD, smslist[i]->Msg);
                    CString smsID;
                    SmParam.index = smslist[i]->ID;
                    SmParam.TP_PID = 0;
                    SmParam.TP_DCS = GSM_UCS2;
                    pSender->PutSendMessage(&SmParam);
                    index++;  
                    
                }
                for(int j=0;j<smsNumber;j++)
                    delete smslist[j];
                free(smslist);
                smslist = NULL;
                break;
            }
        case 2:{
            m_Led1.Ping(200);
            m_Led2.Ping(200);
            m_Led3.Ping(200);
            m_Led4.Ping(200);
            m_Led5.Ping(200);
            m_Led6.Ping(200);
            m_Led7.Ping(200);
            m_Led8.Ping(200);
            break;
               }
        }
        
        
        CDialog::OnTimer(nIDEvent);
    }
    catch (CException* e)
    {
        e->GetErrorMessage(szCause,255);
        LogFatal("文件：%s , 第d%行，发生异常信息：s%",__FILE__,__LINE__,szCause);
    }
}

int CSMS_SenderDlg::getIdleSender()
{
    int index =0;
    for(int i=0;i<COMPOOL_SIZE;i++)
    {
        m_idleSmsSenderList[i].pSmsTraffic = NULL;
        m_idleSmsSenderList[i].available = false;
    }
    
    for(i=0;i<COMPOOL_SIZE;i++)
    {
        if(m_SmsSenderList[i].available)
        {
            int WaitingNumber = m_SmsSenderList[i].pSmsTraffic->getUnSends();
            if(WaitingNumber < IDLE_SMS_NUMBER)
            {
                m_idleSmsSenderList[index].pSmsTraffic = m_SmsSenderList[i].pSmsTraffic;
                m_idleSmsSenderList[index].available = true;
                index++;
            }
            
        }
    }
    return   index;
}

//消息方法
LRESULT CSMS_SenderDlg::OnInfo(WPARAM wParam, LPARAM lParam)
{
    cs_ms.Lock();
    char szPack[128];   
    memcpy(szPack,(char*)wParam,128);
    CString Msg(szPack); 
    AddInfo(Msg);
    free((char*)wParam);
    cs_ms.Unlock();
    return 0;
    
}

void CSMS_SenderDlg::OnClose() 
{
    // TODO: Add your message handler code here and/or call default
    CDlgExitValidate oDlg;
    if(oDlg.DoModal()==IDOK )
        CDialog::OnClose();
}
