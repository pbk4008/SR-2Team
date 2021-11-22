#include "pch.h"
#include"PotalAnim.h"

CPotalAnim::CPotalAnim()
{
}

CPotalAnim::CPotalAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CPotalAnim::CPotalAnim(const CPotalAnim& rhs) : CAnimation(rhs)
{
}

CPotalAnim::~CPotalAnim()
{
}

HRESULT CPotalAnim::Init_PotalAnim()
{
	m_bLoop = true;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.01f;
	InitTexture(L"Potal");
	return S_OK;
}

_int CPotalAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = CAnimation::Update_Component(fDeltaTime);

	return iExit;
}

void CPotalAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CPotalAnim::Clone_Component()
{
	return new CPotalAnim(*this);
}

CPotalAnim* CPotalAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPotalAnim* pInstance = new CPotalAnim(pDevice);
	if (FAILED(pInstance->Init_PotalAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CPotalAnim::ResetTimer()
{
}

void CPotalAnim::Free()
{
	CAnimation::Free();
}
