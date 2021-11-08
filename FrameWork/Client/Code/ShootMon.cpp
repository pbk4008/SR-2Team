#include "pch.h"
#include "ShootMon.h"
#include "Transform.h"
#include "Player.h"

CShootMon::CShootMon()
{

}

CShootMon::CShootMon(LPDIRECT3DDEVICE9 pDevice)
{

}

CShootMon::CShootMon(const CShootMon& rhs)
{

}

CShootMon::~CShootMon()
{

}

HRESULT CShootMon::Init_ShootMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

_int CShootMon::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	_vec3 m_vInfo = *m_pTransform->getAxis(VECAXIS::AXIS_POS);

	Follow(fDeltaTime);

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();

	_vec3 vDis = m_vInfo - vPos;

	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 4.0f)
	{
		Attack(fDeltaTime);
	}

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CShootMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CShootMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

CGameObject* CShootMon::Clone_GameObject()
{
	return new CShootMon(*this);
}

CShootMon* CShootMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShootMon* pInstance = new CShootMon(pDevice);

	if (FAILED(pInstance->Init_ShootMon()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CShootMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹öÆÛ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//texture
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::SHOOTMON_TEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::SHOOTMON_TEX, pComponent);

	return S_OK;
}

void CShootMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target(&playerPos, m_fSpeed, fDeltaTime);
}

void CShootMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 1.0f)
	{
		cout << "Shot!" << endl;
		m_bAttack = false;
		m_iTimer = 0.f;
	}
}

void CShootMon::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
