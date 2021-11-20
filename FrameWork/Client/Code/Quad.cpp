#include "pch.h"
#include "Quad.h"

CQuad::CQuad():m_pBuffer(nullptr), m_pTexture(nullptr)
{

}

CQuad::CQuad(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBuffer(nullptr), m_pTexture(nullptr)
{

}

CQuad::CQuad(const CQuad& rhs) : CGameObject(rhs), m_pBuffer(nullptr), m_pTexture(nullptr)
{
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
	m_pTransform->setScale(m_pTransform->getScale());
	m_pTransform->setAngle(m_pTransform->getAngle());
	m_pTransform->setPos(m_pTransform->getPos());
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
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
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
	m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);

	return S_OK;
}

void CQuad::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}

void CQuad::setTexture(const _tchar* pFileName)
{
	m_pTexture->setTexture(GetTexture(pFileName, TEXTURETYPE::TEX_NORMAL));
}
