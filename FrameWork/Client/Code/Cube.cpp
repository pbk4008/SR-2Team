#include "pch.h"
#include "Cube.h"
#include "Quad.h"
CCube::CCube()
{
	m_CubePlane.reserve(6);
	m_CubeTexture.reserve(6);
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CCube::CCube(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice)
{
	m_CubePlane.reserve(6);
	m_CubePlane.reserve(6);
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CCube::CCube(const CCube& rhs) : CGameObject(rhs)
{
	m_CubePlane.reserve(6);
	for (_int i = 0; i < 6; i++)
	{
		CRcTex* pBuffer = CRcTex::Create(m_pDevice, i);
		m_CubePlane.emplace_back(pBuffer);
		CTexture* pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
		m_CubePlane.emplace_back(pBuffer);
		m_CubeTexture.emplace_back(pTexture);
	}
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CCube::~CCube()
{
}

HRESULT CCube::Init_Cube()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CCube::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->setScale(m_vScale);
	m_pTransform->setAngle(m_vRotate);
	m_pTransform->setPos(m_vPosition);
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CCube::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CCube::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	CGameObject::Render_GameObject();
	for (_int i = 0; i < 6; i++)
	{
		m_CubeTexture[i]->Render_Texture();
		m_CubePlane[i]->Render_Buffer();
	}
}

CGameObject* CCube::Clone_GameObject()
{
	return new CCube(*this);
}

void CCube::ResetObject()
{
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCube* pInstance = new CCube(pDevice);
	if (FAILED(pInstance->Init_Cube()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CCube::Add_Component()
{
	CGameObject::Add_Component();
	return S_OK;
}

void CCube::Free()
{
	for_each(m_CubePlane.begin(), m_CubePlane.end(), DeleteObj);
	m_CubePlane.clear();
	m_CubePlane.shrink_to_fit();
	CGameObject::Free();
}

void CCube::setTexture(const _tchar* pTextureName, const _int iIndex)
{
	m_CubeTexture[iIndex]->setTexture(GetTexture(pTextureName, TEXTURETYPE::TEX_NORMAL));
}

void CCube::LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3 vPosition)
{
	m_vScale = vScale;
	m_vRotate = vRotate;
	m_vPosition = vPosition;
	m_pTransform->setScale(m_vScale);
	m_pTransform->setAngle(m_vRotate);
	m_pTransform->setPos(m_vPosition);
}
