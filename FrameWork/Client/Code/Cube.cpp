#include "pch.h"
#include "Cube.h"
#include "Quad.h"
#include "BoxCollision.h"
CCube::CCube():m_pCollision(nullptr), m_CubePlane(nullptr)
{
	m_CubeTexture.reserve(6);

}

CCube::CCube(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pCollision(nullptr), m_CubePlane(nullptr)
{
	m_CubeTexture.reserve(6);
}


CCube::CCube(const CCube& rhs) : CGameObject(rhs), m_pCollision(nullptr), m_CubePlane(rhs.m_CubePlane)
{
	m_pCollision = Clone_ComProto<CBoxCollision>(COMPONENTID::BOXCOL);
	m_pCollision->setActive(true);

	m_CubePlane->AddRef();

	m_CubeTexture.reserve(6);

	for (_int i = 0; i < 6; ++i)
	{
		CTexture* pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
		m_CubeTexture.emplace_back(pTexture);
	} 
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
	m_pTransform->setScale(m_pTransform->getScale());
	m_pTransform->setAngle(m_pTransform->getAngle());
	m_pTransform->setPos(m_pTransform->getPos());
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	m_pCollision->Update_Component(fDeltaTime);
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
	for (_int i = 0; i < 6; i++)
	{
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_CubeTexture[i]->Render_Texture();
		m_CubePlane->Render_Buffer(i);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	m_pCollision->Render_Collision();
	CGameObject::Render_GameObject();
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

	//transform
	CGameObject::Add_Component();

	CComponent* pComponent = nullptr;

	pComponent = m_CubePlane = Clone_ComProto<CCubeTex>(COMPONENTID::CUBETEX);
	m_mapComponent->emplace(COMPONENTID::CUBETEX, pComponent);
	pComponent->AddRef();

	return S_OK;
}

void CCube::Free()
{
	CGameObject::Free();
	for_each(m_CubeTexture.begin(), m_CubeTexture.end(), DeleteObj);
	m_CubeTexture.clear();
	m_CubeTexture.shrink_to_fit();

	Safe_Release(m_CubePlane);
	Safe_Release(m_pCollision);
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
	m_pCollision->setTransform(m_pTransform);
	m_pCollision->setAxis(vScale,vRotate);
	Insert_Wall(m_pCollision);
}
