// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
	
#include "PageClient.h"
#include "PageServer.h"


CMainDlg::CMainDlg(bool isServer) : SHostWnd(_T("LAYOUT:XML_MAINWND")),m_isServer(isServer),m_pPageHandle(NULL)
{
	SNotifyCenter::getSingletonPtr()->addEvent(EVENTID(EventOutputLog));
}

CMainDlg::~CMainDlg()
{
}


BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	STabCtrl *pTabMode = FindChildByID2<STabCtrl>(R.id.tab_mode);
	if(m_isServer)
	{
		pTabMode->SetCurSel(1);
		m_pPageHandle = new CPageServer;
		m_pPageHandle->OnInit(FindChildByID(R.id.page_server));
		SStringT strTitle = SStringT().Format(_T("%s [server]"),GETSTRING(R.string.title));
		FindChildByID(R.id.txt_title)->SetWindowText(strTitle);
		::SetWindowText(m_hWnd,strTitle);
	}else
	{
		pTabMode->SetCurSel(0);
		m_pPageHandle = new CPageClient;
		m_pPageHandle->OnInit(FindChildByID(R.id.page_client));

		SStringT strTitle = SStringT().Format(_T("%s [client,%d]"),GETSTRING(R.string.title),(int)m_hWnd);
		FindChildByID(R.id.txt_title)->SetWindowText(strTitle);
		::SetWindowText(m_hWnd,strTitle);
	}
	return 0;
}

//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}


const char * KLogLevel[] = {
	"trace",
	"debug",
	"info",
	"warn",
	"error",
	"alarm",
	"fatal",
};

struct tm timeToTm(time_t t)
{
#if _MSC_VER < 1400 //VS2003
	return *localtime(&t);
#else //vs2005->vs2013->
	struct tm tt = { 0 };
	localtime_s(&tt, &t);
	return tt;
#endif
}
void CMainDlg::onOutputLog(int level, const char * filter, const char * log, int nLogLen, unsigned __int64 time_)
{
	EventOutputLog *evt = new EventOutputLog(this);
	evt->level = level;
	evt->filter = filter;
	evt->time_ = time_;
	evt->log = std::string(log, nLogLen);
	SNotifyCenter::getSingletonPtr()->FireEventAsync(evt);
	evt->Release();

}

void CMainDlg::OnLog(EventArgs * e)
{
	EventOutputLog *e2 = sobj_cast<EventOutputLog>(e);

	tm tt = timeToTm(e2->time_ / 1000);
	SStringA strBuf = SStringA().Format("%d-%02d-%02d %02d:%02d:%02d.%03d [%s],%s\n",
		tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, (int)(e2->time_ % 1000),
		KLogLevel[e2->level], e2->log.c_str());
	SStringW wBuf = S_CA2W(strBuf);
	m_pPageHandle->onLog(e2->level, e2->filter.c_str(), wBuf);
}

void CMainDlg::OnDestroy()
{
	delete m_pPageHandle;
	m_pPageHandle = NULL;
	SetMsgHandled(FALSE);
}
