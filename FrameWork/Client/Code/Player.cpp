#include "pch.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"

CPlayer::CPlayer() : m_pMainCamera(nullptr), m_pModel(nullptr)
,m_fSpeed(0.f)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice): CGameObject(pDevice), m_pMainCamera(nullptr), m_pModel(nullptr),
m_fSpeed(0.f)
{
}

CPlayer::CPlayer(const CPlayer& rhs) : CGameObject(rhs), m_pMainCamera(rhs.m_pMainCamera), m_pModel(rhs.m_pModel),
m_fSpeed(rhs.m_fSpeed)
{
	if (rhs.m_pModel)
		m_pModel->AddRef();
	if(rhs.m_pMainCamera)
		m_pMainCamera->AddRef();
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
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	m_pMainCamera->Update_GameObject(fDeltaTime);
	m_pModel->Update_GameObject(fDeltaTime);
	return iExit;
}

void CPlayer::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
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
	_vec3 vPos = *m_pTransform->getAxis(VECAXIS::AXIS_POS);

	if (Key_Pressing(VIR_W))
		vPos += vLook * m_fSpeed * fDeltaTime;
	if (Key_Pressing(VIR_A))
		vPos += vRight * -m_fSpeed * fDeltaTime;
	if (Key_Pressing(VIR_S))
		vPos += vLook * -m_fSpeed * fDeltaTime;
	if (Key_Pressing(VIR_D))
		vPos += vRight * m_fSpeed * fDeltaTime;
	if (Key_Down(VIR_SPACE))
		m_pModel->setAttack(true);

	m_pTransform->setPos(vPos);
}

HRESULT CPlayer::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	return S_OK;
}

void CPlayer::Free()
{
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
}
