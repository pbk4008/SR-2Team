#include "pch.h"
#include "Potal.h"
#include "PotalAnim.h"

CPotal::CPotal():m_pAnimation(nullptr), m_pBuffer(nullptr), m_pInteract(nullptr), m_bClear(false)
{
}

CPotal::CPotal(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBuffer(nullptr), m_pAnimation(nullptr),m_pInteract(nullptr), m_bClear(false)
{
}

CPotal::CPotal(const CPotal& rhs):CGameObject(rhs), m_pBuffer(rhs.m_pBuffer), m_pAnimation(nullptr), m_pInteract(nullptr),m_bClear(rhs.m_bClear)
{
	m_pBuffer->AddRef();
	setAnimation();
	setCollision();
}

CPotal::~CPotal()
{
}

HRESULT CPotal::Init_Potal()
{
	Add_Component();
	return S_OK;
}

_int CPotal::Update_GameObject(const _float& fDeltaTime)
{

	m_pTransform->setScale(m_pTransform->getScale());
	m_pTransform->setAngle(m_pTransform->getAngle());
	m_pTransform->setPos(m_pTransform->getPos());

	_int iExit = CGameObject::Update_GameObject(fDeltaTime);

	m_pInteract->Collison(COLLISIONTAG::PLAYER);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);

	return iExit;
}

void CPotal::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pInteract->getHit())
	{
		m_bClear = true;
		m_pInteract->ResetCollision();
	}
}

void CPotal::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CGameObject::Render_GameObject();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pAnimation->Render_Animation();
	m_pBuffer->Render_Buffer();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CPotal::Clone_GameObject()
{
	return new CPotal(*this);
}

void CPotal::setTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3& vPosition)
{
	m_pTransform->setScale(vScale);
	m_pTransform->setAngle(vRotate);
	m_pTransform->setPos(vPosition);
	m_pInteract->setCenter(m_pTransform->getPos());
}

void CPotal::ResetObject()
{
}

void CPotal::setAnimation()
{
	m_pAnimation = CPotalAnim::Create(m_pDevice);
	m_pAnimation->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATION, m_pAnimation);
}

void CPotal::setCollision()
{
	m_pInteract = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pInteract->setRadius(2.f);
	m_pInteract->setActive(true);
	m_pInteract->setTag(COLLISIONTAG::ETC);
	m_pInteract->setTrigger(COLLISIONTRIGGER::INTERACT);
	m_pInteract->setTransform(m_pTransform);
	Insert_ObjCollision(m_pInteract);
}

CPotal* CPotal::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPotal* pInstance = new CPotal(pDevice);
	if (FAILED(pInstance->Init_Potal()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CPotal::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pCom = nullptr;

	pCom = m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);

	return S_OK;
}

void CPotal::Free()
{
	CGameObject::Free();
	Safe_Release(m_pInteract);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pAnimation);
}
