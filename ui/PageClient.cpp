#include "stdafx.h"
#include "PageClient.h"

#define LOG_FILTER "client"



CClientConnect::CClientConnect()
{
	CAutoRefPtr<IIpcFactory> ipcFac;
	m_comMgr.CreateIpcObject((IObjRef**)&ipcFac);
	ipcFac->CreateIpcHandle(&m_ipcHandle);
	m_ipcHandle->SetIpcConnection(this);
}


CClientConnect::~CClientConnect()
{
	m_ipcHandle = NULL;
}


int CClientConnect::Add(int a, int b)
{
	Param_AddInt params;
	params.a = a;
	params.b = b;
	m_ipcHandle->CallFun(&params);
	SLOG_INFO("call add int, a:" << a << " b:" << b<<" return="<<params.ret);
	return params.ret;
}

std::string CClientConnect::Add(const std::string & a, const std::string & b)
{
	Param_AddString params;
	params.a = a;
	params.b = b;
	m_ipcHandle->CallFun(&params);
	SLOG_INFO("call add string, a:" << a.c_str() << " b:" << b.c_str()<<" return="<<params.ret.c_str());
	return params.ret;
}

IIpcHandle * CClientConnect::GetIpcHandle()
{
	return m_ipcHandle;
}

void CClientConnect::BuildShareBufferName(ULONG_PTR idLocal, ULONG_PTR idRemote, TCHAR szBuf[MAX_PATH]) const
{
	_stprintf(szBuf, DEMO_SHARE_BUF_NAME_FMT, (DWORD)(((LPARAM)idLocal) & 0xffffffff), (DWORD)(((LPARAM)idRemote) & 0xffffffff));
}

void CClientConnect::OnHello(Param_Hello & param)
{
	SLOG_INFO("server call hello: " << param.text.c_str());
}

void CClientConnect::OnAddBack(Param_AddBack & param)
{
	SLOG_INFO("server call add back params: a=" << param.a.c_str()<<" b="<<param.b.c_str()<<" a2="<<param.a2<<" b2="<<param.b2);
	std::stringstream ss;
	ss << param.a << "_" << param.b << ":" << (param.a2 * 100 + param.b2);
	param.ret = ss.str();
	SLOG_INFO("server call add back: ret " << param.ret.c_str());
}

////////////////////////////////////////////////////////////////////////////////
CPageClient::CPageClient()
{
}


CPageClient::~CPageClient()
{
	m_conn.GetIpcHandle()->Disconnect();
}

void CPageClient::OnInit(SWindow * pRoot)
{
	__super::OnInit(pRoot);
	m_pLogView = FindChildByID2<SRichEdit>(R.id.log_client);
}


void CPageClient::onLog(int level, const char * filter, const SStringW &buf)
{
	if (strcmp(filter, LOG_FILTER) != 0)
		return;
	m_pLogView->ReplaceSel(buf, FALSE);
	m_pLogView->SSendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0));
	int nLines = m_pLogView->SSendMessage(EM_GETLINECOUNT);
	if (nLines>60000)
	{
		int iLineIdx = m_pLogView->SSendMessage(EM_LINEINDEX, 30000);
		m_pLogView->SSendMessage(EM_SETSEL, 0, iLineIdx);
		m_pLogView->ReplaceSel(L"", FALSE);
	}
}

void CPageClient::OnConnect()
{
	SStringT strId = FindChildByID(R.id.edit_svr_id)->GetWindowText();
	if (strId.IsEmpty())
	{
		SLOG_WARN("specified server id at first.");
		return;
	}
	int id = 0;
	::StrToIntEx(strId, STIF_SUPPORT_HEX,&id);
	if (id == 0)
	{
		SLOG_WARN("invalid server id");
		return;
	}
	HWND hLocal = GetRoot()->GetContainer()->GetHostHwnd();
	HRESULT hr = m_conn.GetIpcHandle()->ConnectTo((ULONG_PTR)hLocal, id);
	SLOG_INFO("connect to server " << id << " return "<<hr);
}

void CPageClient::OnDisconnect()
{
	HRESULT hr = m_conn.GetIpcHandle()->Disconnect();
	SLOG_INFO("disconnect return: " << hr);
}

void CPageClient::OnAddInt()
{
	int a = _ttoi(FindChildByID(R.id.edit_int_a)->GetWindowText());
	int b = _ttoi(FindChildByID(R.id.edit_int_b)->GetWindowText());
	m_conn.Add(a, b);
}

void CPageClient::OnAddString()
{
	SStringT a = FindChildByID(R.id.edit_str_a)->GetWindowText();
	SStringT b = FindChildByID(R.id.edit_str_b)->GetWindowText();

	SStringA strA = S_CT2A(a);
	SStringA strB = S_CT2A(b);
	m_conn.Add((LPCSTR)strA,(LPCSTR)strB);
}
