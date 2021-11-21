#pragma once
#ifndef __TRICOL_H__
#define __TRICOL_H__
#include "VIBuffer.h"
class ENGINE_DLL CTriCol final : public CVIBuffer
{
private:
	explicit CTriCol();
	explicit CTriCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol();
public:
	virtual HRESULT Init_Buffer()override;
	virtual void Render_Buffer()override;
	virtual CComponent* Clone_Component()override;
public:
	static CTriCol* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
};
#endif