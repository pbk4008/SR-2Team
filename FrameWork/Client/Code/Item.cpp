#include "pch.h"
#include "../Client/Code/Item.h"

CItem::CItem()
	: mItemPower(0)
	, mpCollider(nullptr)
{
}

CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, mItemPower(0)
	, mpCollider(nullptr)
{

}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
	, mItemPower(rhs.mItemPower)
	, mpCollider(nullptr)
	, mItemTexture(nullptr)
{
	Add_Component();
	mpCollider->AddRef();
	mpCollider->setTransform(m_pTransform);
	Insert_Collision(mpCollider);

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, mpCollider);
	for (_uint i = 0; i < 6; ++i)
	{
		CRcTex* pBuffer = CRcTex::Create(m_pDevice, i);
		m_ItemPlane.emplace_back(pBuffer);
		CTexture* pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
		m_ItemTexture.emplace_back(pTexture);
	}
}

CItem::~CItem()
{

}

HRESULT CItem::Init_CItem()
{
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

	mItemTexture->Render_Texture();
	for (_int i = 0; i < 6; i++)
	{
		mItemPlane[i]->Render_Buffer();
	}
	m_pDevice->SetTexture(0, nullptr);

	mpCollider->Render_Collision();

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
	

	CComponent* pComponent = nullptr;

	for (_uint i = 0; i < 6; ++i)
	{
		pComponent = mItemPlane[i] = CRcTex::Create(m_pDevice, i);
		m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::RCTEX, pComponent);
		pComponent = mItemTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
		m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::TEXTURE, pComponent);
	}

	mpCollider = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	mpCollider->setRadius(1.f);
	mpCollider->setTag(COLLISIONTAG::ETC);
	mpCollider->setActive(true);
	mpCollider->setTrigger(COLLISIONTRIGGER::INTERACT);
	mpCollider->setTransform(m_pTransform);

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, mpCollider);

	return S_OK;
}

void CItem::Free()
{
	CGameObject::Free();

	for_each(m_ItemPlane.begin(), m_ItemPlane.end(), DeleteObj);
	m_ItemPlane.clear();
	m_ItemPlane.shrink_to_fit();

	Safe_Release(mItemTexture);

	Safe_Release(mpCollider);

}

void CItem::setTexture(const _tchar* pTextureName)
{
	mItemTexture->setTexture(GetTexture(pTextureName, TEXTURETYPE::TEX_NORMAL));

}
