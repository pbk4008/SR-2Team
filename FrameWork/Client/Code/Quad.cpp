#include "pch.h"
#include "Quad.h"

CQuad::CQuad():m_pBuffer(nullptr), m_pTexture(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CQuad::CQuad(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBuffer(nullptr), m_pTexture(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CQuad::CQuad(const CQuad& rhs) : CGameObject(rhs), m_pBuffer(nullptr), m_pTexture(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
	Add_Component();
}

CQuad::~CQuad()
{
}

HRESULT CQuad::Init_Quad()
{
	return S_OK;
}

_int CQuad::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->setScale(m_vScale);
	m_pTransform->setAngle(m_vRotate);
	m_pTransform->setPos(m_vPosition);
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CQuad::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CQuad::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CGameObject::Render_GameObject();
	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CQuad::Clone_GameObject()
{
	return new CQuad(*this);
}

void CQuad::ResetObject()
{
}

CQuad* CQuad::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CQuad* pInstance = new CQuad(pDevice);
	if (FAILED(pInstance->Init_Quad()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CQuad::Add_Component()
{
	CGameObject::Add_Component();

	m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, m_pBuffer);
	
	m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, m_pTexture);

	return S_OK;
}

void CQuad::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	CGameObject::Free();
}

void CQuad::setTexture(const _tchar* pFileName)
{
	m_pTexture->setTexture(GetTexture(pFileName, TEXTURETYPE::TEX_NORMAL));
}

void CQuad::LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3 vPosition)
{
	m_vScale = vScale;
	m_vRotate = vRotate;
	m_vPosition = vPosition;
	m_pTransform->setScale(vScale);
	m_pTransform->setAngle(vRotate);
	m_pTransform->setPos(vPosition);

}
