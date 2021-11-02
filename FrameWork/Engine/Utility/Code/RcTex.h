#pragma once
#ifndef __RCTEX_H__
#define __RCTEX_H__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CRcTex final : public CVIBuffer
{
private:
	explicit CRcTex();
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();
public:
	virtual HRESULT Init_Buffer();
	virtual void Render_Buffer();
	virtual CComponent* Clone_Component();
public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
};
END
#endif