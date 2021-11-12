#include "pch.h"
#include "BombAnim.h"

CBombAnim::CBombAnim()
{
}

CBombAnim::CBombAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CBombAnim::CBombAnim(const CBombAnim& rhs) : CAnimation(rhs)
{
}

CBombAnim::~CBombAnim()
{
}

HRESULT CBombAnim::Init_BombAnim()
{
	m_bLoop = true;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.01f;
	InitTexture(L"Bomb");
	return S_OK;
}

_int CBombAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CAnimation::Update_Component(fDeltaTime);
	return iExit;
}

void CBombAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CBombAnim::Clone_Component()
{
	return new CBombAnim(*this);
}

CBombAnim* CBombAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBombAnim* pInstance = new CBombAnim(pDevice);
	if (FAILED(pInstance->Init_BombAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CBombAnim::Free()
{
	CAnimation::Free();
}
