#include "pch.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Animator.h"

CPlayer::CPlayer() : m_pMainCamera(nullptr), m_pModel(nullptr) , m_eCulState(STATE::MAX),
m_ePreState(STATE::MAX),m_fSpeed(0.f),m_pHitCollision(nullptr),m_pAtkCollision(nullptr)
, m_bAttack(false), m_fAngle(0.f),m_bJump(false)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice): CGameObject(pDevice), m_pMainCamera(nullptr), m_pModel(nullptr),
m_fSpeed(0.f),m_eCulState(STATE::MAX),m_ePreState(STATE::MAX), m_pHitCollision(nullptr), m_pAtkCollision(nullptr)
, m_bAttack(false), m_fAngle(0.f),m_bJump(false)
{
}

CPlayer::CPlayer(const CPlayer& rhs) : CGameObject(rhs), m_pMainCamera(rhs.m_pMainCamera), m_pModel(rhs.m_pModel),
m_fSpeed(rhs.m_fSpeed), m_eCulState(rhs.m_eCulState),m_ePreState(rhs.m_ePreState)
, m_pHitCollision(rhs.m_pHitCollision),m_pAtkCollision(rhs.m_pAtkCollision)
, m_bAttack(rhs.m_bAttack), m_fAngle(rhs.m_fAngle), m_bJump(rhs.m_bJump)
{
	if (rhs.m_pModel)
		m_pModel->AddRef();
	if(rhs.m_pMainCamera)
		m_pMainCamera->AddRef();
	m_pHitCollision->AddRef();
	m_pHitCollision->setTransform(m_pTransform);
	m_pAtkCollision->AddRef();
	m_pAtkCollision->setTransform(m_pTransform);

	m_pTransform->setPos(0.f, 100.f, 0.f);
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Init_Player()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 8.f;
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	KeyInput(fDeltaTime);
	m_pTransform->setScale(0.8f, 0.5f, 0.8f);
	if (!m_bJump)
		m_pTransform->UsingGravity(fDeltaTime);
	else
		m_pTransform->Jump(fDeltaTime, 4.f,m_bJump);
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	ChangeState();
	m_pMainCamera->Update_GameObject(fDeltaTime);
	m_pModel->Update_GameObject(fDeltaTime);
	m_eCulState=m_pModel->Act();

	if (m_pAtkCollision->getActive())
	{
		m_pAtkCollision->Update_Component(fDeltaTime);
		m_pAtkCollision->Collison(COLLISIONTAG::MONSTER);
	}
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CPlayer::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pHitCollision->getHit())
	{
		//충돌 이후 작업
		cout << "Player 충돌!" << endl;
		//m_pCollision->setHit(false);
		m_pHitCollision->setHit(false);
	}
	if (m_pAtkCollision->getHit())
	{
		m_pAtkCollision->setHit(false);
		m_pAtkCollision->setActive(false);
	}
}

void CPlayer::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pHitCollision->Render_Collision();
	m_pModel->Render_GameObject();

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CGameObject::Render_GameObject();
}

CGameObject* CPlayer::Clone_GameObject()
{
	return new CPlayer(*this);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->Init_Player()))
		Safe_Release(pInstance);
	return pInstance;
}

void CPlayer::KeyInput(const float& fDeltaTime)
{
	_vec3 vLook = *m_pTransform->getAxis(VECAXIS::AXIS_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = *m_pTransform->getAxis(VECAXIS::AXIS_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vPos = m_pTransform->getPos();

	
	if (Key_Pressing(VIR_W))
	{
		vPos += vLook * m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (Key_Pressing(VIR_A))
	{
		vPos += vRight * -m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (Key_Pressing(VIR_S))
	{
		vPos += vLook * -m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (Key_Pressing(VIR_D))
	{
		vPos += vRight * m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (!m_bJump)
	{
		if (Key_Down(VIR_SPACE))
		{
			m_bJump = true;
		}
	}

	if (Key_Down(VIR_LBUTTON))
		m_eCulState = STATE::ATTACK;

	if(Key_Up(VIR_W)|| Key_Up(VIR_A)|| Key_Up(VIR_S)|| Key_Up(VIR_D))
		m_eCulState = STATE::IDLE;

	_vec3 vMousDir = MousePos(g_hWnd);
	if (vMousDir.x < 0.f)
		m_fAngle += -0.1f;
	else if (vMousDir.x > 0.f)
		m_fAngle += +0.1f;

	m_pTransform->setAngle(MATRIXINFO::MAT_UP, m_fAngle);
	m_pTransform->setPos(vPos);
}

void CPlayer::ChangeState()
{
	if (m_eCulState != m_ePreState)
	{
		switch (m_eCulState)
		{
		case STATE::IDLE:
			m_pModel->setState(m_eCulState);
			m_bAttack = false;
			break;
		case STATE::ATTACK:
			m_pModel->setState(m_eCulState);
			if (!m_bAttack)
			{
				m_bAttack = true;
				m_pAtkCollision->setActive(true);
			}
			break;
		case STATE::WALK:
			m_pModel->setState(m_eCulState);
		}
		m_ePreState = m_eCulState;
	}
}

HRESULT CPlayer::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	m_pHitCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pHitCollision->setRadius(1.f);
	m_pHitCollision->setTag(COLLISIONTAG::PLAYER);
	m_pHitCollision->setActive(true);
	m_pHitCollision->setTrigger(COLLISIONTRIGGER::HIT);
	m_pHitCollision->AddRef();
	pCom = m_pHitCollision;
	Insert_Collision(m_pHitCollision);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, pCom);
	
	m_pAtkCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pAtkCollision->setRadius(1.f);
	m_pAtkCollision->setTag(COLLISIONTAG::PLAYER);
	m_pAtkCollision->setActive(false);
	m_pAtkCollision->setTrigger(COLLISIONTRIGGER::ATTACK);
	Insert_Collision(m_pAtkCollision);
	return S_OK;
}

void CPlayer::Free()
{
	if(!m_bClone)
		ClearCollisionList();
	Safe_Release(m_pHitCollision);
	Safe_Release(m_pAtkCollision);
	Safe_Release(m_pMainCamera);
	Safe_Release(m_pModel);
	CGameObject::Free();
}

void CPlayer::setCamera(CMainCamera* pCamera)
{
	m_pMainCamera = pCamera;
	m_pMainCamera->setTarget(this->getTransform());
}

void CPlayer::setModel(CPlayerModel* pModel)
{
	m_pModel = pModel;
	m_pModel->setTarget(this->getTransform());
	m_pModel->SettingAnimator();
}
