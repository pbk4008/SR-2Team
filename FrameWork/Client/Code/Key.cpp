#include "pch.h"
#include "Key.h"

CKey::CKey() : m_pBuffer(nullptr), m_pTexture(nullptr), m_pInteract(nullptr), m_bUpDownCheck(false)
, m_fMoveTime(0.f)
{
}

CKey::CKey(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBuffer(nullptr), m_pTexture(nullptr), m_pInteract(nullptr)
, m_bUpDownCheck(false),m_fMoveTime(0.f)
{
}

CKey::CKey(const CKey& rhs):CGameObject(rhs), m_pBuffer(rhs.m_pBuffer), m_pTexture(rhs.m_pTexture), m_pInteract(nullptr)
,m_bUpDownCheck(rhs.m_bUpDownCheck), m_fMoveTime(rhs.m_fMoveTime)
{
	m_pBuffer->AddRef();
	m_pTexture->AddRef();

	setCollision();
}

CKey::~CKey()
{
}

HRESULT CKey::Init_Key()
{
	Add_Component();
	return S_OK;
}

_int CKey::Update_GameObject(const _float& fDeltaTime)
{
	UpDownMove(fDeltaTime);
	_int iExit = CGameObject::Update_GameObject(fDeltaTime);
	m_pInteract->Collison(COLLISIONTAG::PLAYER);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CKey::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pInteract->getHit())
	{
		setActive(false);
		m_pInteract->ResetCollision();
	}
}

void CKey::Render_GameObject()
{
	CGameObject::Render_GameObject();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
	m_pInteract->Render_Collision();
}

CGameObject* CKey::Clone_GameObject()
{
	return new CKey(*this);
}

void CKey::ResetObject()
{
}

void CKey::setCollision()
{
	m_pInteract = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pInteract->setTag(COLLISIONTAG::ETC);
	m_pInteract->setRadius(1.f);
	m_pInteract->setTrigger(COLLISIONTRIGGER::INTERACT);
	m_pInteract->setTransform(m_pTransform);
	m_pInteract->setTarget(this);
	m_pInteract->setActive(true);
	m_pInteract->AddRef();
	Insert_ObjCollision(m_pInteract);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, m_pInteract);
}

void CKey::UpDownMove(const _float& fDeltaTime)
{
	m_fMoveTime += fDeltaTime;
	_vec3 vPos = m_pTransform->getPos();
	_vec3 vUp = { 0.f,1.f,0.f };
	if (m_bUpDownCheck)
		vPos += vUp * fDeltaTime;
	else
		vPos -= vUp * fDeltaTime;
	if (m_fMoveTime > 1.f)
	{
		m_fMoveTime = 0.f;
		m_bUpDownCheck = !m_bUpDownCheck;
	}
	m_pTransform->setPos(vPos);
}

CKey* CKey::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CKey* pInstance = new CKey(pDevice);
	if (FAILED(pInstance->Init_Key()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CKey::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pCom = nullptr;

	pCom = m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);

	pCom = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_pTexture->setTexture(GetTexture(L"Key", TEXTURETYPE::TEX_NORMAL));
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, pCom);
	return S_OK;
}

void CKey::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pInteract);
}
