#include "pch.h"
#include "Blood.h"

CBlood::CBlood() : m_pTexture(nullptr)
{
}

CBlood::CBlood(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pTexture(nullptr)
{
}

CBlood::CBlood(const CBlood& rhs) : CGameObject(rhs), m_pTexture(rhs.m_pTexture)
{
	//if(rhs.m_vecBuffer.empty)
}

CBlood::~CBlood()
{
}

_int CBlood::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	return iExit;
}

void CBlood::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBlood::Render_GameObject()
{
	CGameObject::Render_GameObject();
}

CGameObject* CBlood::Clone_GameObject()
{
	return new CBlood(*this);
}

void CBlood::ResetObject()
{
}

HRESULT CBlood::Add_Component()
{
	CGameObject::Add_Component();
	return S_OK;
}

void CBlood::Free()
{
	CGameObject::Free();
}
