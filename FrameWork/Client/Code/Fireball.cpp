#include "pch.h"
#include "Fireball.h"
#include "Fireball_Anim.h"

CFireball::CFireball() : m_pAnimation(nullptr), m_fSpeed(0.f), m_pCollision(nullptr), m_bReflect(false)
{
	ZeroMemory(&m_vFirstPos, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));


}

CFireball::CFireball(LPDIRECT3DDEVICE9 pDevice) : CBullet(pDevice), m_pAnimation(nullptr), m_pCollision(nullptr)
, m_fSpeed(0.f), m_bReflect(false)
{
	ZeroMemory(&m_vFirstPos, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));

}

CFireball::CFireball(const CFireball& rhs) : CBullet(rhs), m_pAnimation(nullptr),
m_fSpeed(rhs.m_fSpeed), m_pCollision(nullptr), m_bReflect(rhs.m_bReflect)
{
	Add_Component();
	m_pCollision->setTransform(m_pTransform);

}

CFireball::~CFireball()
{
}

HRESULT CFireball::Init_Fireball()
{
	CBullet::Add_Component();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 10.f;
	return S_OK;
}

_int CFireball::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;

	m_pTransform->setScale(1.f, 1.f, 1.f);


	
	_matrix matRot;
	matRot = *ComputeLookAtTarget();
	m_pTransform->setRotate(matRot);


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
	if (m_bReflect)
		m_pCollision->Collison(COLLISIONTAG::MONSTER);

	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CFireball::LateUpdate_GameObject()
{
	CBullet::LateUpdate_GameObject();
	if (m_pCollision->getHit())
	{
		if (m_pCollision->getCollider()->getTag() == COLLISIONTAG::PLAYER)
		{
			if (m_pCollision->getCollider()->getTrigger() == COLLISIONTRIGGER::ATTACK)
			{
				cout << "Reflect!!" << endl;
				m_bReflect = true;
			}
		}
		else
		{
			if (m_bReflect)
			{
				m_bReflect = false;
				//m_fSpeed = 2.5f;
				setActive(false);
			}
		}
		m_pCollision->ResetCollision();
	}
}

void CFireball::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CBullet::Render_GameObject();

	m_pAnimation->Render_Animation();
	m_pBuffer->Render_Buffer();
	m_pCollision->Render_Collision();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CFireball::Clone_GameObject()
{
	return new CFireball(*this);
}

void CFireball::Move(const _float& fDeltaTime)
{
	_vec3 i = *D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_vFirstPos += i * m_fSpeed * fDeltaTime;

	m_pTransform->setPos(m_vFirstPos);
}

HRESULT CFireball::Add_Component()
{
	m_pAnimation = CFireball_Anim::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pAnimation, E_FAIL);
	m_pAnimation->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATION, m_pAnimation);

	m_pCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pCollision->setRadius(1.f);
	m_pCollision->setActive(true);
	m_pCollision->setTag(COLLISIONTAG::BULLET);
	m_pCollision->setTrigger(COLLISIONTRIGGER::INTERACT);
	m_pCollision->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, m_pCollision);

	Insert_Collision(m_pCollision);

	return S_OK;
}

void CFireball::Free()
{
	CBullet::Free();
	Safe_Release(m_pCollision);
	Safe_Release(m_pAnimation);
}

void CFireball::setPos(const _vec3& vPos)
{
	m_vFirstPos = vPos;
}

void CFireball::setLook(const _vec3& vLook)
{
	m_vLook = vLook;
}

void CFireball::setTarget(const _vec3& vTarget)
{
	m_vTargetPos = vTarget;
}

CFireball* CFireball::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFireball* pInstance = new CFireball(pDevice);
	if (FAILED(pInstance->Init_Fireball()))
		Safe_Release(pInstance);
	return pInstance;
}

_matrix* CFireball::ComputeLookAtTarget()
{
	_matrix matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	return &matBill;
}