#include "pch.h"
#include "Player.h"

CPlayer::CPlayer() : m_pBufferCom(nullptr), m_pTexture(nullptr)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice): CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr)
{
}

CPlayer::CPlayer(const CPlayer& rhs) : CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Init_Player()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return _int();
}

void CPlayer::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
}

CGameObject* CPlayer::Clone_GameObject()
{
	return new CPlayer(*this);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->Init_Player()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CPlayer::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	pCom = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent->emplace(COMPONENTID::RCTEX, pCom);

	pCom = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::PLAYER_TEX);
	m_pTexture->AddRef();
	m_mapComponent->emplace(COMPONENTID::PLAYER_TEX, pCom);

	return S_OK;
}

void CPlayer::Free()
{
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTexture);
	CGameObject::Free();
}
