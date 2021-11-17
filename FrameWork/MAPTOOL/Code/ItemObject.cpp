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
	, mpCollider(rhs.mpCollider)
	, muiItemPower(rhs.muiItemPower)
	, meType(rhs.meType)
{
	for (_uint texIndex = 0; texIndex < 6; ++texIndex)
	{
		marrTexBuffer[texIndex] = CRcTex::Create(m_pDevice,texIndex);
	}
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

	for (_uint i = 0; i < m_vecTextureInfo.size(); ++i)
	{
		if (m_vecTextureInfo[i].pTexture)
		{
			m_vecTextureInfo[i].pTexture->Render_Texture();
		}
		else
		{
			m_pDevice->SetTexture(0, nullptr);
		}

		marrTexBuffer[i]->Render_Buffer();
	}

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
	mpCollider->setRadius(1.f);
	mpCollider->setTag(COLLISIONTAG::ETC);
	mpCollider->setActive(true);
	mpCollider->setTrigger(COLLISIONTRIGGER::INTERACT);

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

