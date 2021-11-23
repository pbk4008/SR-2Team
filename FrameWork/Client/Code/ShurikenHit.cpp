#include "pch.h"
#include "ShurikenHit.h"
#include "ShurikenHitAnim.h"
CShurikenEff::CShurikenEff() : m_pBuffer(nullptr), m_pAnimation(nullptr)
{
	ZeroMemory(&m_vStart, sizeof(_vec3));
}

CShurikenEff::CShurikenEff(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBuffer(nullptr), m_pAnimation(nullptr)
{
	ZeroMemory(&m_vStart, sizeof(_vec3));
}

CShurikenEff::CShurikenEff(const CShurikenEff& rhs) : CGameObject(rhs), m_pBuffer(rhs.m_pBuffer), m_pAnimation(nullptr)
, m_vStart(rhs.m_vStart)
{
	m_pBuffer->AddRef();
	
	m_pAnimation = CShurikenHitAnim::Create(m_pDevice);
	m_pAnimation->setTexture(L"ShurikenEff");
}


CShurikenEff::~CShurikenEff()
{
}

HRESULT CShurikenEff::Init_ShurikenEff()
{
	Add_Component();
	return S_OK;
}

_int CShurikenEff::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->setScale(0.3f, 0.3f, 0.3f);
	m_pTransform->setPos(m_vStart);
	setRotate();
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	m_pAnimation->Update_Component(fDeltaTime);
	if (!m_pAnimation->getPlay())
	{
		setActive(false);
		m_pAnimation->setPlay(true);
		return iExit;
	}
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CShurikenEff::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CShurikenEff::Render_GameObject()
{
	CGameObject::Render_GameObject();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pAnimation->Render_Animation();
	m_pBuffer->Render_Buffer();
}

CGameObject* CShurikenEff::Clone_GameObject()
{
	return new CShurikenEff(*this);
}

void CShurikenEff::ResetObject()
{
	m_pTransform->setPos(m_vStart);
	m_pAnimation->setPlay(true);
}

void CShurikenEff::setRotate()
{
	_matrix matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransform->setRotate(matBill);
}

CShurikenEff* CShurikenEff::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShurikenEff* pInstance = new CShurikenEff(pDevice);
	if (FAILED(pInstance->Init_ShurikenEff()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CShurikenEff::Add_Component()
{
	CGameObject::Add_Component();
	
	m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, m_pBuffer);

	return S_OK;
}

void CShurikenEff::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pAnimation);
}

void CShurikenEff::setStart(const _vec3& vStart)
{
	m_vStart = vStart;
	m_pTransform->setPos(vStart);
}
