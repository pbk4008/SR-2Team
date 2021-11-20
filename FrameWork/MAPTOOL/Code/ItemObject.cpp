#include "pch.h"
#include "ItemObject.h"

CItemObject::CItemObject()
	: mpCollider(nullptr)
	, muiItemPower(0)
	, meType(eITEM::ITEMEND)
	, marrTexBuffer{ nullptr }
{
}

CItemObject::CItemObject(LPDIRECT3DDEVICE9 pDeivce)
	: CToolGameObject(pDeivce)
	, mpCollider(nullptr)
	, muiItemPower(0)
	, meType(eITEM::ITEMEND)
	, marrTexBuffer{ nullptr }
{

}

CItemObject::CItemObject(const CItemObject& rhs)
	: CToolGameObject(rhs)
	, mpCollider(Clone_ComProto<CCollision>(COMPONENTID::COLLISION))
	, muiItemPower(rhs.muiItemPower)
	, meType(rhs.meType)
{
	for (_uint texIndex = 0; texIndex < 6; ++texIndex)
	{
		marrTexBuffer[texIndex] = CRcTex::Create(m_pDevice,texIndex);
	}
	mpCollider->setRadius(0.8f);
	mpCollider->setTag(COLLISIONTAG::ETC);
	mpCollider->setActive(true);
	mpCollider->setTrigger(COLLISIONTRIGGER::INTERACT);
}
CItemObject::~CItemObject()
{

}

HRESULT CItemObject::Init_CItemObjectObject()
{
	m_vecTextureInfo.resize(6);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CItemObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	_vec3 vecAngle = m_pTransform->getAngle();
	vecAngle.y += fDeltaTime * 100.f;
	if (vecAngle.y >= 360.f)
	{
		vecAngle.y -= 360.f;
	}
	m_pTransform->setAngle(vecAngle);


	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CItemObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CItemObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	m_pDevice->SetMaterial(&mMaterial);


	for (_uint i = 0; i < m_vecTextureInfo.size(); ++i)
	{
		if (m_vecTextureInfo[i].pTexture)
		{
			m_vecTextureInfo[i].pTexture->Render_Texture();
		}
		marrTexBuffer[i]->Render_Buffer();
		m_pDevice->SetTexture(0, nullptr);
	}

	mpCollider->Render_Collision();
	
	CGameObject::Render_GameObject();
}

Engine::CGameObject* CItemObject::Clone_GameObject()
{
	return new CItemObject(*this);

}

CItemObject* CItemObject::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItemObject* pInstance = new CItemObject(pDevice);
	if (FAILED(pInstance->Init_CItemObjectObject()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT CItemObject::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pComponent = nullptr;


	for (_uint i = 0; i < 6; ++i)
	{
		pComponent = marrTexBuffer[i] = CRcTex::Create(m_pDevice, i);
		m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);
	}

	mpCollider = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	//Load할때 따로 지정해줌
	mpCollider->setRadius(0.8f);
	mpCollider->setTag(COLLISIONTAG::ETC);
	mpCollider->setActive(true);
	mpCollider->setTrigger(COLLISIONTRIGGER::INTERACT);
	mpCollider->setTransform(m_pTransform);

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, mpCollider);


	return S_OK;
}

void CItemObject::Free()
{
	Safe_Release(mpCollider);

	for (auto& texBuffer : marrTexBuffer)
		Safe_Release(texBuffer);

	CToolGameObject::Free();
	CGameObject::Free();
}

