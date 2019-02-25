// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "PageClient.h"
#include "PageServer.h"

class CMainDlg : public SHostWnd
	, public ILog4zManager::IOutputListener
	, public TAutoEventMapReg<CMainDlg>
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	// 通过 IOutputListener 继承
	virtual void onOutputLog(int level, const char * filter, const char * log, int nLogLen, unsigned __int64 time_) override;
protected:
	void OnLog(EventArgs *e);
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_HANDLER(EventOutputLog::EventID,OnLog)
		CHAIN_EVENT_MAP_MEMBER(m_pageSvr)
		CHAIN_EVENT_MAP_MEMBER(m_pageClient)
	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP_MEMBER(m_pageSvr)
		CHAIN_MSG_MAP_MEMBER(m_pageClient)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:

	CPageServer		m_pageSvr;
	CPageClient		m_pageClient;
};
