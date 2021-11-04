#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_pTransform(nullptr), m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice, GAMEOBJECTID eID)
	:CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), m_pTransform(nullptr), 
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_eGameObjectID(eID)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(Clone_ComProto<CTexture>(COMPONENTID::RCTEX)),
	m_pTransform(rhs.m_pTransform), m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f)
{

}

CMeleeMon::~CMeleeMon()
{

}

HRESULT CMeleeMon::Init_MeleeMon()
{
	m_fXPos = 2.f;
	m_fYPos = 2.f;
	m_fZPos = 2.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->setScale(2.f, 2.f, 0.f);
	m_pTransform->setPos(m_fXPos, m_fYPos, m_fZPos);

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	//Follow_Mouse();
	Key_Input();

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

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CMeleeMon::Follow_Mouse()
{
	LPPOINT pCursor;
	GetCursorPos(&pCursor[0]);
	ScreenToClient(g_hWnd, &pCursor[0]);


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

CMeleeMon* CMeleeMon::Create(LPDIRECT3DDEVICE9 pDevice, GAMEOBJECTID eID)
{
	CMeleeMon* pInstance = new CMeleeMon(pDevice, eID);

	if (FAILED(pInstance->Init_MeleeMon()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;


	//texture
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);

	//transform
	pComponent = m_pTransform = Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent->emplace(COMPONENTID::TRANSFORM, pComponent);

	return S_OK;
}



void CMeleeMon::Free()
{
	CGameObject::Free();
}
