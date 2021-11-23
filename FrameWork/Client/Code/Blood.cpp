#include "pch.h"
#include "Blood.h"

CBlood::CBlood() : m_pTexture(nullptr)
{
}

CBlood::CBlood(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pTexture(nullptr)
{
}

CBlood::CBlood(const CBlood& rhs) : CGameObject(rhs), m_pTexture(rhs.m_pTexture)
{
	if (!rhs.m_vecBuffer.empty())
	{
		for (auto pBuff : rhs.m_vecBuffer)
		{
			m_vecBuffer.emplace_back(pBuff);
			pBuff->AddRef();
		}
	}
}

CBlood::~CBlood()
{
}

_int CBlood::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CBlood::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBlood::Render_GameObject()
{
	CGameObject::Render_GameObject();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pTexture->Render_Texture();
	for (auto pBuffer : m_vecBuffer)
		pBuffer->Render_Buffer();
}

CGameObject* CBlood::Clone_GameObject()
{
	return new CBlood(*this);
}

void CBlood::ResetObject()
{
}

void CBlood::ActivEffect()
{
	
}

HRESULT CBlood::Add_Component()
{
	CGameObject::Add_Component();

	m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_pTexture->setTexture(GetTexture(L"Blood", TEXTURETYPE::TEX_NORMAL));
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, m_pTexture);
	return S_OK;
}

void CBlood::Free()
{
	CGameObject::Free();
	Safe_Release(m_pTexture);

	for_each(m_vecBuffer.begin(), m_vecBuffer.end(), DeleteObj);
	m_vecBuffer.clear();
	m_vecBuffer.shrink_to_fit();
}

void CBlood::setStart(const _vec3 vStart)
{
	m_vStart = vStart;
	m_pTransform->setPos(m_vStart);
}
