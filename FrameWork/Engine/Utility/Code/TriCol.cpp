#include "Engine_Include.h"
#include "TriCol.h"

CTriCol::CTriCol()
{
}

CTriCol::CTriCol(LPDIRECT3DDEVICE9 pDevice) : CVIBuffer(pDevice)
{
}

CTriCol::CTriCol(const CTriCol& rhs) : CVIBuffer(rhs)
{
}

CTriCol::~CTriCol()
{
}

HRESULT CTriCol::Init_Buffer()
{
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 1;
	m_dwCntX = 2;
	m_dwCntZ = 2;
	m_dwVtxCnt = 3;
	m_dwInterval = 1;
	m_dwVtxSize = sizeof(VTXCOL);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(0.f, 1.f, 0.f);
	pVertex[0].tColor = D3DXCOLOR(1.0f,0.f,0.f,1.f);

	pVertex[1].vPos = _vec3(1.f, -1.f, 0.f);
	pVertex[1].tColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);

	pVertex[2].vPos = _vec3(-1.f, -1.f, 0.f);
	pVertex[2].tColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();
	return S_OK;
}


void CTriCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CTriCol* CTriCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriCol* pInstance = new CTriCol(pDevice);
	if (FAILED(pInstance->Init_Buffer()))
		Safe_Release(pInstance);
	return pInstance;
}

CComponent* CTriCol::Clone_Component()
{
	return new CTriCol(*this);
}

void CTriCol::Free()
{
	CVIBuffer::Free();
}
