#include "Engine_Include.h"
#include "CubeTex.h"

CCubeTex::CCubeTex()
{
}

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pDevice) : CVIBuffer(pDevice)
{
}

CCubeTex::CCubeTex(const CCubeTex& rhs):CVIBuffer(rhs)
{
}

CCubeTex::~CCubeTex()
{
}

HRESULT CCubeTex::Init_Buffer()
{
	m_dwFVF = FVF_CUBE;
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBE);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	VTXCUBE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-1.f, 1.f, -1.f);
	pVertex[0].vUV = pVertex[0].vPos;

	pVertex[1].vPos = _vec3(1.f, 1.f, -1.f);
	pVertex[1].vUV = pVertex[1].vPos;

	pVertex[2].vPos = _vec3(1.f, -1.f, -1.f);
	pVertex[2].vUV = pVertex[2].vPos;

	pVertex[3].vPos = _vec3(-1.f, -1.f, -1.f);
	pVertex[3].vUV = pVertex[3].vPos;

	pVertex[4].vPos = _vec3(-1.f, 1.f, 1.f);
	pVertex[4].vUV = pVertex[4].vPos;

	pVertex[5].vPos = _vec3(1.f, 1.f, 1.f);
	pVertex[5].vUV = pVertex[5].vPos;

	pVertex[6].vPos = _vec3(1.f, -1.f, 1.f);
	pVertex[6].vUV = pVertex[6].vPos;

	pVertex[7].vPos = _vec3(-1.f, -1.f, 1.f);
	pVertex[7].vUV = pVertex[7].vPos;

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CCubeTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CCubeTex::Clone_Component()
{
	return new CCubeTex(*this);
}

CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTex* pInstance = new CCubeTex(pDevice);
	if (FAILED(pInstance->Init_Buffer()))
		Safe_Release(pInstance);
	return pInstance;
}

void CCubeTex::Free()
{
	CVIBuffer::Free();
}
