#pragma once
#ifndef __CUBETEX_H__
#define __CUBETEX_H__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CCubeTex final : public CVIBuffer
{
private:
	explicit CCubeTex();
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();
public:
	virtual HRESULT Init_Buffer();
	virtual void Render_Buffer();
	virtual CComponent* Clone_Component();
public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
};
END
#endif