#pragma once
#include "PageHandlerBase.h"
#include <interface/sipcobj-i.h>

class CClientConnect : public IClient , public SOUI::TObjRefImpl<SOUI::IIpcConnection>
{
public:
	CClientConnect()
	{
		CAutoRefPtr<IIpcFactory> ipcFac;
		m_comMgr.CreateIpcObject((IObjRef**)&ipcFac);
		ipcFac->CreateIpcHandle(&m_ipcHandle);
	}
	~CClientConnect()
	{
		m_ipcHandle = NULL;
	}

public:
	// 通过 IClient 继承
	virtual int Add(int a, int b) override;
	virtual std::string Add(const std::string & a, const std::string & b) override;


public:
	// 通过 TObjRefImpl 继承
	virtual IIpcHandle * GetIpcHandle() override;
	virtual void BuildShareBufferName(ULONG_PTR idLocal, ULONG_PTR idRemote, TCHAR szBuf[MAX_PATH]) const override;

	void OnHello(Param_Hello & param);
	void OnAddBack(Param_AddBack & param);
	FUN_BEGIN
		FUN_HANDLER(Param_Hello, OnHello)
		FUN_HANDLER(Param_AddBack, OnAddBack)
	FUN_END
private:
	CAutoRefPtr<IIpcHandle> m_ipcHandle;

	SComMgr2 m_comMgr;

};

class CPageClient : public CPageHandlerBase
{
private:
	SRichEdit * m_pLogView;

	CClientConnect m_conn;
public:
	CPageClient();
	~CPageClient();

	virtual void OnInit(SWindow * pRoot);

	virtual void onLog(int level, const char * filter, const SStringW &buf);


private:
	void OnConnect();
	void OnDisconnect();
	void OnAddInt();
	void OnAddString();

	EVENT_MAP_BEGIN()
		EVENT_CHECK_SENDER_ROOT(GetRoot())
		EVENT_ID_COMMAND(R.id.btn_connect,OnConnect)
		EVENT_ID_COMMAND(R.id.btn_disconnect,OnDisconnect)
		EVENT_ID_COMMAND(R.id.btn_add_int,OnAddInt)
		EVENT_ID_COMMAND(R.id.btn_add_str,OnAddString)
	EVENT_MAP_BREAK()

	BEGIN_MSG_MAP_EX(CPageClient)
		CHAIN_MSG_MAP_2_IPC2(m_conn.GetIpcHandle())
	END_MSG_MAP()

};

