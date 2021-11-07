#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"
#include "Player.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_fSpeed(0.f)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_fSpeed(0.f)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_TEX)),
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_fSpeed(0.f)
{

}

CMeleeMon::~CMeleeMon()
{

}

HRESULT CMeleeMon::Init_MeleeMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	_vec3	m_vInfo = *m_pTransform->getAxis(VECAXIS::AXIS_POS);

	m_fSpeed = 2.f;

	m_pTransform->setScale(0.1f, 0.1f, 0.1f);

	CGameObject* pObject =  GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 Position = pObject->getTransform()->getPos();

	Chase_Target(&Position, m_fSpeed, fDeltaTime);

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

	m_pTexture->Render_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CMeleeMon::Clone_GameObject()
{
	return new CMeleeMon(*this);
}

CMeleeMon* CMeleeMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon* pInstance = new CMeleeMon(pDevice);

	if (FAILED(pInstance->Init_MeleeMon()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMeleeMon::Attack()
{

}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//texture
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_TEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::MELEEMON_TEX, pComponent);

	//camera
	pComponent = m_pCamera = Clone_ComProto<CCamera>(COMPONENTID::CAMERA);
	m_pCamera->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::CAMERA, m_pBufferCom);

	return S_OK;
}

void CMeleeMon::Free()
{
	//Safe_Release(m_pCamera);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
