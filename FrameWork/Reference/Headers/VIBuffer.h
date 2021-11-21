#pragma once
#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();
public:
	LPDIRECT3DVERTEXBUFFER9 Get_VBuffer() { return m_pVB; }
	LPDIRECT3DINDEXBUFFER9 Get_IBuffer() { return m_pIB; }
	UPOINT Get_XZ() { return UPOINT{ m_dwCntX, m_dwCntZ }; }
protected:
	virtual HRESULT Init_Buffer();
	virtual void Render_Buffer();
	virtual void Render_Buffer(_uint DrawIndex);
protected:
	virtual void Free();
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_ulong m_dwCntX;//가로 버텍스 갯수
	_ulong m_dwCntZ;//새로 버텍스 갯수
	_ulong m_dwVtxCnt;//총 버텍스 갯수
	_ulong m_dwVtxSize; // 버텍스 사이즈 
	_ulong m_dwTriCnt; // 삼각형 개수
	_ulong m_dwInterval; // 간격

	_ulong m_dwFVF; // flexible vertex formal

	D3DFORMAT m_IdxFmt; // 인덱스 버퍼 포맷
	_ulong m_dwIdxSize; // 인덱스 버퍼 사이즈
};
END
#endif