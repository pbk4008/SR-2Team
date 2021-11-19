#include "pch.h"
#include "TerrainObject.h"

CTerrainObject::CTerrainObject()
	:m_pTerrainTex(nullptr)
{
}

CTerrainObject::CTerrainObject(LPDIRECT3DDEVICE9 pDevice)
	: CToolGameObject(pDevice)
	, m_pTerrainTex(nullptr)
{
}

CTerrainObject::CTerrainObject(const CTerrainObject& rhs)
	: CToolGameObject(rhs)
	, m_pTerrainTex(rhs.m_pTerrainTex)
{
	VTXINFO newVtxInfo;
	newVtxInfo.X = rhs.m_pTerrainTex->getCntX();
	newVtxInfo.Z = rhs.m_pTerrainTex->getCntZ();
	newVtxInfo.Interval = rhs.m_pTerrainTex->getInterval();
	m_pTerrainTex = CTerrainTex::Create(m_pDevice,newVtxInfo.X,newVtxInfo.Z,newVtxInfo.Interval);
}

CTerrainObject::~CTerrainObject()
{

}

HRESULT CTerrainObject::Init_CTerrainObject(VTXINFO tVtxInfo)
{
	memcpy(&m_tVTXINFO, &tVtxInfo,sizeof(tVtxInfo));
	m_vecTextureInfo.resize(1);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CTerrainObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);


	return iExit;
}

void CTerrainObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CTerrainObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	m_pDevice->SetMaterial(&mMaterial);


	if (!m_vecTextureInfo.empty() && 
		m_vecTextureInfo.front().pTexture)
	{
		m_vecTextureInfo.front().pTexture->Render_Texture();
	}
	else
	{
		m_pDevice->SetTexture(0, nullptr);
	}

	m_pTerrainTex->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CTerrainObject::Clone_GameObject()
{

	return new CTerrainObject(*this);
}


CTerrainObject* CTerrainObject::Create(LPDIRECT3DDEVICE9 pDevice, VTXINFO tVtxInfo)
{
	CTerrainObject* pInstance = new CTerrainObject(pDevice);
	if(FAILED(pInstance->Init_CTerrainObject(tVtxInfo)))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CTerrainObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pTerrainTex = CTerrainTex::Create(m_pDevice,m_tVTXINFO.X, m_tVTXINFO.Z, m_tVTXINFO.Interval,m_tVTXINFO.Detail);
	m_mapComponent->emplace(COMPONENTID::TERRAINTEX, pComponent);



	return S_OK;
}

void CTerrainObject::Free()
{
	Safe_Release(m_pTerrainTex);
	CToolGameObject::Free();
	CGameObject::Free();
}
