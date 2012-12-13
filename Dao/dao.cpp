
#include "../stdafx.h"
#include "../SMS_Sender.h"
#include "dao.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Afxmt.h"

const CString DB = "pss";
const CString USERNAME = "pss";
const CString PASSWORD = "pss";

BOOL OCI_Initialized = false;

CCriticalSection cs;


void err_handler(OCI_Error *err)
{
    printf(
                "code  : ORA-%05i\n"
                "msg   : %s\n"
                "sql   : %s\n",
                OCI_ErrorGetOCICode(err), 
                OCI_ErrorGetString(err),
                OCI_GetSql(OCI_ErrorGetStatement(err))
           );
}

PSendSMS* getSendSms(int &smsNumber)
{
    OCI_Connection *cn;
    OCI_Statement  *st;
    OCI_Resultset  *rs;

    cs.Lock();
    int index = 0;
    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT | OCI_ENV_CONTEXT))
        return NULL;
    OCI_Initialized = true;
    cn  = OCI_ConnectionCreate(DB, USERNAME, PASSWORD, OCI_SESSION_DEFAULT);
    if (cn == NULL)
    {
        OCI_Error *err = OCI_GetLastError();
        printf("errcode %d, errmsg %s", OCI_ErrorGetOCICode(err), 
                                        OCI_ErrorGetString(err));
        return NULL;
    }

    st  = OCI_StatementCreate(cn);
    OCI_SetFetchMode(st, OCI_SFM_SCROLLABLE); //设置为双向模式，用于获取记录数
    OCI_ExecuteStmt(st, "select ID,TELECODE,CODEC,MSG from A_SMS_SEND where STATUS = 0");

    rs = OCI_GetResultset(st);
    OCI_FetchLast(rs); 
    int recordNumber = OCI_GetRowCount(rs);
    smsNumber = recordNumber;
    if(recordNumber == 0)
    {
        OCI_ConnectionFree(cn);
        cs.Unlock();
        return NULL;
    }
    PSendSMS* sendSMSList = (PSendSMS*)malloc(recordNumber * sizeof(PSendSMS));

    OCI_FetchFirst(rs); 
    PSendSMS pSms = new SendSMS();
    pSms->ID = OCI_GetInt(rs, 1);
    pSms->TeleCode = OCI_GetString(rs, 2);
    pSms->CodeC = (CODEC)OCI_GetInt(rs, 3);
    pSms->Msg = OCI_GetString(rs, 4);
    sendSMSList[index++] = pSms;
    setStatus(pSms->ID,ssSending,false,cn);
    while (OCI_FetchNext(rs))
    {      
        pSms = new SendSMS();
        pSms->ID = OCI_GetInt(rs, 1);
        pSms->TeleCode = OCI_GetString(rs, 2);
        pSms->CodeC = (CODEC)OCI_GetInt(rs, 3);
        pSms->Msg = OCI_GetString(rs, 4);
        sendSMSList[index++] = pSms;
        setStatus(pSms->ID,ssSending,false,cn);
    }
    OCI_ConnectionFree(cn);
    if(OCI_Initialized)
        OCI_Cleanup();  
    cs.Unlock();
    return sendSMSList;
}



BOOL setStatus(int id,SendStatus status,BOOL needClean,OCI_Connection *cn)
{
    cs.Lock();
    BOOL needFree = FALSE;
    OCI_Statement *st;

    int smsId;
    int sms_status;

    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
    {
        cs.Unlock();
        return false;
    }
    if(cn == NULL)
    {
        cn  = OCI_ConnectionCreate(DB, USERNAME, PASSWORD, OCI_SESSION_DEFAULT);
        needFree = true;
    }
    st = OCI_StatementCreate(cn);
  
    OCI_Prepare(st, "update A_SMS_SEND set STATUS = :status where ID = :id");
    OCI_BindInt(st, ":status", &sms_status);
    OCI_BindInt(st, ":id", &smsId);
    
    smsId = id;
    sms_status = status;
    OCI_Execute(st);
   
    OCI_Commit(cn);
    if(needFree)
        OCI_ConnectionFree(cn);
    if(needClean)   
      OCI_Cleanup();  
    cs.Unlock();
    return true;
}

BOOL insertRecvSms(SM_PARAM* pparam)
{
    cs.Lock();
    OCI_Connection *cn;
    OCI_Statement *st;

    int smsId;
    int sms_dcs;

    if (!OCI_Initialize(err_handler, NULL, OCI_ENV_DEFAULT))
    {
        cs.Unlock();
        return false;
    }
    cn  = OCI_ConnectionCreate(DB, USERNAME, PASSWORD, OCI_SESSION_DEFAULT);
 
    st = OCI_StatementCreate(cn);
  
    OCI_Prepare(st, "insert into A_SMS_RECEIVE(ID,TELECODE,CODEC,MSG) values(:id,:telecode,:codec,:msg)");
    OCI_BindInt(st, ":id", &smsId);
    OCI_BindString(st, ":telecode", pparam->TPA, 16); 
    OCI_BindInt(st, ":codec", &sms_dcs);
    OCI_BindString(st, ":msg", pparam->TP_UD,160);
    
    smsId = pparam->index;
    sms_dcs = pparam->TP_DCS;
    OCI_Execute(st);
   
    OCI_Commit(cn);
    OCI_ConnectionFree(cn);
    OCI_Cleanup();  
    cs.Unlock();
    return true;   
}