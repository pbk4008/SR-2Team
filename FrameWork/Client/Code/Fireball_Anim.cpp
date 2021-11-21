#include "pch.h"
#include "Fireball_Anim.h"

CFireball_Anim::CFireball_Anim()
{
}

CFireball_Anim::CFireball_Anim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CFireball_Anim::CFireball_Anim(const CFireball_Anim& rhs) : CAnimation(rhs)
{
}

CFireball_Anim::~CFireball_Anim()
{
}

HRESULT CFireball_Anim::Init_Fireball_Anim()
{
	m_bLoop = true;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.01f;
	InitTexture(L"Boss_Fireball");

	return S_OK;
}

_int CFireball_Anim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	return iExit;
}

void CFireball_Anim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CFireball_Anim::Clone_Component()
{
	return new CFireball_Anim(*this);
}

CFireball_Anim* CFireball_Anim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFireball_Anim* pInstance = new CFireball_Anim(pDevice);
	if (FAILED(pInstance->Init_Fireball_Anim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CFireball_Anim::ResetTimer()
{
}

void CFireball_Anim::Free()
{
	CAnimation::Free();
}
