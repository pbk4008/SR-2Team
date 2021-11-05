#include "pch.h"
#include "Player.h"
#include "MainCamera.h"

CPlayer::CPlayer() : m_pBufferCom(nullptr), m_pTexture(nullptr), m_pMainCamera(nullptr)
,m_fSpeed(0.f)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice): CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), m_pMainCamera(nullptr)
, m_fSpeed(0.f)
{
}

CPlayer::CPlayer(const CPlayer& rhs) : CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture), m_pMainCamera(rhs.m_pMainCamera)
,m_fSpeed(rhs.m_fSpeed)
{
	m_pBufferCom->AddRef();
	m_pTexture->AddRef();
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

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	if(m_bMove)
		m_pMainCamera->Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CPlayer::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();
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
	m_bMove = false;
	_vec3 vLook = m_pTransform->getAxis(VECAXIS::AXIS_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = m_pTransform->getAxis(VECAXIS::AXIS_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vPos = m_pTransform->getAxis(VECAXIS::AXIS_POS);

	if (Key_Pressing(VIR_W))
	{
		vPos += vLook * m_fSpeed * fDeltaTime;
		m_bMove = true;
	}
	if (Key_Pressing(VIR_A))
	{
		vPos += vRight * -m_fSpeed * fDeltaTime;
		m_bMove = true;
	}
	if (Key_Pressing(VIR_S))
	{
		vPos += vLook * -m_fSpeed * fDeltaTime;
		m_bMove = true;
	}
	if (Key_Pressing(VIR_D))
	{
		vPos += vRight * m_fSpeed * fDeltaTime;
		m_bMove = true;
	}
	m_pTransform->setPos(vPos);
}

HRESULT CPlayer::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pCom = nullptr;

	pCom = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);

	pCom = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::PLAYER_TEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::PLAYER_TEX, pCom);

	return S_OK;
}

void CPlayer::Free()
{
	Safe_Release(m_pMainCamera);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTexture);
	CGameObject::Free();
}

void CPlayer::setCamera(CMainCamera* pCamera)
{
	m_pMainCamera = pCamera;
	m_pMainCamera->setTarget(this);
}
