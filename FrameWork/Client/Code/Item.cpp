#include "pch.h"
#include "../Client/Code/Item.h"

CItem::CItem()
	: mItemPower(0)
	, mpCollider(nullptr)
{
	m_ItemTexture.reserve(6);
	m_ItemPlane.reserve(6);
}

CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, mItemPower(0)
	, mpCollider(nullptr)
{
	m_ItemTexture.reserve(6);
	m_ItemPlane.reserve(6);
}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
	, mItemPower(rhs.mItemPower)
	, mpCollider(rhs.mpCollider)
{
	mpCollider->AddRef();
	mpCollider->setTransform(m_pTransform);
	Insert_Collision(mpCollider);

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, mpCollider);


	m_ItemPlane.reserve(6);
	for (_int i = 0; i < 6; ++i)
	{
		CRcTex* pBuffer = CRcTex::Create(m_pDevice, i);
		m_ItemPlane.emplace_back(pBuffer);
		CTexture* pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
		m_ItemTexture.emplace_back(pTexture);
		pTexture->AddRef();
	}

}

CItem::~CItem()
{

}

HRESULT CItem::Init_CItem()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CItem::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->setScale(m_pTransform->getScale());
	// 뺑글뻉글 돌아가는 표현하고싶으면 여기를 바꾸면됨
	m_pTransform->setAngle(m_pTransform->getAngle());
	m_pTransform->setPos(m_pTransform->getPos());
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CItem::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CItem::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	mpCollider->Render_Collision();


	for (_int i = 0; i < 6; i++)
	{
		m_ItemTexture[i]->Render_Texture();
		m_ItemPlane[i]->Render_Buffer();
	}
	CGameObject::Render_GameObject();
}

CGameObject* CItem::Clone_GameObject()
{
	return new CItem(*this);
}

void CItem::ResetObject()
{

}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem* pInstance = new CItem(pDevice);
	if (FAILED(pInstance->Init_CItem()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CItem::Add_Component()
{
	CGameObject::Add_Component();

	mpCollider = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	mpCollider->setRadius(1.f);
	mpCollider->setTag(COLLISIONTAG::ETC);
	mpCollider->setActive(true);
	mpCollider->setTrigger(COLLISIONTRIGGER::INTERACT);
	mpCollider->AddRef();
	

	return S_OK;
}

void CItem::Free()
{
	for_each(m_ItemPlane.begin(), m_ItemPlane.end(), DeleteObj);
	m_ItemPlane.clear();
	m_ItemPlane.shrink_to_fit();

	for_each(m_ItemTexture.begin(), m_ItemTexture.end(), DeleteObj);
	m_ItemTexture.clear();
	m_ItemTexture.shrink_to_fit();

	Safe_Release(mpCollider);

	CGameObject::Free();
}

void CItem::setTexture(const _tchar* pTextureName, const _int iIndex)
{
	m_ItemTexture[iIndex]->setTexture(GetTexture(pTextureName, TEXTURETYPE::TEX_NORMAL));

}
