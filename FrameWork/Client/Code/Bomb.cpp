#include "pch.h"
#include "Bomb.h"
#include "BombAnim.h"
CBomb::CBomb() :  m_pAnimation(nullptr), m_fSpeed(0.f), m_bJump(false)
{
}

CBomb::CBomb(LPDIRECT3DDEVICE9 pDevice) : CBullet(pDevice), m_pAnimation(nullptr), m_fSpeed(0.f)
, m_bJump(false)
{

}

CBomb::CBomb(const CBomb& rhs) : CBullet(rhs),  m_pAnimation(nullptr), m_fSpeed(rhs.m_fSpeed)
, m_bJump(rhs.m_bJump)
{
	Add_Component();
}

CBomb::~CBomb()
{
}

HRESULT CBomb::Init_Bomb()
{
	CBullet::Add_Component();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fDestroyTime = 0.f;
	m_fSpeed = 30.f;
	m_bJump = true;
	return S_OK;
}

_int CBomb::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	m_pTransform->setAngle(MATRIXINFO::MAT_UP, m_fAngle);
	if(m_bJump)
		Move(fDeltaTime);
	iExit=CBullet::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CBomb::LateUpdate_GameObject()
{
	CBullet::LateUpdate_GameObject();
	if (!m_bJump)
	{
		setActive(false);
	}
}

void CBomb::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CBullet::Render_GameObject();
	//Animation
	m_pAnimation->Render_Animation();
	m_pBuffer->Render_Buffer();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CBomb::Clone_GameObject()
{
	return new CBomb(*this);
}

void CBomb::ResetObject()
{
	m_bJump = true;
}

void CBomb::Move(const _float& fDeltaTime)
{
	_vec3 vPos = m_pTransform->getPos();
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	vPos += m_vLook * m_fSpeed * fDeltaTime;
	m_pTransform->setPos(vPos);
	
	m_pTransform->Jump(fDeltaTime, 3.f, m_bJump,false);
}

HRESULT CBomb::Add_Component()
{
	m_pAnimation = CBombAnim::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pAnimation, E_FAIL);
	m_pAnimation->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATION, m_pAnimation);

	return S_OK;
}

void CBomb::Free()
{
	Safe_Release(m_pAnimation);
	CBullet::Free();
}

CBomb* CBomb::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBomb* pInstance = new CBomb(pDevice);
	if (FAILED(pInstance->Init_Bomb()))
		Safe_Release(pInstance);
	return pInstance;
}

void CBomb::setPos(const _vec3& vPos)
{
	m_pTransform->setPos(vPos);
	m_vFirstPos = vPos;
}
