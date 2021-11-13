#include "pch.h"
#include "MonBulletAnim.h"

CMonBulletAnim::CMonBulletAnim()
{
}

CMonBulletAnim::CMonBulletAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CMonBulletAnim::CMonBulletAnim(const CMonBulletAnim& rhs) : CAnimation(rhs)
{
}

CMonBulletAnim::~CMonBulletAnim()
{
}

HRESULT CMonBulletAnim::Init_MonBulletAnim()
{
	m_bLoop = true;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.01f;
	InitTexture(L"MonBullet");

	return S_OK;
}

_int CMonBulletAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	return iExit;
}

void CMonBulletAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CMonBulletAnim::Clone_Component()
{
	return new CMonBulletAnim(*this);
}

CMonBulletAnim* CMonBulletAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonBulletAnim* pInstance = new CMonBulletAnim(pDevice);
	if (FAILED(pInstance->Init_MonBulletAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CMonBulletAnim::ResetTimer()
{
}

void CMonBulletAnim::Free()
{
	CAnimation::Free();
}
