#pragma once
#include <string>
#include <sstream>
#include <interface/SIpcObj-i.h>
#include <helper/sipcparamhelper.hpp>

#define DEMO_SHARE_BUF_NAME_FMT _T("demo_share_buffer_8085395F-E2FA-4F96-8BD0-FE5D7412CD22_%08x_2_%08x")


//////////////////////////////////////////////////////////////////
namespace SOUI {

	template<>
	inline SParamStream & SParamStream::operator<<(const std::string & str)
	{
		int nSize = (int)str.size();
		GetBuffer()->Write(&nSize, sizeof(int));
		GetBuffer()->Write(str.c_str(), nSize);
		return *this;
	}
	template<>
	inline SParamStream & SParamStream::operator >> (std::string & str)
	{
		int nSize = 0;
		GetBuffer()->Read(&nSize, sizeof(int));
		char *pBuf = new char[nSize];
		GetBuffer()->Read(pBuf, nSize);
		str = std::string(pBuf, nSize);
		delete[]pBuf;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////
	template<>
	inline SParamStream & SParamStream::operator<<(const std::wstring & str)
	{
		int nSize = (int)str.size();
		GetBuffer()->Write(&nSize, sizeof(int));
		GetBuffer()->Write(str.c_str(), nSize * sizeof(wchar_t));
		return *this;
	}
	template<>
	inline SParamStream & SParamStream::operator >> (std::wstring & str)
	{
		int nSize = 0;
		GetBuffer()->Read(&nSize, sizeof(int));
		wchar_t *pBuf = new wchar_t[nSize];
		GetBuffer()->Read(pBuf, nSize * sizeof(wchar_t));
		str = std::wstring(pBuf, nSize);
		delete[]pBuf;
		return *this;
	}
}

struct FunParams_Base : SOUI::IFunParams
{
	virtual void ToStream4Input(SOUI::SParamStream &  ps) {}
	virtual void ToStream4Output(SOUI::SParamStream &  ps) {}
	virtual void FromStream4Input(SOUI::SParamStream &  ps) {}
	virtual void FromStream4Output(SOUI::SParamStream &  ps) {}
};


struct IClient
{
	virtual int Add(int a, int b) = 0;
	virtual std::string Add(const std::string & a, const std::string & b) = 0;
	virtual int Sum(int n) = 0;
};

struct IServer
{
	virtual void SayHello(const std::string & name) = 0;
	virtual std::string AddBack(const std::string & a, const std::string & b, int a2, int b2) = 0;
};

enum {
	CID_AddInt = SOUI::FUN_ID_START,
	CID_AddString,

	SID_Hello,
	SID_AddBack,

	BID_Sum,
};

struct Param_AddInt : FunParams_Base
{
	int a, b;
	int ret;
	FUNID(CID_AddInt)
		PARAMS2(Input, a,b)
		PARAMS1(Output,ret)
};

struct Param_AddString : FunParams_Base
{
	std::string a, b;
	std::string ret;
	FUNID(CID_AddString)
		PARAMS2(Input, a, b)
		PARAMS1(Output, ret)
};

struct Param_Hello : FunParams_Base
{
	std::string text;
	FUNID(SID_Hello)
		PARAMS1(Input, text)
};

struct Param_AddBack : FunParams_Base
{
	std::string a,b;
	int a2, b2;
	std::string ret;
	FUNID(SID_AddBack)
		PARAMS4(Input, a,b,a2,b2)
		PARAMS1(Output,ret)
};

struct Param_Sum : FunParams_Base
{
	int n;
	int nRet;
	FUNID(BID_Sum)
		PARAMS1(Input, n)
		PARAMS1(Output, nRet)
};