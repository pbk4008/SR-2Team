#include "Engine_Include.h"
#include "FrameMgr.h"
#include "Animation.h"
#include "Texture.h"
#pragma comment(lib,"System.lib")

CAnimation::CAnimation() : m_pTexture(nullptr), m_bPlay(false), m_fPlaySpeed(0.f), m_fCulTime(0.f),m_iIndex(0), m_bLoop(false)
{
}

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pTexture(nullptr),m_bPlay(false), m_fPlaySpeed(0.f), m_fCulTime(0.f), m_iIndex(0), m_bLoop(nullptr)
{
}

CAnimation::CAnimation(const CAnimation& rhs) : CComponent(rhs), m_pTexture(rhs.m_pTexture),m_bPlay(rhs.m_bPlay), m_fPlaySpeed(rhs.m_fPlaySpeed), m_fCulTime(rhs.m_fCulTime), m_iIndex(rhs.m_iIndex), m_bLoop(rhs.m_bLoop)
{
	m_pTexture->AddRef();
}

CAnimation::~CAnimation()
{
}



_int CAnimation::Update_Component(const _float& fDeltaTime)
{
	return 0;
}

void CAnimation::Render_Animation()
{
	_float fDeltaTime = CFrameMgr::GetInstance()->getOutDeltaTime();
	m_pTexture->Render_Texture(m_iIndex);
	if (m_bPlay)
	{
		m_fCulTime += fDeltaTime;
		if (m_fCulTime >= m_fPlaySpeed)
		{
			m_iIndex++;
			if (m_iIndex >= m_pTexture->getTextureCount())
			{
				m_iIndex = 0;
				if(!m_bLoop)
					m_bPlay = false;
			}
			m_fCulTime = 0.f;
		}
	}
}

void CAnimation::Free()
{
	Safe_Release(m_pTexture);
	CComponent::Free();
}

void CAnimation::setTexture(CTexture* pTexutre)
{
	if (m_pTexture)
		Safe_Release(m_pTexture);
	m_pTexture = pTexutre;
	m_pTexture->AddRef();
}
