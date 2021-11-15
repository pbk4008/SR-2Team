#include "pch.h"
#include "CubeObject.h"

CCubeObject::CCubeObject()
	: m_pCubeTex{}
{
}

CCubeObject::CCubeObject(LPDIRECT3DDEVICE9 pDevice)
	: CToolGameObject(pDevice)
	, m_pCubeTex{}
{

}

CCubeObject::CCubeObject(const CCubeObject& rhs)
	: CToolGameObject(rhs)
{
	std::copy(rhs.m_pCubeTex.begin(), rhs.m_pCubeTex.end(), m_pCubeTex.begin());
}

CCubeObject::~CCubeObject()
{
}

HRESULT CCubeObject::Init_CCubeObject()
{
	m_tVTXINFO.X = 2;
	m_tVTXINFO.Z = 2;
	m_tVTXINFO.Interval = 1;
	m_tVTXINFO.Detail = 1;
	m_vecTextureInfo.resize(6);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CCubeObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CCubeObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CCubeObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	
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
		
		m_pCubeTex[i]->Render_Buffer();
	}


	CGameObject::Render_GameObject();

	
}

Engine::CGameObject* CCubeObject::Clone_GameObject()
{
	return new CCubeObject(*this);
}

CCubeObject* CCubeObject::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeObject* pInstance = new CCubeObject(pDevice);
	if (FAILED(pInstance->Init_CCubeObject()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CCubeObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;


	for (_uint i = 0; i < 6; ++i)
	{
		pComponent = m_pCubeTex[i] = CRcTex::Create(m_pDevice,i);
		m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);
	}



	return S_OK;
}

void CCubeObject::Free()
{
	for (auto& cube : m_pCubeTex)
		Safe_Release(cube);

	CToolGameObject::Free();
	CGameObject::Free();
}
