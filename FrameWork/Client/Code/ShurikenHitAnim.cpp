#include "pch.h"
#include "ShurikenHitAnim.h"

CShurikenHitAnim::CShurikenHitAnim()
{
}

CShurikenHitAnim::CShurikenHitAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CShurikenHitAnim::CShurikenHitAnim(const CShurikenHitAnim& rhs) : CAnimation(rhs)
{
}

CShurikenHitAnim::~CShurikenHitAnim()
{
}

HRESULT CShurikenHitAnim::Init_ShurikenHitAnim()
{
	m_bLoop = false;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.2f;
	InitTexture(L"ShurikenEff");

	return S_OK;
}

_int CShurikenHitAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CAnimation::Update_Component(fDeltaTime);

	return _int();
}

void CShurikenHitAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CShurikenHitAnim::Clone_Component()
{
	return new CShurikenHitAnim(*this);
}

CShurikenHitAnim* CShurikenHitAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShurikenHitAnim* pInstance = new CShurikenHitAnim(pDevice);
	if (FAILED(pInstance->Init_ShurikenHitAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CShurikenHitAnim::ResetTimer()
{
	//m_iIndex = 0;
	//m_fPlaySpeed = 0.3f;
	//m_fCulTime = 0.f;
}

void CShurikenHitAnim::Free()
{
	CAnimation::Free();
}
