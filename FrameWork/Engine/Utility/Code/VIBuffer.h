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
protected:
	virtual HRESULT Init_Buffer();
	virtual void Render_Buffer();
protected:
	virtual void Free();
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_ulong m_dwCntX;//���� ���ؽ� ����
	_ulong m_dwCntZ;//���� ���ؽ� ����
	_ulong m_dwVtxCnt;//�� ���ؽ� ����
	_ulong m_dwVtxSize;
	_ulong m_dwTriCnt;
	_ulong m_dwInterval;

	_ulong m_dwFVF;

	D3DFORMAT m_IdxFmt;
	_ulong m_dwIdxSize;
};
END
#endif