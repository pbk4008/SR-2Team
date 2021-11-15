#include "pch.h"
#include "QuadObject.h"

CQuadObject::CQuadObject()
	: m_pQuadTex(nullptr)
{
}

CQuadObject::CQuadObject(LPDIRECT3DDEVICE9 pDevice)
	: CToolGameObject(pDevice)
	, m_pQuadTex(nullptr)
{
}

CQuadObject::CQuadObject(const CQuadObject& rhs)
	: CToolGameObject(rhs)
	//, m_pQuadTex(rhs.m_pQuadTex)
{
	m_pQuadTex = CRcTex::Create(m_pDevice);
}

CQuadObject::~CQuadObject()
{

}

HRESULT CQuadObject::Init_CQuadObject()
{
	m_tVTXINFO.X = 2;
	m_tVTXINFO.Z = 2;
	m_tVTXINFO.Detail = 1;
	m_tVTXINFO.Interval = 1;
	m_vecTextureInfo.resize(1);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CQuadObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CQuadObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CQuadObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	if (!m_vecTextureInfo.empty() && 
		m_vecTextureInfo.front().pTexture)
	{
		m_vecTextureInfo.front().pTexture->Render_Texture();
	}
	else
	{
		m_pDevice->SetTexture(0, nullptr);
	}

	m_pQuadTex->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CQuadObject::Clone_GameObject()
{
	return new CQuadObject(*this);

}

CQuadObject* CQuadObject::Create(LPDIRECT3DDEVICE9 pDevice/*, QUADINFO tQuadInfo*/)
{
	CQuadObject* pInstance = new CQuadObject(pDevice);
	if (FAILED(pInstance->Init_CQuadObject()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CQuadObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pQuadTex = CRcTex::Create(m_pDevice);
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CQuadObject::Free()
{

	Safe_Release(m_pQuadTex);
	CToolGameObject::Free();
	CGameObject::Free();
}
