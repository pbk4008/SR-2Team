#include "pch.h"
#include "Item.h"
#include "Player.h"

CItem::CItem()
	: mItemPower(0)
	, mItemType(eITEM::ITEMEND)
	, mpCollider(nullptr)
	, mItemPlane(nullptr)
	, mItemTexture(nullptr)
{
}

CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, mItemType(eITEM::ITEMEND)
	, mItemPower(0)
	, mpCollider(nullptr)
	, mItemPlane(nullptr)
	, mItemTexture(nullptr)
{

}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
	, mItemType(eITEM::ITEMEND)
	, mItemPower(rhs.mItemPower)
	, mpCollider(nullptr)
	, mItemTexture(nullptr)
	, mItemPlane(rhs.mItemPlane)
{
	mpCollider = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	mpCollider->setRadius(1.f);
	mpCollider->setTag(COLLISIONTAG::ETC);
	mpCollider->setActive(true);
	mpCollider->setTrigger(COLLISIONTRIGGER::INTERACT);
	mpCollider->setTransform(m_pTransform);
	mpCollider->setTarget(this);
	mpCollider->AddRef();
	Insert_Collision(mpCollider);

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, mpCollider);

	mItemTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, mItemTexture);
	mItemTexture->AddRef();
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
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	m_pTransform->setScale(m_pTransform->getScale());
	// 뺑글뻉글 돌아가는 표현하고싶으면 여기를 바꾸면됨
	_vec3 vecAngle = m_pTransform->getAngle();
	vecAngle.y += fDeltaTime * 100.f;
	if (vecAngle.y >= 360.f)
	{
		vecAngle.y -= 360.f;
	}
	m_pTransform->setAngle(vecAngle);
	m_pTransform->setPos(m_pTransform->getPos());
	mpCollider->Collison(COLLISIONTAG::PLAYER);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CItem::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (mpCollider->getHit())
	{
		//충돌 이후 작업
		//cout << "Player 충돌!" << endl;		
		CCollision* pCollider = mpCollider->getCollider();//충돌한 대상 가져오기
		COLLISIONTAG eTag = pCollider->getTag();//충돌한 대상의 tag값

		CGameObject* Player = GetGameObject(GAMEOBJECTID::PLAYER);

		static_cast<CPlayer*>(Player)->PlusFormItem(mItemPower, mItemType);

		std::cout << std::get<0>(static_cast<CPlayer*>(Player)->getState()) << " " << std::get<1>(static_cast<CPlayer*>(Player)->getState()) << " " <<
			std::get<2>(static_cast<CPlayer*>(Player)->getState()) << std::endl;

		this->setActive(false);
	}
}

void CItem::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	if (mpCollider->getHit())
		std::cout << "아이템 충돌" << std::endl;

	mItemTexture->Render_Texture();
	for (_int i = 0; i < 6; i++)
	{
		mItemPlane->Render_Buffer(i);
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
	CGameObject::Add_Component();

	CComponent* pComponent = nullptr;

	pComponent = mItemPlane = CCubeTex::Create(m_pDevice);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::CUBETEX, pComponent);
	mItemPlane->AddRef();

	return S_OK;
}

void CItem::Free()
{
	CGameObject::Free();

	Safe_Release(mItemTexture);
	Safe_Release(mItemPlane);
	Safe_Release(mpCollider);
}

void CItem::setTexture(const _tchar* pTextureName)
{
	mItemTexture->setTexture(GetTexture(pTextureName, TEXTURETYPE::TEX_NORMAL));
}
