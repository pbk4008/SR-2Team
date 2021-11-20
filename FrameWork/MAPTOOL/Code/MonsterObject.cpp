#include "pch.h"
#include "MonsterObject.h"

CMonsterObject::CMonsterObject()
	: mTexBuffer{ nullptr }
{
}

CMonsterObject::CMonsterObject(LPDIRECT3DDEVICE9 pDeivce)
	: CToolGameObject(pDeivce)
	, mTexBuffer{ nullptr }
{

}

CMonsterObject::CMonsterObject(const CMonsterObject& rhs)
	: CToolGameObject(rhs)
	, meMonsterType(rhs.meMonsterType)
{
	mTexBuffer = CRcTex::Create(m_pDevice);
}
CMonsterObject::~CMonsterObject()
{

}

HRESULT CMonsterObject::Init_CMonsterObjectObject()
{
	m_tVTXINFO.X = 2;
	m_tVTXINFO.Z = 2;
	m_tVTXINFO.Detail = 1;
	m_tVTXINFO.Interval = 1;
	m_vecTextureInfo.resize(1);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CMonsterObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CMonsterObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMonsterObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	m_pDevice->SetMaterial(&mMaterial);



	if (!m_vecTextureInfo.empty() &&
		m_vecTextureInfo.front().pTexture)
	{
		m_vecTextureInfo.front().pTexture->Render_Texture();
	}
	else
	{
		m_pDevice->SetTexture(0, nullptr);
	}

	mTexBuffer->Render_Buffer();

	
	CGameObject::Render_GameObject();
}

Engine::CGameObject* CMonsterObject::Clone_GameObject()
{
	return new CMonsterObject(*this);

}

CMonsterObject* CMonsterObject::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterObject* pInstance = new CMonsterObject(pDevice);
	if (FAILED(pInstance->Init_CMonsterObjectObject()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT CMonsterObject::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pComponent = nullptr;


	pComponent = mTexBuffer= CRcTex::Create(m_pDevice);
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);


	return S_OK;
}

void CMonsterObject::Free()
{
	Safe_Release(mTexBuffer);

	CToolGameObject::Free();
	CGameObject::Free();
}

