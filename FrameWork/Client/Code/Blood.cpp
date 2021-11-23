#include "pch.h"
#include "Blood.h"

CBlood::CBlood() : m_pTexture(nullptr), m_pBuffer(nullptr), m_fGravityTime(0.f)
{
}

CBlood::CBlood(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pTexture(nullptr), m_pBuffer(nullptr)
, m_fGravityTime(0.f)
{
}

CBlood::CBlood(const CBlood& rhs) : CGameObject(rhs), m_pTexture(rhs.m_pTexture), m_pBuffer(rhs.m_pBuffer)
, m_fGravityTime(rhs.m_fGravityTime)
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
	m_pTransform->setScale(0.3f, 0.3f, 0.3f);
	Rotate();
	DownMove(fDeltaTime);
	m_pTransform->setPos(m_pTransform->getPos());
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
	m_fGravityTime = 0.f;
}

void CBlood::TerrainCheck()
{
	_vec3 vPivot = m_pTransform->getPos();
	vPivot.y -= m_pTransform->getScale().y * 0.5f;
	if (vPivot.y < m_pTransform->getBottomY())
		setActive(false);
}

void CBlood::Rotate()
{
	_matrix matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransform->setRotate(matBill);
}

void CBlood::DownMove(const _float& fDeltaTime)
{
	m_fGravityTime += fDeltaTime;
	_vec3 vPos = m_pTransform->getPos();
	_float fY = -(0.5f * 9.8f * m_fGravityTime * m_fGravityTime);
	vPos.y += fY;
	m_pTransform->setPos(vPos);
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
