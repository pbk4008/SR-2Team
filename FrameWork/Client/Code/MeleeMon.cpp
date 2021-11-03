#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr)
{

}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(Clone_ComProto<CTexture>(COMPONENTID::RCCOL))
{

}

CMeleeMon::~CMeleeMon()
{

}

HRESULT CMeleeMon::Init_MeleeMon(SCENEID eID)
{
	m_eSceneID = eID;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CMeleeMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMeleeMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CMeleeMon::Clone_GameObject()
{
	return new CMeleeMon(*this);
}

CMeleeMon* CMeleeMon::Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eID)
{
	CMeleeMon* pInstance = new CMeleeMon(pDevice);

	if (FAILED(pInstance->Init_MeleeMon(eID)))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = Clone_ComProto<CRcCol>(COMPONENTID::RCCOL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CMeleeMon::Free()
{
	CGameObject::Free();
}

void CMeleeMon::setTextureCom(SCENEID eID)
{

}
