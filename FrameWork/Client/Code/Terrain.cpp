#include "pch.h"
#include "Terrain.h"

CTerrain::CTerrain():m_pBufferCom(nullptr)
{
}

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBufferCom(nullptr)
{
}

CTerrain::CTerrain(const CTerrain& rhs):CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom)
{
	m_pBufferCom->AddRef();
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Init_Terrain()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CTerrain::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CTerrain::Render_GameObject()
{
	CGameObject::Render_GameObject();
	m_pBufferCom->Render_Buffer();
}

CGameObject* CTerrain::Clone_GameObject()
{
	return new CTerrain(*this);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);
	if (FAILED(pInstance->Init_Terrain()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CTerrain::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	pCom = m_pBufferCom = Clone_ComProto<CTerrainTex>(COMPONENTID::TERRAINTEX);
	NULL_CHECK_RETURN(m_pBufferCom,E_FAIL);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TERRAINTEX, pCom);
	
	//pCom = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TERRAIN_TEX1,pCom);

	
	return S_OK;
}

void CTerrain::Free()
{
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
