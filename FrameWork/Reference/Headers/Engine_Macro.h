#ifndef __ENGINE_MACRO_H__
#define __ENGINE_MACRO_H__
namespace Engine
{

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL __declspec(dllexport)
#else
#define ENGINE_DLL __declspec(dllimport)
#endif
#ifdef _AFX
#define MSG_BOX(_msg) AfxMessageBox(_msg);
#else
#define MSG_BOX(_msg) MessageBox(nullptr,_msg,L"System Message",MB_OK);
#endif

#define NULL_CHECK(_ptr)	if(!_ptr)	{__asm{int 3}; return;}
#define NULL_CHECK_RETURN(_ptr,_return) if(!_ptr){__asm{int 3}; return _return;}
#define NULL_CHECK_MSG(_ptr,_msg) if(!_ptr){__asm{int 3}; MSG_BOX(_msg); return;}
#define NULL_CHECK_RETURN_MSG(_ptr,_msg) if(!_ptr){__asm{int 3}; MSG_BOX(_msg); return _return;}

#define FAILED_CHECK(_hr) if((HRESULT)(_hr)<0)	\
{MSG_BOX(L"Fail"); __asm{int 3}; return E_FAIL;}
#define FAILED_CHECK_RETURN(_hr,_return) if((HRESULT)(_hr)<0)	\
{MSG_BOX(L"Fail"); __asm{int 3}; return _return;}
#define FAILED_CHECK_MSG(_hr,_msg) if((HRESULT)(_hr)<0)	\
{MSG_BOX(_msg); __asm{int 3}; return E_FAIL;}
#define FAILED_CHECK_RETURN_MSG(_hr,_return,_msg) if((HRESULT)(_hr)<0)	\
{MSG_BOX(_msg); __asm{int 3}; return _return;}


#define NOCOPY(CLASSNAME)								\
private:												\
	explicit CLASSNAME(const CLASSNAME&);						\
	CLASSNAME& operator= (const CLASSNAME&);	

#define DECLARE_SINGLETON(CLASSNAME)					\
NOCOPY(CLASSNAME)										\
private:												\
	static CLASSNAME* m_pInstance;						\
public:													\
	static CLASSNAME* GetInstance();					\
	static unsigned long DestroyInstance();

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
CLASSNAME* CLASSNAME::m_pInstance = nullptr;			\
CLASSNAME* CLASSNAME::GetInstance()						\
{														\
	if (!m_pInstance)									\
		m_pInstance = new CLASSNAME;					\
	return m_pInstance;									\
}														\
														\
unsigned long CLASSNAME::DestroyInstance()				\
{														\
	unsigned long dwRefCnt = 0;							\
	if (m_pInstance)									\
	{													\
		dwRefCnt = m_pInstance->Release();				\
		if (dwRefCnt <= 0)								\
		{												\
			m_pInstance = nullptr;						\
			return 0;									\
		}												\
	}													\
	return dwRefCnt;									\
}

}
#endif