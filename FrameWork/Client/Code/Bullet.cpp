#include "pch.h"
#include "Bullet.h"

CBullet::CBullet() : m_pBuffer(nullptr)
{
}

CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBuffer(nullptr)
{
}

CBullet::CBullet(const CBullet& rhs) : CGameObject(rhs), m_pBuffer(rhs.m_pBuffer)
{
	if (rhs.m_pBuffer)
		m_pBuffer->AddRef();
}

CBullet::~CBullet()
{
}

_int CBullet::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	return _int();
}

void CBullet::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBullet::Render_GameObject()
{
	CGameObject::Render_GameObject();
}

HRESULT CBullet::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	pCom = m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);


	return S_OK;
}

void CBullet::Free()
{
	Safe_Release(m_pBuffer);


	CGameObject::Free();
}
