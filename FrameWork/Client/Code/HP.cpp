#include "pch.h"
#include "HP.h"

CHP::CHP()
	:m_pTexture(nullptr), m_pBoss(nullptr), m_pBufferCom(nullptr)
{

}

CHP::CHP(LPDIRECT3DDEVICE9 pDevice)
	: m_pTexture(nullptr), m_pBoss(nullptr), m_pBufferCom(nullptr)
{

}

CHP::CHP(const CHP& rhs)
	: m_pTexture(rhs.m_pTexture), m_pBoss(rhs.m_pBoss), m_pBufferCom(rhs.m_pBufferCom)
{


	m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, m_pTexture);
	m_pTexture->AddRef();
	

	setTexture(L"Boss_HPFull");
}

CHP::~CHP()
{

}

HRESULT CHP::Init_CHP()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

Engine::_int CHP::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	m_pTransform->setPos(m_pTransform->getPos());
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CHP::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CHP::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();
	
	m_pDevice->SetTexture(0, nullptr);

	CGameObject::Render_GameObject();
}

void CHP::setLength(const _float& fLength)
{
	m_fLength = fLength;
}

Engine::CGameObject* CHP::Clone_GameObject()
{
	return new CHP(*this);
}

void CHP::ResetObject()
{

}

CHP* CHP::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHP* pInstance = new CHP(pDevice);
	if (FAILED(pInstance->Init_CHP()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CHP::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CHP::Free()
{
	CGameObject::Free();
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pBoss);
}

void CHP::setTexture(const _tchar* pTextureName)
{
	m_pTexture->setTexture(GetTexture(pTextureName, TEXTURETYPE::TEX_NORMAL));
}
