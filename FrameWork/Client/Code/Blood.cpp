#include "pch.h"
#include "Blood.h"

CBlood::CBlood() : m_pTexture(nullptr), m_pBuffer(nullptr)
{
}

CBlood::CBlood(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pTexture(nullptr), m_pBuffer(nullptr)
{
}

CBlood::CBlood(const CBlood& rhs) : CGameObject(rhs), m_pTexture(rhs.m_pTexture), m_pBuffer(rhs.m_pBuffer)
{
	m_pBuffer->AddRef();
	m_pTexture->AddRef();
}

CBlood::~CBlood()
{
}

HRESULT CBlood::Init_Blood()
{
	Add_Component();
	return S_OK;
}

_int CBlood::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->UsingGravity(fDeltaTime);
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	TerrainCheck();
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
	m_pBuffer->Render_Buffer();
}

CGameObject* CBlood::Clone_GameObject()
{
	return new CBlood(*this);
}

void CBlood::ResetObject()
{
	m_pTransform->setPos(m_vStart);
}

void CBlood::TerrainCheck()
{
	_vec3 vPivot = m_pTransform->getPos();
	vPivot.y -= m_pTransform->getScale().y * 0.5f;
	if (vPivot.y < m_pTransform->getBottomY())
	{
		setActive(false);
	}
}

CBlood* CBlood::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBlood* pInstance = new CBlood(pDevice);
	if (FAILED(pInstance->Init_Blood()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CBlood::Add_Component()
{
	CGameObject::Add_Component();

	m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, m_pBuffer);


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
	Safe_Release(m_pBuffer);
}

void CBlood::setStart(const _vec3 vStart)
{
	m_vStart = vStart;
	m_pTransform->setPos(m_vStart);
}
