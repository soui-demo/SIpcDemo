SIpcDemo is a ipc demo using SIpcObject. this demo depends on soui. Soui is a direct UI library and can be found from https://github.com/soui2/soui.git.

SIpcObject is a IPC component. Related headers were placed in soui.src/soui/include/interface, where soui.src is the folder you had cloned soui repository. Actually, SIpcObject does not depend on SOUI, and you can extract it from soui without any trouble.

After you had finished constructing the IPC framework, using IpcObject, making a IPC call will be as simple as calling a normal function.

To make a IPC call, we need deine a struct which delieved from SOUI::IFunParams at first as demo.


struct FunParams_Base : SOUI::IFunParams
{
	virtual void ToStream4Input(SOUI::SParamStream &  ps) {}
	virtual void ToStream4Output(SOUI::SParamStream &  ps) {}
	virtual void FromStream4Input(SOUI::SParamStream &  ps) {}
	virtual void FromStream4Output(SOUI::SParamStream &  ps) {}
};

struct Param_AddInt : FunParams_Base
{
	int a, b;
	int ret;
	FUNID(CID_AddInt)
		PARAMS2(Input, a,b)
		PARAMS1(Output,ret)
};

Here, FunParams_Base is used to provide a empty implementation for all of interfaces of IFunParams, and Param_AddInt is the actual IPC call parameters object.

We defined a serial of macro to simplify implementing interfaces of IFunParams. FUNID is used to define the IPC call id.
PARAMx is used to implement serailize of parameters, where x can be from 1 to 5, and you can extend it to any number as you need. The first parameter must be "input" or "output". "input" means the parameter is input parameter and "output" means the parameter is used to save returns. A parameter can be both "input" and "output".

OK, you can call the IPC function from client by using IIpcHandle->CallFun(&Param_AddInt).

Now let us have a look of SOUI::IIpcConnection
struct IIpcConnection : IObjRef
	{
		virtual IIpcHandle * GetIpcHandle() = 0;
		virtual bool HandleFun(UINT uFunID, SParamStream & ps) = 0;
		virtual void BuildShareBufferName(ULONG_PTR idLocal, ULONG_PTR idRemote, TCHAR szBuf[MAX_PATH]) const = 0;
	};
  
To response for client call, at server client, we implement virtual function HandleFun by using macro FUN_BEGIN and FUN_END as used in demo.

  void OnAddInt(Param_AddInt & param)
  {
  	param.ret = param.a + param.b;
  }
  
	FUN_BEGIN
		FUN_HANDLER(Param_AddInt, OnAddInt)
	FUN_END

It's all. Wish it may be useful to you!

2019.2.26
  
  

