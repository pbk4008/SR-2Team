#include "Engine_Include.h"
#include "QuadCubeTex.h"

Engine::CQuadCubeTex::CQuadCubeTex()
{

}

Engine::CQuadCubeTex::CQuadCubeTex(LPDIRECT3DDEVICE9 pDevice)
{

}

Engine::CQuadCubeTex::CQuadCubeTex(const CQuadCubeTex& rhs)
{

}

Engine::CQuadCubeTex::~CQuadCubeTex()
{

}

HRESULT Engine::CQuadCubeTex::Init_Buffer()
{
	m_dwFVF = FVF_QUADCUBE	;
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXQUADCUBE);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	VTXQUADCUBE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 0번 : Z-
	// 1번 : Z+
	// 2번 : X-
	// 3번 : X+
	// 4번 : Y-
	// 5번 : Y+

	pVertex[0].vPos = _vec3(-1.f, 1.f, -1.f);

	pVertex[0].vUV[0] = { 0.f,0.f};
	pVertex[0].vUV[1] = { 0.f,0.f };
	pVertex[0].vUV[2] = { 1.f,0.f };
	pVertex[0].vUV[3] = { 0.f,0.f };
	pVertex[0].vUV[4] = { 0.f,0.f };
	pVertex[0].vUV[5] = { 0.f,1.f };


	pVertex[1].vPos = _vec3(1.f, 1.f, -1.f);

	pVertex[1].vUV[0] = { 1.f,0.f };
	pVertex[1].vUV[1] = { 0.f,0.f };
	pVertex[1].vUV[2] = { 0.f,0.f };
	pVertex[1].vUV[3] = { 0.f,0.f };
	pVertex[1].vUV[4] = { 0.f,0.f };
	pVertex[1].vUV[5] = { 1.f,1.f };

	pVertex[2].vPos = _vec3(1.f, -1.f, -1.f);
	
	pVertex[2].vUV[0] = { 1.f,1.f };
	pVertex[2].vUV[1] = { 0.f,0.f };
	pVertex[2].vUV[2] = { 0.f,0.f };
	pVertex[2].vUV[3] = { 0.f,1.f };
	pVertex[2].vUV[4] = { 1.f,0.f };
	pVertex[2].vUV[5] = { 0.f,0.f };

	pVertex[3].vPos = _vec3(-1.f, -1.f, -1.f);
	
	pVertex[3].vUV[0] = { 0.f,1.f };
	pVertex[3].vUV[1] = { 0.f,0.f };
	pVertex[3].vUV[2] = { 1.f,1.f };
	pVertex[3].vUV[3] = { 0.f,0.f };
	pVertex[3].vUV[4] = { 0.f,0.f };
	pVertex[3].vUV[5] = { 0.f,0.f };

	pVertex[4].vPos = _vec3(-1.f, 1.f, 1.f);
	
	pVertex[4].vUV[0] = { 0.f,0.f };
	pVertex[4].vUV[1] = { 1.f,0.f };
	pVertex[4].vUV[2] = { 0.f,0.f };
	pVertex[4].vUV[3] = { 0.f,0.f };
	pVertex[4].vUV[4] = { 0.f,0.f };
	pVertex[4].vUV[5] = { 0.f,0.f };

	pVertex[5].vPos = _vec3(1.f, 1.f, 1.f);
	
	pVertex[5].vUV[0] = { 0.f,0.f };
	pVertex[5].vUV[1] = { 0.f,0.f };
	pVertex[5].vUV[2] = { 0.f,0.f };
	pVertex[5].vUV[3] = { 1.f,0.f };
	pVertex[5].vUV[4] = { 0.f,0.f };
	pVertex[5].vUV[5] = { 1.f,0.f };

	pVertex[6].vPos = _vec3(1.f, -1.f, 1.f);
	
	pVertex[6].vUV[0] = { 0.f,0.f };
	pVertex[6].vUV[1] = { 0.f,1.f };
	pVertex[6].vUV[2] = { 0.f,0.f };
	pVertex[6].vUV[3] = { 1.f,1.f };
	pVertex[6].vUV[4] = { 1.f,1.f };
	pVertex[6].vUV[5] = { 0.f,0.f };


	pVertex[7].vPos = _vec3(-1.f, -1.f, 1.f);
	
	pVertex[7].vUV[0] = { 0.f,0.f };
	pVertex[7].vUV[1] = { 1.f,1.f };
	pVertex[7].vUV[2] = { 0.f,1.f };
	pVertex[7].vUV[3] = { 0.f,0.f };
	pVertex[7].vUV[4] = { 0.f,1.f };
	pVertex[7].vUV[5] = { 0.f,0.f };

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// x+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;
	// x-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;
	// y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;
	// y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;
	// z+
	pIndex[8]._0 = 5;
	pIndex[8]._1 = 4;
	pIndex[8]._2 = 7;

	pIndex[9]._0 = 5;
	pIndex[9]._1 = 7;
	pIndex[9]._2 = 6;
	// z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CQuadCubeTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

Engine::CComponent* Engine::CQuadCubeTex::Clone_Component()
{
	return new CQuadCubeTex(*this);
}

Engine::CQuadCubeTex* Engine::CQuadCubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CQuadCubeTex* pInstance = new CQuadCubeTex(pDevice);
	if (FAILED(pInstance->Init_Buffer()))
		Safe_Release(pInstance);
	return pInstance;
}

void Engine::CQuadCubeTex::Free()
{
	CVIBuffer::Free();
}

