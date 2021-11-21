#include "Engine_Include.h"
#include "FrameMgr.h"
#include "Animation.h"
#include "Texture.h"
#include "TextureMgr.h"
#include "ProtoMgr.h"
#pragma comment(lib,"System.lib")

CAnimation::CAnimation() : m_pTexture(nullptr), m_bPlay(false), m_fPlaySpeed(0.f), m_fCulTime(0.f),m_iIndex(0), m_bLoop(false), m_bDelay(false), m_pTextureMgr(nullptr)
{
}

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pTexture(nullptr),m_bPlay(false), m_fPlaySpeed(0.f), m_fCulTime(0.f), m_iIndex(0), m_bLoop(nullptr),m_bDelay(false)
, m_pTextureMgr(CTextureMgr::GetInstance())
{
	m_pTextureMgr->AddRef();
}

CAnimation::CAnimation(const CAnimation& rhs) : CComponent(rhs), m_pTexture(rhs.m_pTexture),m_bPlay(rhs.m_bPlay), m_fPlaySpeed(rhs.m_fPlaySpeed), m_fCulTime(rhs.m_fCulTime), m_iIndex(rhs.m_iIndex), m_bLoop(rhs.m_bLoop)
,m_bDelay(rhs.m_bDelay), m_pTextureMgr(rhs.m_pTextureMgr)
{
	m_pTextureMgr->AddRef();
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
			if (!m_bDelay)
			{
				m_iIndex++;
				if (m_iIndex >= m_pTexture->getTextureCount())
				{
					m_iIndex = 0;
					if (!m_bLoop)
						m_bPlay = false;
				}
				m_fCulTime = 0.f;
			}
		}
	}
}

void CAnimation::InitTexture(const _tchar* pTextureName)
{
	vector<LPDIRECT3DBASETEXTURE9>* pTex = m_pTextureMgr->getTexture(pTextureName, TEXTURETYPE::TEX_NORMAL);

	m_pTexture = CProtoMgr::GetInstance()->Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_pTexture->setTexture(pTex);
}

void CAnimation::ResetTimer()
{
}

void CAnimation::Free()
{
	Safe_Release(m_pTextureMgr);
	Safe_Release(m_pTexture);
	CComponent::Free();
}

void CAnimation::setTexture(const _tchar* pTextureName)
{
	m_pTexture->setTexture(m_pTextureMgr->getTexture(pTextureName,TEXTURETYPE::TEX_NORMAL));
}

void CAnimation::setPlay(_bool bPlay)
{
	if (bPlay)
		ResetTimer();
	m_bPlay = bPlay;
}
