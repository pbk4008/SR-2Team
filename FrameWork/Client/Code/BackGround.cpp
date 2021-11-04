#include "pch.h"
#include "BackGround.h"
#include "Transform.h"

CBackGround::CBackGround() : m_pBufferCom(nullptr), m_pTexture(nullptr), m_eSceneID(SCENEID::STAGE_END)
{
}

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), m_eSceneID(SCENEID::STAGE_END)
{
}

CBackGround::CBackGround(const CBackGround& rhs) : CGameObject(rhs), m_eSceneID(rhs.m_eSceneID),m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture)
{
}

CBackGround::~CBackGround()
{
}
HRESULT CBackGround::Init_BackGround(SCENEID eID)
{
	m_eSceneID = eID;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	iExit=CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CBackGround::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBackGround::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
}

CGameObject* CBackGround::Clone_GameObject()
{
	return new CBackGround(*this);
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pDevice,SCENEID eID)
{
	CBackGround* pInstance = new CBackGround(pDevice);
	if (FAILED(pInstance->Init_BackGround(eID)))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CBackGround::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);

	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::BACKGROUND_TEX);
	m_pTexture->AddRef();
	m_mapComponent->emplace(COMPONENTID::BACKGROUND_TEX, pComponent);

	return S_OK;
}

void CBackGround::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}

void CBackGround::setTextureCom(SCENEID eID)
{
	//ToDo:스테이지 BackGrond 셋팅
	m_eSceneID = eID;
	switch (m_eSceneID)
	{
	case SCENEID::STAGE_ONE:
		m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::BACKGROUND_TEX);
		break;
	case SCENEID::STAGE_TWO:
		break;
	default:
		break;
	}
}
