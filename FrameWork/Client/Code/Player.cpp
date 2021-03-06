#include "pch.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "Shuriken.h"
#include "Bomb.h"
#include "SphereCollision.h"
#include "Key.h"

CPlayer::CPlayer() : m_pMainCamera(nullptr), m_pModel(nullptr) , m_eCulState(STATE::MAX),
m_ePreState(STATE::MAX),m_fSpeed(0.f),m_pHitCollision(nullptr),m_pAtkCollision(nullptr)
, m_bAttack(false), m_fAngle(0.f),m_bJump(false), m_eCurType(ATTACKTYPE::SWORD),m_ePreType(ATTACKTYPE::SWORD)
, m_bHide(false), m_bDash(false), m_fDashTime(0.f), m_bDashDelay(false), m_iJumpCount(-1)
,m_iMaxHp(0), m_iCurrentHp(0),m_iShurikenCount(0),m_iBombCount(0),m_iGetKeyCount(0)
, m_eScene(SCENEID::STAGE_END)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice): CGameObject(pDevice), m_pMainCamera(nullptr), m_pModel(nullptr),
m_fSpeed(0.f),m_eCulState(STATE::MAX),m_ePreState(STATE::MAX), m_pHitCollision(nullptr), m_pAtkCollision(nullptr)
, m_bAttack(false), m_fAngle(0.f),m_bJump(false), m_eCurType(ATTACKTYPE::SWORD), m_ePreType(ATTACKTYPE::SWORD)
, m_bHide(false), m_bDash(false), m_fDashTime(0.f), m_bDashDelay(false), m_iJumpCount(-1)
, m_iMaxHp(0), m_iCurrentHp(0), m_iShurikenCount(0), m_iBombCount(0), m_iGetKeyCount(0),m_fAngleX(0.f)
,m_eScene(SCENEID::STAGE_END)
{
}

CPlayer::CPlayer(const CPlayer& rhs) : CGameObject(rhs), m_pMainCamera(rhs.m_pMainCamera), m_pModel(rhs.m_pModel),
m_fSpeed(rhs.m_fSpeed), m_eCulState(rhs.m_eCulState),m_ePreState(rhs.m_ePreState)
, m_pHitCollision(rhs.m_pHitCollision),m_pAtkCollision(rhs.m_pAtkCollision)
, m_bAttack(rhs.m_bAttack), m_fAngle(rhs.m_fAngle), m_bJump(rhs.m_bJump),
m_eCurType(rhs.m_eCurType),m_ePreType(rhs.m_ePreType), m_bHide(rhs.m_bHide), m_bDash(rhs.m_bDash)
, m_fDashTime(rhs.m_fDashTime), m_bDashDelay(rhs.m_bDashDelay), m_iJumpCount(rhs.m_iJumpCount)
, m_iMaxHp(rhs.m_iMaxHp), m_iCurrentHp(rhs.m_iCurrentHp), m_iShurikenCount(rhs.m_iShurikenCount), m_iBombCount(rhs.m_iBombCount)
, m_iGetKeyCount(rhs.m_iGetKeyCount) , m_fAngleX(rhs.m_fAngleX), m_eScene(rhs.m_eScene)
{
	m_pTransform->setPos(3.f,0.f,3.f);
	if (rhs.m_pModel)
		m_pModel->AddRef();
	if(rhs.m_pMainCamera)
		m_pMainCamera->AddRef();

	m_pHitCollision->AddRef();
	m_pHitCollision->setTransform(m_pTransform);
	m_pAtkCollision->AddRef();
	m_pAtkCollision->setTransform(m_pTransform);
	Insert_ObjCollision(m_pHitCollision);
	m_pHitCollision->setTarget(this);
	Insert_ObjCollision(m_pAtkCollision);


	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, m_pHitCollision);
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Init_Player()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 8.f;
	m_iMaxHp = 100;
	//m_iCurrentHp = m_iMaxHp;
	m_iCurrentHp = 60;
	m_iShurikenCount = 15;
	m_iBombCount = 3;

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
	if (!m_bJump)
		m_pTransform->UsingGravity(fDeltaTime);
	else
		m_pTransform->Jump(fDeltaTime, 4.f, m_bJump);
	KeyInput(fDeltaTime);
	m_pHitCollision->WallCollision(m_eScene);
	m_pTransform->setScale(0.8f, 0.5f, 0.8f);
	
	if (m_bHide)
		cout << "???? ??" << endl;
	if (m_bDash && !m_bDashDelay)
	{
		Dash(fDeltaTime);
	}
	if (m_bDashDelay)
	{
		m_fDashTime += fDeltaTime;
		if (m_fDashTime > 3.f)
		{
			m_fDashTime = 0.f;
			m_bDashDelay = false;
		}
	}
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	ChangeState();
	if (m_eCulState == STATE::HIT)
		m_eCulState = m_pMainCamera->Hit();
	else
		m_eCulState=m_pModel->Act();
	ChangeAttackType();

	m_pMainCamera->Update_GameObject(fDeltaTime);
	m_pModel->Update_GameObject(fDeltaTime);
	if (!m_bAttack)
		m_pAtkCollision->setActive(false);
	if (m_pAtkCollision->getActive())
	{
		m_pAtkCollision->Update_Component(fDeltaTime);
		m_pAtkCollision->Collison(COLLISIONTAG::MONSTER);
	}

	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	if (m_bHide)
	{
		//????
	}
	return iExit;
}

void CPlayer::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pHitCollision->getHit())
	{
		//???? ???? ????
		//cout << "Player ????!" << endl;		
		CGameObject* pCollider = m_pHitCollision->getCollider()->getTarget();
		COLLISIONTAG eTag = m_pHitCollision->getCollider()->getTag();//?????? ?????? tag??
		switch (eTag)
		{
		case COLLISIONTAG::ETC:
			if (pCollider)
			{
				if (typeid(*pCollider) == typeid(CKey))
					m_iGetKeyCount++;
			}
			m_bHide = true;
			break;
		case COLLISIONTAG::MONSTER:
			m_iCurrentHp--;
			m_eCulState = STATE::HIT;
			break;
		case COLLISIONTAG::BULLET:
			m_iCurrentHp--;
			m_eCulState = STATE::HIT;
			break;
		}
		m_pHitCollision->ResetCollision();
	}
	else
	{
		if (m_bHide)
			m_bHide = false;
	}
	if (m_pAtkCollision->getCollider())
	{
		m_pAtkCollision->ResetCollision();
		m_pAtkCollision->setActive(false);
	}
}

void CPlayer::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	if (m_pAtkCollision->getActive())
	{
		m_pAtkCollision->Render_Collision();
	}
	m_pHitCollision->Render_Collision();
	

	m_pModel->Render_GameObject();
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

	vLook.y = 0.f;
	vRight.y = 0.f;

	ZeroMemory(&m_vecWalkPower, sizeof(_vec3));

	if (Key_Pressing(VIR_W))
	{
		m_vecWalkPower += (vLook * m_fSpeed * fDeltaTime);
		vPos += vLook * m_fSpeed * fDeltaTime;

		m_eCulState = STATE::WALK;
	}
	if (Key_Pressing(VIR_A))
	{
		m_vecWalkPower += (vRight * -m_fSpeed * fDeltaTime);

		vPos += vRight * -m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (Key_Pressing(VIR_S))
	{
		m_vecWalkPower += (vLook * -m_fSpeed * fDeltaTime);

		vPos += vLook * -m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (Key_Pressing(VIR_D))
	{
		m_vecWalkPower += (vRight * m_fSpeed * fDeltaTime);

		vPos += vRight * m_fSpeed * fDeltaTime;
		m_eCulState = STATE::WALK;
	}
	if (!m_bJump)
	{
		if (Key_Down(VIR_SPACE))
		{
			if (m_iJumpCount > 0)
				m_iJumpCount--;
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
	{
		m_eCulState = STATE::ATTACK;
		if (m_bHide)
			m_bHide = false;
	}
	if (Key_Down(VIR_RBUTTON))
	{
		if(!m_bDashDelay)
			m_bDash = true;
	}

	if((Key_Up(VIR_W)|| Key_Up(VIR_A)|| Key_Up(VIR_S)|| Key_Up(VIR_D))&&m_eCulState!=STATE::HIT)
		m_eCulState = STATE::IDLE;

	_vec3 vMousDir = MousePos(g_hWnd);
	if (vMousDir.x < 0.f)
		m_fAngle += -3.f;
	else if (vMousDir.x > 0.f)
		m_fAngle += 3.f ;

	if (vMousDir.y < 0.f)
		m_fAngleX += -3.f;
	else if (vMousDir.y > 0.f)
		m_fAngleX += 3.f ;

	_vec3 rotangle = { m_fAngleX,m_fAngle, 0.f };
	m_pTransform->setAngle(rotangle);
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
					CSoundMgr::Get_Instance()->PlaySound((TCHAR*)L"Sword.mp3", CSoundMgr::PLAYER);
					break;
				case ATTACKTYPE::SHURIKEN:
					if (m_iShurikenCount > 0)
					{
						pBullet = Shoot(GAMEOBJECTID::SHURIKEN, bCheck);
						static_cast<CShuriken*>(pBullet)->setScene(m_eScene);
						if (bCheck)
							Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::SHURIKEN, pBullet);
					}
					else if (m_iShurikenCount <= 0)
					{
						m_pModel->setState(STATE::IDLE);
					}
					break;
				case ATTACKTYPE::BOMB:
					if (m_iBombCount > 0)
					{
						pBullet = Shoot(GAMEOBJECTID::BOMB, bCheck);
						if (bCheck)
							Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::BOMB, pBullet);
					}
					else if (m_iBombCount <= 0)
					{
						m_pModel->setState(STATE::IDLE);
					}
					break;
				}
			}
			break;
		case STATE::WALK:
			m_pModel->setState(m_eCulState);
			break;
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

void CPlayer::Dash(const float& fDeltaTime)
{
	m_pMainCamera->CameraZoomInAndOut(fDeltaTime);
	_vec3 vLook;
	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	if(!m_bJump)
	vLook.y = 0.f;
	TelePort(fDeltaTime, vLook, 1.f);
	if (!m_bDash)
	{
		m_bDashDelay = true;
		m_pMainCamera->CameraZoomReset();
	}
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
		if (eID == GAMEOBJECTID::SHURIKEN)
		{
			pBullet = Clone_ObjProto<CShuriken>(eID);
		}
		if (eID == GAMEOBJECTID::BOMB)
		{
			pBullet = Clone_ObjProto<CBomb>(eID);
		}
		bCheck = true;
	}
	else
		bCheck = false;
	static_cast<CBullet*>(pBullet)->setPos(vPos);
	static_cast<CBullet*>(pBullet)->setLook(vLook);
	static_cast<CBullet*>(pBullet)->setAngle(m_pTransform->getAngle().y);

	if (eID == GAMEOBJECTID::SHURIKEN)
	{ 
		CSoundMgr::Get_Instance()->PlaySound((TCHAR*)L"Shuriken.mp3",CSoundMgr::PLAYER);
		--m_iShurikenCount;
	}
	else if (eID == GAMEOBJECTID::BOMB)
	{
		CSoundMgr::Get_Instance()->PlaySound((TCHAR*)L"BombAttack.mp3", CSoundMgr::PLAYER);
		--m_iBombCount;
	}

	
	return static_cast<CBullet*>(pBullet);
}

void CPlayer::TelePort(const _float& fDeltaTime, const _vec3& vLook, const _float fPower)
{
	_float fDashSpeed = 30.f;
	_vec3 vPos = m_pTransform->getPos();
	vPos += fDashSpeed * vLook * fDeltaTime;
	m_pTransform->setPos(vPos);
	m_pTransform->Jump(fDeltaTime, fPower, m_bDash, 2);
}

HRESULT CPlayer::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	m_pHitCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pHitCollision->setRadius(1.f);
	m_pHitCollision->setTag(COLLISIONTAG::PLAYER);
	m_pHitCollision->setActive(true);
	m_pHitCollision->setTrigger(COLLISIONTRIGGER::HIT);
	m_pHitCollision->AddRef();
	pCom = m_pHitCollision;
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, pCom);
	
	m_pAtkCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pAtkCollision->setRadius(3.f);
	m_pAtkCollision->setTag(COLLISIONTAG::PLAYER);
	m_pAtkCollision->setActive(false);
	m_pAtkCollision->setPivot(1.f);
	m_pAtkCollision->setTarget(this);
	m_pAtkCollision->setTrigger(COLLISIONTRIGGER::ATTACK);
	return S_OK;
}

void CPlayer::Free()
{
	CGameObject::Free();
	Safe_Release(m_pHitCollision);
	Safe_Release(m_pAtkCollision);
	Safe_Release(m_pMainCamera);
	Safe_Release(m_pModel);
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

void CPlayer::PlusFormItem(const _int& _itemPower, const eITEM& _itemType)
{
	switch (_itemType)
	{
	case eITEM::HP20:
	case eITEM::HP50:
	case eITEM::HP100:
		m_iCurrentHp += _itemPower;
		CSoundMgr::Get_Instance()->PlaySound((TCHAR*)L"HPPostion.mp3", CSoundMgr::EFFECT);
		if (m_iCurrentHp > m_iMaxHp)
		{
			m_iCurrentHp = m_iMaxHp;
		}
		break;
	case eITEM::SHURIKEN20:
	case eITEM::SHURIKEN50:
		m_iShurikenCount += _itemPower;
		if (m_iShurikenCount > 99)
		{
			m_iShurikenCount = 99;
		}
		break;
	case eITEM::BOMB2:
	case eITEM::BOMB5:
		m_iBombCount += _itemPower;
		if (m_iBombCount > 20)
		{
			m_iBombCount = 20;
		}
		break;
	default:
		break;
	}

}
