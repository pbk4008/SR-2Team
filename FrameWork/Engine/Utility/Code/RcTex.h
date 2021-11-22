#pragma once
#ifndef __RCTEX_H__
#define __RCTEX_H__

enum class CubeID {
	ZM,
	ZP,
	XM,
	XP,
	YM,
	YP
};

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
	virtual HRESULT Init_Buffer(_uint index);
	virtual void Render_Buffer();
	virtual CComponent* Clone_Component();
public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice, _uint index);

	virtual LPDIRECT3DVERTEXBUFFER9 Get_VtxBuffer() { return m_pVB; }


private:
	virtual void Free();
};
END
#endif