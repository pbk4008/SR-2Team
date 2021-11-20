#include "pch.h"
#include "Terrain.h"
#include "NaviMesh.h"

CTerrain::CTerrain():m_pBufferCom(nullptr),m_pTexture(nullptr),m_pNaviMesh(nullptr)
{
	ZeroMemory(&m_vScale,sizeof(_vec3));
	ZeroMemory(&m_vRotate,sizeof(_vec3));
	ZeroMemory(&m_vPosition,sizeof(_vec3));
}

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), m_pNaviMesh(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
}

CTerrain::CTerrain(const CTerrain& rhs):CGameObject(rhs), m_pBufferCom(nullptr), m_pTexture(nullptr), m_pNaviMesh(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vRotate, sizeof(_vec3));
	ZeroMemory(&m_vPosition, sizeof(_vec3));
	Add_Component();
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Init_Terrain()
{
	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	m_pTransform->setScale(m_vScale);
	m_pTransform->setAngle(m_vRotate);
	m_pTransform->setPos(m_vPosition);
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CTerrain::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CTerrain::Render_GameObject()
{
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

CGameObject* CTerrain::Clone_GameObject()
{
	return new CTerrain(*this);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);
	if (FAILED(pInstance->Init_Terrain()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CTerrain::Add_Component()
{
	CComponent* pCom = nullptr;

	pCom = m_pBufferCom = Clone_ComProto<CTerrainTex>(COMPONENTID::TERRAINTEX);
	NULL_CHECK_RETURN(m_pBufferCom,E_FAIL);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TERRAINTEX, pCom);
	
	pCom = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, pCom);

	pCom = m_pNaviMesh = Clone_ComProto<CNaviMesh>(COMPONENTID::NAVIMESH);
	NULL_CHECK_RETURN(m_pNaviMesh, E_FAIL);
	m_pNaviMesh->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::NAVIMESH, pCom);
	return S_OK;
}

void CTerrain::Free()
{
	CGameObject::Free();
	Safe_Release(m_pNaviMesh);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
}

void CTerrain::setTexture(const _tchar* pFileName)
{
	m_pTexture->setTexture(GetTexture(pFileName, TEXTURETYPE::TEX_NORMAL));
}

void CTerrain::setVtxSetting(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwInterVal, const _ulong& dwDetail)
{
	m_pBufferCom->Init_BufferNoTexture(dwCntX, dwCntZ, dwInterVal, dwDetail);
}

void CTerrain::LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3 vPosition)
{
	m_vScale = vScale;
	m_vRotate = vRotate;
	m_vPosition = vPosition;
	m_pTransform->setScale(m_vScale);
	m_pTransform->setAngle(m_vRotate);
	m_pTransform->setPos(m_vPosition);
}

void CTerrain::Create_NaviMesh()
{
	m_pNaviMesh->setVtxCnt(m_pBufferCom->getVtxPos(), m_pBufferCom->getCntX(), m_pBufferCom->getCntZ());
	auto pVector = CCollisionMgr::GetInstance()->getVecWall();
	for (auto pWall : pVector)
	{ 
		_vec3 vAxis = static_cast<CBoxCollision*>(pWall)->getAxis();
		vAxis * 0.5f;
		_vec3 vPos = pWall->getCenter();
		
		vPos.y -= vAxis.y;
		if (vPos.y < 0)
			vPos.y = 0.f;
		else if (vPos.y > 0.f)
			continue;
		_vec3 vLeftBottom = _vec3(vPos.x - vAxis.x, vPos.y, vPos.z - vAxis.z);
		_vec3 vRightTop = _vec3(vPos.x + vAxis.x, vPos.y, vPos.z + vAxis.z);

		RECT rc = { _long(vLeftBottom.x),_long(vLeftBottom.z), _long(vRightTop.x), _long(vRightTop.z) };
		//m_pNaviMesh->Check_Shape(vPos,&rc);
	}
	m_pNaviMesh->Connet_NaviMesh();
}
