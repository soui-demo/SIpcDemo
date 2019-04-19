#pragma once

#include "PageHandlerBase.h"

class CSvrConnect : public IServer, public SOUI::TObjRefImpl<SOUI::IIpcConnection>
{
public:
	CSvrConnect(IIpcHandle *pHandle);
	~CSvrConnect();

public:

public:
	// 通过 TObjRefImpl 继承
	virtual IIpcHandle * GetIpcHandle() override;
	virtual void BuildShareBufferName(ULONG_PTR idLocal, ULONG_PTR idRemote, TCHAR szBuf[MAX_PATH]) const override;

	void OnAddInt(Param_AddInt & param);
	void OnAddStr(Param_AddString & param);
	void OnSum(Param_Sum & param);
	FUN_BEGIN
		FUN_HANDLER(Param_AddInt, OnAddInt)
		FUN_HANDLER(Param_AddString, OnAddStr)
		FUN_HANDLER(Param_Sum,OnSum)
	FUN_END
private:
	CAutoRefPtr<IIpcHandle> m_ipcHandle;

	// 通过 IServer 继承
	virtual void SayHello(const std::string & name) override;
	virtual std::string AddBack(const std::string & a, const std::string & b, int a2, int b2) override;
};

class CPageServer : public CPageHandlerBase, SOUI::IIpcSvrCallback
{
private:
	SRichEdit * m_pLogView;
	CAutoRefPtr<SOUI::IIpcServer> m_ipcSvr;
	SComMgr2	m_comMgr;
public:
	CPageServer();
	~CPageServer();


	virtual void OnInit(SWindow *pRoot);

	virtual void onLog(int level, const char * filter,const SStringW &buf);

	// 通过 IIpcSvrCallback 继承
	virtual void OnNewConnection(IIpcHandle * pIpcHandle, IIpcConnection ** ppConn) override;
	virtual int GetBufSize() const override;
	virtual void * GetSecurityAttr() const override;
	virtual void ReleaseSecurityAttr(void * psa) const override;
	virtual void OnConnected(IIpcConnection * pConn) override;
	virtual void OnDisconnected(IIpcConnection * pConn) override;

	void OnCopySvrId();

	static void DoSayHello(IIpcConnection *pConn,ULONG_PTR data);


	void OnHello();
	void OnClearLog();
	EVENT_MAP_BEGIN()
		EVENT_CHECK_SENDER_ROOT(GetRoot())
		EVENT_ID_COMMAND(R.id.btn_copy_id,OnCopySvrId)
		EVENT_ID_COMMAND(R.id.btn_hello,OnHello)
		EVENT_ID_COMMAND(R.id.btn_clear_log,OnClearLog)
	EVENT_MAP_BREAK()


	void OnTimer(UINT_PTR id);
	BEGIN_MSG_MAP_EX(CPageServer)
		MSG_WM_TIMER(OnTimer)
		CHAIN_MSG_MAP_2_IPC(m_ipcSvr)
	END_MSG_MAP()

};

