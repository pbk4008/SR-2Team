#pragma once
#ifndef __TERRAINTEX_H__
#define __TERRAINTEX_H__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex();
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();
public:
	virtual HRESULT Init_Buffer(LPDIRECT3DTEXTURE9 pTexture, const _ulong& dwVtxInv = 1);
	virtual void Render_Buffer();
	virtual CComponent* Clone_Component();
public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 pTexture, const _ulong& dwVtxInv = 1);
private:
	virtual void Free();
private:
	LPDIRECT3DTEXTURE9 m_pHeightMap;
};
END
#endif