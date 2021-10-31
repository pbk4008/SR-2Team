#ifndef __BASE_H__
#define __BASE_H__
class __declspec(dllexport) CBase abstract
{
protected:
	inline explicit CBase();
	inline virtual ~CBase();
public:
	inline unsigned long AddRef();
	inline unsigned long Release();
protected:
	inline virtual void Free();
private:
	unsigned long m_dwRefCnt;
};
#include "Base.inl"
#endif