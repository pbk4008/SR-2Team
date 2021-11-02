#include "Engine_Include.h"
#include "RcTex.h"

CRcTex::CRcTex()
{
}

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pDevice) : CVIBuffer(pDevice)
{
}

CRcTex::CRcTex(const CRcTex& rhs) : CVIBuffer(rhs)
{
}

CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Init_Buffer()
{
	m_dwFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwCntX = 2;
	m_dwCntZ = 2;
	m_dwVtxCnt = m_dwCntX * m_dwCntZ;
	m_dwInterval = 1;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-1.f, 1.f, 0.f);
	pVertex[0].vUV = _vec2(0.f,0.f);

	pVertex[1].vPos = _vec3(1.f, 1.f, 0.f);
	pVertex[1].vUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(1.f, -1.f, 0.f);
	pVertex[2].vUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(-1.f, -1.f, 0.f);
	pVertex[3].vUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRcTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CRcTex::Clone_Component()
{
	return new CRcTex(*this);
}

CRcTex* CRcTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRcTex* pInstance = new CRcTex(pDevice);
	if (FAILED(pInstance->Init_Buffer()))
		Safe_Release(pInstance);
	return pInstance;
}

void CRcTex::Free()
{
	CVIBuffer::Free();
}
