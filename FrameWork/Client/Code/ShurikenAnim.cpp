#include "pch.h"
#include "ShurikenAnim.h"

CShurikenAnim::CShurikenAnim()
{
}

CShurikenAnim::CShurikenAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CShurikenAnim::CShurikenAnim(const CShurikenAnim& rhs) : CAnimation(rhs)
{
}

CShurikenAnim::~CShurikenAnim()
{
}

HRESULT CShurikenAnim::Init_ShurikenAnim()
{
	m_bLoop = true;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.01f;
	InitTexture(L"Shuriken");

	return S_OK;
}

_int CShurikenAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	return iExit;
}

void CShurikenAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CShurikenAnim::Clone_Component()
{
	return new CShurikenAnim(*this);
}

CShurikenAnim* CShurikenAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShurikenAnim* pInstance = new CShurikenAnim(pDevice);
	if (FAILED(pInstance->Init_ShurikenAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CShurikenAnim::ResetTimer()
{
}

void CShurikenAnim::Free()
{
	CAnimation::Free();
}
