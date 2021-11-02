#pragma once
#ifndef __RCCOL_H__
#define __RCCOL_H__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CRcCol final : public CVIBuffer
{
private:
	explicit CRcCol();
	explicit CRcCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();
public:
	virtual HRESULT Init_Buffer();
	virtual void Render_Buffer();
	virtual CComponent* Clone_Component();
public:
	static CRcCol* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
};
END
#endif