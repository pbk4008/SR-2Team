#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), 
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_TEX)),
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f)
{

}

CMeleeMon::~CMeleeMon()
{

}

HRESULT CMeleeMon::Init_MeleeMon()
{
	m_fXPos = 1.f;
	m_fYPos = 1.f;
	m_fZPos = 1.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->setScale(1.f, 1.f, 0.f);
	m_pTransform->setPos(m_fXPos, m_fYPos, m_fZPos);

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	Key_Input();
	//Follow_Mouse();
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CMeleeMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMeleeMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CMeleeMon::Follow_Mouse()
{
	/*LPPOINT pCursor;
	GetCursorPos(&pCursor[0]);
	ScreenToClient(g_hWnd, &pCursor[0]);*/


}

void CMeleeMon::Key_Input()
{
	m_pTransform->getPos();

	if (GetAsyncKeyState(VK_UP) & 0x80000)
		m_pTransform->setPos(m_fXPos, --m_fYPos, m_fZPos);

	if (GetAsyncKeyState(VK_DOWN) & 0x80000)
		m_pTransform->setPos(m_fXPos, ++m_fYPos, m_fZPos);
}

Engine::CGameObject* CMeleeMon::Clone_GameObject()
{
	return new CMeleeMon(*this);
}

CMeleeMon* CMeleeMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon* pInstance = new CMeleeMon(pDevice);

	if (FAILED(pInstance->Init_MeleeMon()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	//¹ö¼hÄÞ
	//texture
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);
	//texture
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_TEX);
	m_pTexture->AddRef();
	m_mapComponent->emplace(COMPONENTID::MELEEMON_TEX, pComponent);

	////transform
	//pComponent = m_pTransform = Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent->emplace(COMPONENTID::TRANSFORM, pComponent);

	return S_OK;
}

void CMeleeMon::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
