#include "pch.h"
#include "FogAnim.h"

CFogAnim::CFogAnim() :m_iRoutin(0)
{
}

CFogAnim::CFogAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice), m_iRoutin(0)
{
}

CFogAnim::CFogAnim(const CFogAnim& rhs) : CAnimation(rhs), m_iRoutin(rhs.m_iRoutin)
{
}

CFogAnim::~CFogAnim()
{
}

HRESULT CFogAnim::Init_FogAnim()
{
	m_bActive = true;
	m_bLoop = true;
	m_fPlaySpeed = 0.05f;
	m_iRoutin = 0;
	InitTexture(L"BombSmoge");

	return S_OK;
}

_int CFogAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	return iExit;
}

void CFogAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CFogAnim::Clone_Component()
{
	return new CFogAnim(*this);
}

CFogAnim* CFogAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFogAnim* pInstance = new CFogAnim(pDevice);
	if (FAILED(pInstance->Init_FogAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CFogAnim::Free()
{
	CAnimation::Free();
}
