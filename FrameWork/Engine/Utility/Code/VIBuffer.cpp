#include "Engine_Include.h"
#include "VIBuffer.h"

CVIBuffer::CVIBuffer() : m_pVB(nullptr), m_pIB(nullptr), m_dwCntX(0), m_dwCntZ(0),m_dwVtxSize(0),
m_dwFVF(0), m_dwIdxSize(0),m_dwInterval(0),m_dwTriCnt(0),m_dwVtxCnt(0),m_IdxFmt(D3DFMT_UNKNOWN)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pVB(nullptr), m_pIB(nullptr), 
													m_dwCntX(0), m_dwCntZ(0),m_dwVtxSize(0),
													m_dwFVF(0), m_dwIdxSize(0), m_dwInterval(0), 
													m_dwTriCnt(0), m_dwVtxCnt(0), m_IdxFmt(D3DFMT_UNKNOWN)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs) : CComponent(rhs), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB),
m_dwCntX(rhs.m_dwCntX), m_dwCntZ(rhs.m_dwCntZ),
m_dwFVF(rhs.m_dwFVF), m_dwIdxSize(rhs.m_dwIdxSize), m_dwInterval(rhs.m_dwInterval),
m_dwTriCnt(rhs.m_dwTriCnt), m_dwVtxCnt(rhs.m_dwVtxCnt), m_IdxFmt(rhs.m_IdxFmt), m_dwVtxSize(rhs.m_dwVtxSize)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Init_Buffer()
{
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwFVF,
		D3DPOOL_MANAGED,&m_pVB, NULL),E_FAIL);
	FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt,
		D3DPOOL_MANAGED, &m_pIB, NULL), E_FAIL);
	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}
