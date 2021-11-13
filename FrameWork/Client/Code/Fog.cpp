#include "pch.h"
#include "Fog.h"
#include "FogAnim.h"

CFog::CFog() : m_pAnimation(nullptr),m_pCollision(nullptr), m_pFrontBuffer(nullptr), m_pSideBuffer(nullptr),m_fDestroyTime(0.f)
, m_pSideTransform(nullptr)
{
	ZeroMemory(&m_vFirstPos, sizeof(_vec3));
}

CFog::CFog(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pAnimation(nullptr), m_pSideBuffer(nullptr),m_pFrontBuffer(nullptr)
, m_fDestroyTime(0.f), m_pSideTransform(nullptr)
{
	ZeroMemory(&m_vFirstPos, sizeof(_vec3));
}

CFog::CFog(const CFog& rhs) : CGameObject(rhs), m_pAnimation(nullptr),m_pCollision(nullptr), m_vFirstPos(rhs.m_vFirstPos), m_pFrontBuffer(rhs.m_pFrontBuffer), m_pSideBuffer(rhs.m_pSideBuffer)
, m_fDestroyTime(rhs.m_fDestroyTime), m_pSideTransform(nullptr)
{
	m_pSideBuffer->AddRef();
	m_pFrontBuffer->AddRef();
	Add_Component();
	Insert_Collision(m_pCollision);
}

CFog::~CFog()
{
}

HRESULT CFog::Init_Fog()
{
	CGameObject::Add_Component();

	m_pFrontBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pFrontBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, m_pFrontBuffer);

	m_pSideBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pSideBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, m_pSideBuffer);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CFog::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->setScale(5.f, 5.f, 5.f);
	m_pTransform->setPos(m_vFirstPos);
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	cout << m_vFirstPos.x <<" " << m_vFirstPos.y <<" " << m_vFirstPos.z << endl;
	m_pCollision->Collison(COLLISIONTAG::PLAYER);
	m_fDestroyTime += fDeltaTime;
	setSideMatrix();

	if (m_fDestroyTime > 5.f)
	{
		m_fDestroyTime = 0.f;
		setActive(false);
	}

	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CFog::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pCollision->getHit())
	{
		cout << "Ãæµ¹!!" << endl;
		m_pCollision->ResetCollision();
	}
}

void CFog::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	CGameObject::Render_GameObject();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pAnimation->Render_Animation();
	m_pFrontBuffer->Render_Buffer();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pSideTransform->getWorldMatrix());
	m_pSideBuffer->Render_Buffer();
	m_pCollision->Render_Collision();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CFog::Clone_GameObject()
{
	return new CFog(*this);
}

void CFog::ResetObject()
{
}

void CFog::setSideMatrix()
{
	m_pSideTransform->setScale(5.f, 5.f, 5.f);
	m_pSideTransform->setAngle(MATRIXINFO::MAT_UP, 90);
	m_pSideTransform->setPos(m_vFirstPos);
	m_pSideTransform->Update_Component(0.f);
}

CFog* CFog::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFog* pInstance = new CFog(pDevice);
	if (FAILED(pInstance->Init_Fog()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CFog::Add_Component()
{
	m_pSideTransform = Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM);

	m_pCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pCollision->AddRef();
	m_pCollision->setTransform(m_pTransform);
	m_pCollision->setTag(COLLISIONTAG::ETC);
	m_pCollision->setTrigger(COLLISIONTRIGGER::INTERACT);
	m_pCollision->setRadius(3.f);
	m_pCollision->setActive(true);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, m_pCollision);

	m_pAnimation = CFogAnim::Create(m_pDevice);
	m_pAnimation->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, m_pAnimation);
	
	return S_OK;
}

void CFog::Free()
{
	if (!m_bClone)
		ClearCollisionList();

	Safe_Release(m_pSideTransform);
	Safe_Release(m_pAnimation);
	Safe_Release(m_pCollision);
	Safe_Release(m_pFrontBuffer);
	Safe_Release(m_pSideBuffer);
	CGameObject::Free();
}