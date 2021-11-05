#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"
#include "Player.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_fSpeed(0.f)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_fSpeed(0.f)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_TEX)),
	m_fXPos(0.f), m_fYPos(0.f), m_fZPos(0.f), m_fSpeed(0.f)
{

}

CMeleeMon::~CMeleeMon()
{

}

HRESULT CMeleeMon::Init_MeleeMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	int iExit = 0;
		_vec3	m_vInfo = m_pTransform->getAxis(VECAXIS::AXIS_POS);

	m_fSpeed = 2.f;
	//Key_Input(fDeltaTime);

	m_pTransform->setScale(0.1f, 0.1f, 0.1f);

	///////////////////////////////////////////////////////////////////////////

	/*POINT pCursor{};
	GetCursorPos(&pCursor);
	ScreenToClient(g_hWnd, &pCursor);

	_vec3 pTargetPos;
	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pDevice->GetViewport(&ViewPort);

	pTargetPos.x = pCursor.x / (ViewPort.Width * 0.5f) - 1.f;
	pTargetPos.y = pCursor.y / -(ViewPort.Height * 0.5f) + 1.f;
	pTargetPos.z = 0.f;*/

	CPlayer* m_pPlayer = nullptr;
	_vec3 pTargetPos;

	m_pPlayer->get_c

	Chase(&pTargetPos, m_fSpeed, fDeltaTime);
	//Chase_Target(&pTargetPos, m_fSpeed, fDeltaTime);

	///////////////////////////////////////////////////////////////////////////


	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CMeleeMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMeleeMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CMeleeMon::Key_Input(const float& fDeltaTime)
{
	/*_vec3 vLook = m_pTransform->getAxis(VECAXIS::AXIS_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = m_pTransform->getAxis(VECAXIS::AXIS_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vPos = m_pTransform->getAxis(VECAXIS::AXIS_POS);

	if (GetAsyncKeyState(VK_UP) & 0x80000)
		m_pTransform->setPos(vPos.x, vPos.y + 0.1f, m_fZPos);

	if (GetAsyncKeyState(VK_DOWN) & 0x80000)
		m_pTransform->setPos(vPos.x, vPos.y - 0.1f, m_fZPos);*/

	_vec3 vLook = m_pTransform->getAxis(VECAXIS::AXIS_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight = m_pTransform->getAxis(VECAXIS::AXIS_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vPos = m_pTransform->getAxis(VECAXIS::AXIS_POS);

	if (GetAsyncKeyState(VK_UP) & 0x80000)
		vPos += vLook * m_fSpeed * fDeltaTime;
	if (GetAsyncKeyState(VK_LEFT) & 0x80000)
		vPos += vRight * -m_fSpeed * fDeltaTime;
	if (GetAsyncKeyState(VK_DOWN) & 0x80000)
		vPos += vLook * -m_fSpeed * fDeltaTime;
	if (GetAsyncKeyState(VK_RIGHT) & 0x80000)
		vPos += vRight * m_fSpeed * fDeltaTime;

	m_pTransform->setPos(vPos);
}
void CMeleeMon::Chase(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo = m_pTransform->getAxis(VECAXIS::AXIS_POS);
	_matrix m_matWorld = m_pTransform->getWorldMatrix();
	_vec3	m_vScale = m_pTransform->getScale();

	_vec3 vDir = *pTargetPos - m_vInfo;

	m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix matScale, matTrans;
	//	_matrix matScale, matRot, matTrans;


	//matRot = *ComputeLookAtTarget(pTargetPos);

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXMatrixTranslation(&matTrans, m_vInfo.x, m_vInfo.y, m_vInfo.z);

	m_matWorld = matScale, matTrans;
	//	m_matWorld = matScale, matRot, matTrans;
}
/*cout << (vLook * m_fSpeed * fDeltaTime).x << " " << (vLook * m_fSpeed * fDeltaTime).y
		<<" " << (vLook * m_fSpeed * fDeltaTime).z << endl;;*/

Engine::CGameObject* CMeleeMon::Clone_GameObject()
{
	return new CMeleeMon(*this);
}

CMeleeMon* CMeleeMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon* pInstance = new CMeleeMon(pDevice);

	if (FAILED(pInstance->Init_MeleeMon()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//texture
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_TEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::MELEEMON_TEX, pComponent);

	//camera
	pComponent = m_pCamera = Clone_ComProto<CCamera>(COMPONENTID::CAMERA);
	m_pCamera->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::CAMERA, m_pBufferCom);

	return S_OK;
}

void CMeleeMon::Free()
{
	//Safe_Release(m_pCamera);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
