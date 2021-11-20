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
	m_dwVtxCnt = 24;
	m_dwVtxSize = sizeof(VTXCUBE);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	VTXCUBE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	//ZM
	pVertex[0] = VTXCUBE(_vec3{ -0.5f, 0.5f, -0.5f }, _vec3{ 0.f,0.f,-1.f }, _vec2{ 0.f, 0.f });
	pVertex[1] = VTXCUBE(_vec3(0.5f, 0.5f, -0.5f), _vec3{ 0.f,0.f,-1.f }, _vec2(1.f, 0.f));
	pVertex[2] = VTXCUBE(_vec3(0.5f, -0.5f, -0.5f), _vec3{ 0.f,0.f,-1.f }, _vec2(1.f, 1.f));
	pVertex[3] = VTXCUBE(_vec3(-0.5f, -0.5f, -0.5f), _vec3{ 0.f,0.f,-1.f }, _vec2(0.f, 1.f));
	//ZP
	pVertex[4] = VTXCUBE(_vec3(0.5f, 0.5f, 0.5f), _vec3{ 0.f,0.f,1.f }, _vec2(0.f, 0.f));
	pVertex[5] = VTXCUBE(_vec3(-0.5f, 0.5f, 0.5f), _vec3{ 0.f,0.f,1.f }, _vec2(1.f, 0.f));
	pVertex[6] = VTXCUBE(_vec3(-0.5f, -0.5f, 0.5f), _vec3{ 0.f,0.f,1.f }, _vec2(1.f, 1.f));
	pVertex[7] = VTXCUBE(_vec3(0.5f, -0.5f, 0.5f), _vec3{ 0.f,0.f,1.f }, _vec2(0.f, 1.f));
	//XM
	pVertex[8] = VTXCUBE(_vec3(-0.5f, 0.5f, 0.5f), _vec3{ -1.f,0.f,0.f }, _vec2(0.f, 0.f)) ;
	pVertex[9] = VTXCUBE(_vec3(-0.5f, 0.5f, -0.5f), _vec3{ -1.f,0.f,0.f }, _vec2(1.f, 0.f)) ;
	pVertex[10] = VTXCUBE(_vec3(-0.5f, -0.5f, -0.5f), _vec3{ -1.f,0.f,0.f }, _vec2(1.f, 1.f)) ;
	pVertex[11] = VTXCUBE(_vec3(-0.5f, -0.5f, 0.5f), _vec3{ -1.f,0.f,0.f }, _vec2(0.f, 1.f) );
	//XP																  
	pVertex[12] = VTXCUBE(_vec3(0.5f, 0.5f, -0.5f), _vec3{ 1.f,0.f,0.f }, _vec2(0.f, 0.f)) ;
	pVertex[13] = VTXCUBE(_vec3(0.5f, 0.5f, 0.5f), _vec3{ 1.f,0.f, 0.f }, _vec2(1.f, 0.f));
	pVertex[14] = VTXCUBE(_vec3(0.5f, -0.5f, 0.5f), _vec3{ 1.f,0.f,0.f }, _vec2(1.f, 1.f)) ;
	pVertex[15] = VTXCUBE(_vec3(0.5f, -0.5f, -0.5f), _vec3{ 1.f,0.f,0.f }, _vec2(0.f, 1.f));
	//YM																  
	pVertex[16] = VTXCUBE(_vec3(-0.5f, -0.5f, -0.5f), _vec3{ 0.f,-1.f,0.f }, _vec2(0.f, 0.f)) ;
	pVertex[17] = VTXCUBE(_vec3(0.5f, -0.5f, -0.5f), _vec3{ 0.f,-1.f,0.f }, _vec2(1.f, 0.f)) ;
	pVertex[18] = VTXCUBE(_vec3(0.5f, -0.5f, 0.5f), _vec3{ 0.f,-1.f,0.f }, _vec2(1.f, 1.f)) ;
	pVertex[19] = VTXCUBE(_vec3(-0.5f, -0.5f, 0.5f), _vec3{ 0.f,-1.f,0.f }, _vec2(0.f, 1.f)) ;
	//YP																  
	pVertex[20] = VTXCUBE(_vec3(-0.5f, 0.5f, 0.5f), _vec3{ 0.f,1.f,0.f }, _vec2(0.f, 0.f)) ;
	pVertex[21] = VTXCUBE(_vec3(0.5f, 0.5f, 0.5f), _vec3{ 0.f,1.f,0.f }, _vec2(1.f, 0.f)) ;
	pVertex[22] = VTXCUBE(_vec3(0.5f, 0.5f, -0.5f), _vec3{ 0.f,1.f,0.f }, _vec2(1.f, 1.f)) ;
	pVertex[23] = VTXCUBE(_vec3(-0.5f, 0.5f, -0.5f), _vec3{ 0.f,1.f,0.f }, _vec2(0.f, 1.f));

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 5;
	pIndex[2]._2 = 6;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 6;
	pIndex[3]._2 = 7;

	pIndex[4]._0 = 8;
	pIndex[4]._1 = 9;
	pIndex[4]._2 = 10;

	pIndex[5]._0 = 8;
	pIndex[5]._1 = 10;
	pIndex[5]._2 = 11;

	pIndex[6]._0 = 12;
	pIndex[6]._1 = 13;
	pIndex[6]._2 = 14;

	pIndex[7]._0 = 12;
	pIndex[7]._1 = 14;
	pIndex[7]._2 = 15;

	pIndex[8]._0 = 16;
	pIndex[8]._1 = 17;
	pIndex[8]._2 = 18;

	pIndex[9]._0 = 16;
	pIndex[9]._1 = 18;
	pIndex[9]._2 = 19;

	pIndex[10]._0 = 20;
	pIndex[10]._1 = 21;
	pIndex[10]._2 = 22;

	pIndex[11]._0 = 20;
	pIndex[11]._1 = 22;
	pIndex[11]._2 = 23;

	m_pIB->Unlock();

	return S_OK;
}

void CCubeTex::Render_Buffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, 12);
}

void Engine::CCubeTex::Render_Buffer(_uint DrawIndex)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt , DrawIndex * 6, 2);
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
