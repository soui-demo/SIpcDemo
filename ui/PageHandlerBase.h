#pragma once


class CPageHandlerBase
{
public:
	CPageHandlerBase();
	virtual ~CPageHandlerBase();

	virtual void OnInit(SWindow *pRoot)
	{
		m_pPageRoot = pRoot;
	}

	virtual void OnUninit() {
		m_pPageRoot = NULL;
	}

	SWindow * GetRoot() {
		return m_pPageRoot;
	}


	SWindow* FindChildByID(int nID, int nDeep = -1) {
		return GetRoot()->FindChildByID(nID, nDeep);
	}


	template<class T>
	T* FindChildByID2(int nID, int nDeep = -1)
	{
		return GetRoot()->FindChildByID2<T>(nID, nDeep);
	}

	SWindow* FindChildByName(LPCWSTR pszName, int nDeep = -1)
	{
		return GetRoot()->FindChildByName(pszName, nDeep);
	}

	SWindow* FindChildByName(LPCSTR pszName, int nDeep = -1)
	{
		return FindChildByName(S_CA2W(pszName), nDeep);
	}

	template<class T>
	T* FindChildByName2(LPCWSTR pszName, int nDeep = -1)
	{
		return GetRoot()->FindChildByName2<T>(pszName, nDeep);
	}

	template<class T>
	T* FindChildByName2(LPCSTR pszName, int nDeep = -1)
	{
		return FindChildByName2<T>(S_CA2W(pszName), nDeep);
	}

	virtual void onLog(int level, const char * filter, const SStringT &buf) = 0;

	virtual BOOL _HandleEvent(SOUI::EventArgs *pEvt) = 0;

	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & lResult, DWORD dwMsgMapID = 0) = 0;
protected:
	SWindow * m_pPageRoot;
};

