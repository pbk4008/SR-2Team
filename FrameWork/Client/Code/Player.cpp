#include "pch.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "Shuriken.h"
#include "Bomb.h"
CPlayer::CPlayer() : m_pMainCamera(nullptr), m_pModel(nullptr) , m_eCulState(STATE::MAX),
m_ePreState(STATE::MAX),m_fSpeed(0.f),m_pHitCollision(nullptr),m_pAtkCollision(nullptr)
, m_bAttack(false), m_fAngle(0.f),m_bJump(false), m_eCurType(ATTACKTYPE::SWORD),m_ePreType(ATTACKTYPE::SWORD)
, m_bHide(false), m_bDash(false)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice): CGameObject(pDevice), m_pMainCamera(nullptr), m_pModel(nullptr),
m_fSpeed(0.f),m_eCulState(STATE::MAX),m_ePreState(STATE::MAX), m_pHitCollision(nullptr), m_pAtkCollision(nullptr)
, m_bAttack(false), m_fAngle(0.f),m_bJump(false), m_eCurType(ATTACKTYPE::SWORD), m_ePreType(ATTACKTYPE::SWORD)
, m_bHide(false), m_bDash(false)
{
}

CPlayer::CPlayer(const CPlayer& rhs) : CGameObject(rhs), m_pMainCamera(rhs.m_pMainCamera), m_pModel(rhs.m_pModel),
m_fSpeed(rhs.m_fSpeed), m_eCulState(rhs.m_eCulState),m_ePreState(rhs.m_ePreState)
, m_pHitCollision(rhs.m_pHitCollision),m_pAtkCollision(rhs.m_pAtkCollision)
, m_bAttack(rhs.m_bAttack), m_fAngle(rhs.m_fAngle), m_bJump(rhs.m_bJump),
m_eCurType(rhs.m_eCurType),m_ePreType(rhs.m_ePreType), m_bHide(rhs.m_bHide), m_bDash(rhs.m_bDash)
{
	if (rhs.m_pModel)
		m_pModel->AddRef();
	if(rhs.m_pMainCamera)
		m_pMainCamera->AddRef();

	m_pHitCollision->AddRef();
	m_pHitCollision->setTransform(m_pTransform);
	m_pAtkCollision->AddRef();
	m_pAtkCollision->setTransform(m_pTransform);
	Insert_Collision(m_pHitCollision);
	Insert_Collision(m_pAtkCollision);

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, m_pHitCollision);
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
	if (m_bDash)
	{
		Dash(fDeltaTime);
	}
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	ChangeState();
	m_eCulState=m_pModel->Act();
	ChangeAttackType();
	
	m_pMainCamera->Update_GameObject(fDeltaTime);
	m_pModel->Update_GameObject(fDeltaTime);

	if (m_pAtkCollision->getActive())
	{
		m_pAtkCollision->Update_Component(fDeltaTime);
		m_pAtkCollision->Collison(COLLISIONTAG::MONSTER);
	}

	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	if (m_bHide)
	{
		//숨기
	}
	return iExit;
}

void CPlayer::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pHitCollision->getHit())
	{
		//충돌 이후 작업
		cout << "Player 충돌!" << endl;		
		CCollision* pCollider = m_pHitCollision->getCollider();//충돌한 대상 가져오기
		COLLISIONTAG eTag = pCollider->getTag();//충돌한 대상의 tag값
		switch (eTag)
		{
		case COLLISIONTAG::ETC:
			m_bHide = true;
			break;
		case COLLISIONTAG::MONSTER:
			break;
		}
		m_pHitCollision->ResetCollision();
	}
	else
	{
		if (m_bHide)
			m_bHide = false;
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
	_vec3 vLook, vRight, vPos;
	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransform->getAxis(VECAXIS::AXIS_RIGHT, vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	vPos = m_pTransform->getPos();

	
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
	if (Key_Down(VIR_NUM1))
		m_eCurType = ATTACKTYPE::SWORD;
	if (Key_Down(VIR_NUM2))
		m_eCurType = ATTACKTYPE::SHURIKEN;
	if (Key_Down(VIR_NUM3))
		m_eCurType = ATTACKTYPE::BOMB;

	if (Key_Down(VIR_LBUTTON))
		m_eCulState = STATE::ATTACK;
	if (Key_Down(VIR_RBUTTON))
		m_bDash = true;

	if(Key_Up(VIR_W)|| Key_Up(VIR_A)|| Key_Up(VIR_S)|| Key_Up(VIR_D))
		m_eCulState = STATE::IDLE;

	_vec3 vMousDir = MousePos(g_hWnd);
	if (vMousDir.x < 0.f)
		m_fAngle += -0.1f;
	else if (vMousDir.x > 0.f)
		m_fAngle += 0.1f;

	m_pTransform->setAngle(MATRIXINFO::MAT_UP, m_fAngle);
	m_pTransform->setPos(vPos);
}

void CPlayer::ChangeState()
{
	if (m_eCulState != m_ePreState)
	{
		bool bCheck = false;
		CBullet* pBullet = nullptr;
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
				switch (m_eCurType)
				{
				case ATTACKTYPE::SWORD:
					m_pAtkCollision->setActive(true);
					break;
				case ATTACKTYPE::SHURIKEN:
					pBullet =Shoot(GAMEOBJECTID::SHURIKEN, bCheck);
					if (bCheck)
						Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::SHURIKEN, pBullet);
					break;
				case ATTACKTYPE::BOMB:
					pBullet =Shoot(GAMEOBJECTID::BOMB,bCheck);
					if (bCheck)
						Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::BOMB, pBullet);
					break;
				}
			}
			break;
		case STATE::WALK:
			m_pModel->setState(m_eCulState);
		}
		m_ePreState = m_eCulState;
	}
}

void CPlayer::ChangeAttackType()
{
	if (m_eCurType != m_ePreType)
	{
		if (!m_pModel->getChange())
		{
			m_pModel->setAttackType(m_eCurType);
			m_ePreType = m_eCurType;
		}
	}
}

void CPlayer::Dash(const _float& fDeltaTime)
{
	_float fDashSpeed = 30.f;
	_vec3 vPos = m_pTransform->getPos();
	_vec3 vLook;
	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	vPos += fDashSpeed * vLook * fDeltaTime;
	m_pTransform->setPos(vPos);
	m_pTransform->Jump(fDeltaTime, 1.f, m_bDash,2);
}

CBullet* CPlayer::Shoot(GAMEOBJECTID eID, _bool& bCheck)
{
	_vec3 vLook,vPos;
	_float fAngle;
	vPos = m_pTransform->getPos();
	fAngle = m_pTransform->getAngle().y;
	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	CGameObject* pBullet = GetGameObject(LAYERID::GAME_LOGIC, eID);
	if (!pBullet)
	{
		if(eID==GAMEOBJECTID::SHURIKEN)
			pBullet = Clone_ObjProto<CShuriken>(eID);
		if (eID == GAMEOBJECTID::BOMB)
			pBullet = Clone_ObjProto<CBomb>(eID);
		bCheck = true;
	}
	else
		bCheck = false;
	static_cast<CBullet*>(pBullet)->setPos(vPos);
	static_cast<CBullet*>(pBullet)->setLook(vLook);
	static_cast<CBullet*>(pBullet)->setAngle(m_pTransform->getAngle().y);
	

	return static_cast<CBullet*>(pBullet);
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
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, pCom);
	
	m_pAtkCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pAtkCollision->setRadius(1.f);
	m_pAtkCollision->setTag(COLLISIONTAG::PLAYER);
	m_pAtkCollision->setActive(false);
	m_pAtkCollision->setTrigger(COLLISIONTRIGGER::ATTACK);
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
