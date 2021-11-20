#include "pch.h"
#include "TestCubeObject.h"

CTestCubeObject::CTestCubeObject()
	: m_pCubeTex(nullptr)
{
}

CTestCubeObject::CTestCubeObject(LPDIRECT3DDEVICE9 pDevice)
	: CToolGameObject(pDevice)
	, m_pCubeTex(nullptr)
{

}

CTestCubeObject::CTestCubeObject(const CTestCubeObject& rhs)
	: CToolGameObject(rhs)

{
	m_pCubeTex = CCubeTex::Create(m_pDevice);
}

CTestCubeObject::~CTestCubeObject()
{
}

HRESULT CTestCubeObject::Init_CTestCubeObject()
{
	m_tVTXINFO.X = 2;
	m_tVTXINFO.Z = 2;
	m_tVTXINFO.Interval = 1;
	m_tVTXINFO.Detail = 1;
	m_vecTextureInfo.resize(6);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CTestCubeObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CTestCubeObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CTestCubeObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	m_pDevice->SetMaterial(&mMaterial);
	
	for(_uint i = 0 ; i < m_vecTextureInfo.size() ; ++i)
	{
		if (m_vecTextureInfo[i].pTexture)
		{
			m_vecTextureInfo[i].pTexture->Render_Texture();
		}
		else
		{
			m_pDevice->SetTexture(0, nullptr);
		}
		
		m_pCubeTex->Render_Buffer(i);
	}


	CGameObject::Render_GameObject();

	
}

Engine::CGameObject* CTestCubeObject::Clone_GameObject()
{
	return new CTestCubeObject(*this);
}

CTestCubeObject* CTestCubeObject::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTestCubeObject* pInstance = new CTestCubeObject(pDevice);
	if (FAILED(pInstance->Init_CTestCubeObject()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CTestCubeObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeTex =  CCubeTex::Create(m_pDevice);		
	m_mapComponent->emplace(COMPONENTID::CUBETEX, pComponent);



	return S_OK;
}

void CTestCubeObject::Free()
{
	CToolGameObject::Free();
	CGameObject::Free();
}
