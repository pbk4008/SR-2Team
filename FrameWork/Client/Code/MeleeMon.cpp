#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"
#include "Player.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(1), m_pCollision(nullptr)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(1),m_pCollision(nullptr)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture),
	 m_fSpeed(rhs.m_fSpeed), m_bAttack(false), m_iTimer(1),m_pCollision(rhs.m_pCollision)
{
	m_pCollision->AddRef();
	m_pCollision->setTransform(m_pTransform);
}

CMeleeMon::~CMeleeMon()
{

}

HRESULT CMeleeMon::Init_MeleeMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;

	m_fSpeed = 2.f;
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	//Follow(fDeltaTime);

	//CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	//_vec3 vPos = pObject->getTransform()->getPos();

	//_vec3  vDis = m_vInfo - vPos;

	//_float fDis = D3DXVec3Length(&vDis);

	//if (fDis <= 1.0f)
	//{
	//	//m_bAttack = true;
	//	Attack(fDeltaTime);

	//}

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CMeleeMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pCollision->getHit())
	{
		cout << "¸ó½ºÅÍ Ãæµ¹" << endl;
		m_pCollision->setHit(false);
	}
}

void CMeleeMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pCollision->Render_Collision();

	m_pTexture->Render_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
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

void CMeleeMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target(&playerPos, m_fSpeed, fDeltaTime);
}

void CMeleeMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 1.0f)
	{
		cout << "Atack" << endl;
		m_bAttack = false;
		m_iTimer = 0.f;
	}
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//texture
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_pTexture->setTexture(GetTexture(L"Monster", TEXTURETYPE::TEX_NORMAL));
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, pComponent);

	m_pCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pCollision->setRadius(1.f);
	m_pCollision->setTag(COLLISIONTAG::MONSTER);
	m_pCollision->setActive(true);
	m_pCollision->setTrigger(COLLISIONTRIGGER::HIT);
	pComponent = m_pCollision;
	Insert_Collision(m_pCollision);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, pComponent);


	return S_OK;
}

void CMeleeMon::Free()
{
	Safe_Release(m_pCollision);
	ClearCollisionList();
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
