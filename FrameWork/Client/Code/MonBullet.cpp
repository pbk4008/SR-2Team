#include "pch.h"
#include "MonBullet.h"
#include "MonBulletAnim.h"

CMonBullet::CMonBullet() : m_pAnimation(nullptr), m_fSpeed(0.f), m_pCollision(nullptr)
{
	ZeroMemory(&m_vFirstPos, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));

	
}

CMonBullet::CMonBullet(LPDIRECT3DDEVICE9 pDevice) : CBullet(pDevice), m_pAnimation(nullptr), m_pCollision(nullptr)
, m_fSpeed(0.f)
{
	ZeroMemory(&m_vFirstPos, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));

}

CMonBullet::CMonBullet(const CMonBullet& rhs) : CBullet(rhs), m_pAnimation(nullptr), 
m_fSpeed(rhs.m_fSpeed), m_pCollision(nullptr)
{
	Add_Component();
	m_pCollision->setTransform(m_pTransform);

}

CMonBullet::~CMonBullet()
{
}

HRESULT CMonBullet::Init_MonBullet()
{
	CBullet::Add_Component();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 5.f;
	return S_OK;
}

_int CMonBullet::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;

	//m_pTransform->setAngle(MATRIXINFO::MAT_UP, 90.f);
	m_pTransform->setScale(0.1f, 0.1f, 0.1f);

	iExit = CBullet::Update_GameObject(fDeltaTime);

	Move(fDeltaTime);
	m_fDestroyTime += fDeltaTime;
	if (m_fDestroyTime > 3.f)
	{
		m_fDestroyTime = 0.f;
		setActive(false);
		return iExit;
	}

	m_pCollision->Collison(COLLISIONTAG::PLAYER);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);

	return iExit;
}

void CMonBullet::LateUpdate_GameObject()
{
	CBullet::LateUpdate_GameObject();
	if (m_pCollision->getHit())
	{
		m_pCollision->setHit(false);
		cout << "Player �浹" << endl;
		setActive(false);
	}
}

void CMonBullet::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CBullet::Render_GameObject();

	m_pAnimation->Render_Animation();
	m_pBuffer->Render_Buffer();
	m_pCollision->Render_Collision();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CMonBullet::Clone_GameObject()
{
	return new CMonBullet(*this);
}

void CMonBullet::Move(const _float& fDeltaTime)
{
	_vec3 i = *D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_vFirstPos += i * m_fSpeed * fDeltaTime;

	m_pTransform->setPos(m_vFirstPos);
}

HRESULT CMonBullet::Add_Component()
{
	m_pAnimation = CMonBulletAnim::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pAnimation, E_FAIL);
	m_pAnimation->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATION, m_pAnimation);

	m_pCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pCollision->setRadius(1.f);
	m_pCollision->setActive(true);
	m_pCollision->setTag(COLLISIONTAG::BULLET);
	m_pCollision->setTrigger(COLLISIONTRIGGER::INTERACT);
	m_pCollision->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, m_pCollision);

	Insert_Collision(m_pCollision);

	return S_OK;
}

void CMonBullet::Free()
{
	if (!m_bClone)
		ClearCollisionList();

	Safe_Release(m_pCollision);
	Safe_Release(m_pAnimation);
	CBullet::Free();
}

void CMonBullet::setPos(const _vec3& vPos)
{
	m_vFirstPos = vPos;
}

void CMonBullet::setLook(const _vec3& vLook)
{
	m_vLook = vLook;
}

void CMonBullet::setTarget(const _vec3& vTarget)
{
	m_vTargetPos = vTarget;
}

CMonBullet* CMonBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonBullet* pInstance = new CMonBullet(pDevice);
	if (FAILED(pInstance->Init_MonBullet()))
		Safe_Release(pInstance);
	return pInstance;
}