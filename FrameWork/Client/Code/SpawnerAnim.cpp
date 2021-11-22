#include "pch.h"
#include "SpawnerAnim.h"

CSpawnerAnim::CSpawnerAnim()
{
}

CSpawnerAnim::CSpawnerAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CSpawnerAnim::CSpawnerAnim(const CSpawnerAnim& rhs) : CAnimation(rhs)
{
}

CSpawnerAnim::~CSpawnerAnim()
{
}

HRESULT CSpawnerAnim::Init_SpawnerAnim()
{
	m_bLoop = true;
	m_bPlay = true;
	m_bActive = true;
	m_fPlaySpeed = 0.01f;
	InitTexture(L"Spawner");

	return S_OK;
}

_int CSpawnerAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = CAnimation::Update_Component(fDeltaTime);

	return iExit;
}

void CSpawnerAnim::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CSpawnerAnim::Clone_Component()
{
	return new CSpawnerAnim(*this);
}

CSpawnerAnim* CSpawnerAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpawnerAnim* pInstance = new CSpawnerAnim(pDevice);
	if (FAILED(pInstance->Init_SpawnerAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CSpawnerAnim::ResetTimer()
{
}

void CSpawnerAnim::Free()
{
	CAnimation::Free();
}
