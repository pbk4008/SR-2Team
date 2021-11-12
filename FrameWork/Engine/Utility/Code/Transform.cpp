#include "Engine_Include.h"
#include "Transform.h"
#include "Export_Function.h"

CTransform::CTransform() : m_pParent(nullptr), m_dwFlag(0x00000000),
FLAG_SCALE(0x00000001),
FLAG_POS(0x00000002),
FLAG_ANGLE(0x00000004),
FLAG_REVOLVE(0x00000008),
FLAG_AXISROTATE(0x00000010),
FLAG_PARENT(0x00000020), m_fBottomY(0.f), m_fJumpTime(0.f), m_fGravityTime(0.f)
{
	ZeroMemory(&m_vPos, sizeof(_vec3));
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vAngle, sizeof(_vec3));
	ZeroMemory(&m_vRevolve, sizeof(_vec3));
	ZeroMemory(&m_matWorld, sizeof(_matrix));
	ZeroMemory(&m_matRotate, sizeof(_matrix));
	ZeroMemory(&m_matRotateAxis, sizeof(_matrix));
	ZeroMemory(&m_matRevolve, sizeof(_matrix));
	ZeroMemory(&m_matOldParent, sizeof(_matrix));
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pParent(nullptr), m_dwFlag(0x00000000),
FLAG_SCALE(0x00000001),
FLAG_POS(0x00000002),
FLAG_ANGLE(0x00000004),
FLAG_REVOLVE(0x00000008),
FLAG_AXISROTATE(0x00000010),
FLAG_PARENT(0x00000020), m_fBottomY(0.f), m_fJumpTime(0.f), m_fGravityTime(0.f)
{
	ZeroMemory(&m_vPos, sizeof(_vec3));
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vAngle, sizeof(_vec3));
	ZeroMemory(&m_vRevolve, sizeof(_vec3));
	ZeroMemory(&m_matWorld, sizeof(_matrix));
	ZeroMemory(&m_matRotate, sizeof(_matrix));
	ZeroMemory(&m_matRotateAxis, sizeof(_matrix));
	ZeroMemory(&m_matRevolve, sizeof(_matrix));
	ZeroMemory(&m_matOldParent, sizeof(_matrix));
}

CTransform::CTransform(const CTransform& rhs) : CComponent(rhs),m_vPos(rhs.m_vPos), m_vScale(rhs.m_vScale)
, m_vAngle(rhs.m_vAngle), m_vRevolve(rhs.m_vRevolve), m_matWorld(rhs.m_matWorld), m_matRotate(rhs.m_matRotate), m_matRotateAxis(rhs.m_matRotateAxis), m_matRevolve(rhs.m_matRevolve)
,m_pParent(rhs.m_pParent), m_dwFlag(rhs.m_dwFlag), m_matOldParent(rhs.m_matOldParent), m_fJumpTime(rhs.m_fJumpTime),
FLAG_SCALE(0x00000001),
FLAG_POS(0x00000002),
FLAG_ANGLE(0x00000004),
FLAG_REVOLVE(0x00000008),
FLAG_AXISROTATE(0x00000010),
FLAG_PARENT(0x00000020), m_fBottomY(rhs.m_fBottomY), m_fGravityTime(rhs.m_fGravityTime)
{
	if (rhs.m_pParent)
		m_pParent->AddRef();
}

CTransform::~CTransform()
{
}

HRESULT CTransform::Init_Transform()
{
	m_vScale = _vec3(1.0f, 1.0f, 1.0f);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matOldParent);
	D3DXMatrixIdentity(&m_matRotateAxis);
	D3DXMatrixIdentity(&m_matRevolve);
	m_fBottomY = 0.f;
	return S_OK;
}

_int CTransform::Update_Component(const _float& fDeltaTime)
{
	D3DXMatrixIdentity(&m_matWorld);
	//ReSetVector();

	//스케일 값 조정
	if (m_dwFlag & FLAG_SCALE)
	{
		for (_int i = 0; i < _int(MATRIXINFO::MAT_POS); i++)
			m_matWorld.m[i][i] = *(((_float*)&m_vScale) + i);
		m_dwFlag ^= FLAG_SCALE;
	}

	//회전 값 조정
	_matrix matRot;
	D3DXMatrixIdentity(&matRot);
	if (m_dwFlag & FLAG_ANGLE)
	{
		matRot =m_matRotate;
		D3DXMatrixIdentity(&m_matRotate);
		m_dwFlag ^= FLAG_ANGLE;
	}
	else if(m_dwFlag & FLAG_AXISROTATE)//임의의 축 회전 했을때
	{
		matRot = m_matRotateAxis;
		D3DXMatrixIdentity(&m_matRotateAxis);
		m_dwFlag ^= FLAG_AXISROTATE;
	}
	m_matWorld *= matRot;
	//위치 값 조정
	if (m_dwFlag & FLAG_POS)
	{
		for (_int i = 0; i < (_int)MATRIXINFO::MAT_POS; i++)
			m_matWorld.m[3][i] = *(((_float*)&m_vPos) + i);
		m_dwFlag ^= FLAG_POS;
	}
	//공전 설정
	
	if (m_dwFlag & FLAG_REVOLVE)
	{
		m_matWorld *= m_matRevolve;
		m_dwFlag ^= FLAG_REVOLVE;
	}
	//부모 오브젝트가 있으면 적용한다
	//스자이공(부모)
	ChangeParentMatrix();
	if (m_pParent)
		m_matWorld *= m_matOldParent;

	return 0;
}

CComponent* CTransform::Clone_Component()
{
	return new CTransform(*this);
}

_bool CTransform::IsZero(const _vec3& vVector)
{
	return !_bool(D3DXVec3Length(&vVector));
}

void CTransform::TerrainOverMove()
{
	//Terrain 로컬좌표로 y값 구하기
	CTerrainTex * pTerrainTex = static_cast<CTerrainTex*>(Get_Component(LAYERID::ENVIRONMENT, GAMEOBJECTID::TERRAIN, COMPONENTID::TERRAINTEX, COMPONENTTYPE::TYPE_STATIC));
	CCollisionMgr* pCollMgr = Init_CollisionMgr();

	_ulong dwCntX = pTerrainTex->getCntX();
	_ulong dwCntZ = pTerrainTex->getCntZ();
	_ulong dwInterval= pTerrainTex->getInterval();

	pCollMgr->TerrainCollision(m_vPos.x,m_fBottomY,m_vPos.z, pTerrainTex->getVtxPos(), dwCntX, dwCntZ, dwInterval);
	
	//Terrain 월드 좌표 y변환 값 구하기
	CTransform* pTransform = static_cast<CTransform*>(Get_Component(LAYERID::ENVIRONMENT, GAMEOBJECTID::TERRAIN, COMPONENTID::TRANSFORM, COMPONENTTYPE::TYPE_DYNAMIC));
	_vec3 vTerrainPos;
	pTransform->getAxis(VECAXIS::AXIS_POS, vTerrainPos);

	m_fBottomY += vTerrainPos.y;
}

void CTransform::Jump(const _float& fDeltaTime, const _float& fJumpPow,_bool& bJumpCheck)
{
	TerrainOverMove();
	m_fJumpTime += fDeltaTime;
	_float fY = fJumpPow*0.5f * m_fJumpTime - (0.5f * 9.8f * m_fJumpTime * m_fJumpTime);
	m_vPos.y += fY;

	if (m_vPos.y < m_fBottomY + 1.f)
	{
		m_fJumpTime = 0.f;
		m_vPos.y = m_fBottomY + 1.f;
		bJumpCheck = false;
	}
}

void CTransform::UsingGravity(const _float& fDeltaTime)
{
	TerrainOverMove();
	m_fGravityTime += fDeltaTime;
	_float fY = -(0.5f * 9.8f * m_fGravityTime * m_fGravityTime);
	m_vPos.y += fY;
	if (m_fBottomY + 1.f > m_vPos.y)
	{
		m_fGravityTime = 0.f;
		m_vPos.y = m_fBottomY+1.f;
	}
}

void CTransform::ReSetVector()
{
	m_vAngle = _vec3(0.f, 0.f, 0.f);
	m_vRevolve = _vec3(0.f, 0.f, 0.f);
}


void CTransform::ChangeParentMatrix()
{
	_matrix ChangedParent;
	ZeroMemory(&ChangedParent, sizeof(_matrix));
	if (D3DXMatrixIsIdentity(&m_matOldParent))
	{
		if (m_pParent)
		{
			ChangedParent = m_pParent->getWorldMatrix();
			matParentReMoveScale(ChangedParent);
			m_matOldParent = ChangedParent;
			m_dwFlag |= FLAG_PARENT;
		}
	}
	else
	{
		ChangedParent = m_pParent->getWorldMatrix();
		matParentReMoveScale(ChangedParent);
		if (m_matOldParent != ChangedParent)
		{
			m_dwFlag |= FLAG_PARENT;
			m_matOldParent = ChangedParent;
		}
	}
}

void CTransform::matParentReMoveScale(_matrix& pMatirx)
{
	for (_int i = 0; i < (_int)VECAXIS::AXIS_POS; i++)
	{
		_vec3 vAxis = _vec3(pMatirx.m[i][0], pMatirx.m[i][1], pMatirx.m[i][2]);

		D3DXVec3Normalize(&vAxis, &vAxis);
		
		pMatirx.m[i][0] = vAxis.x;
		pMatirx.m[i][1] = vAxis.y;
		pMatirx.m[i][2] = vAxis.z;
	}
}

CTransform* CTransform::Create()
{
	CTransform* pInstance = new CTransform;
	if (FAILED(pInstance->Init_Transform()))
		Safe_Release(pInstance);
	return pInstance;
}

void CTransform::Free()
{
	Safe_Release(m_pParent);
	CComponent::Free();
}

void CTransform::getAxis(VECAXIS eAxis, _vec3& pVec)
{
	pVec =_vec3(m_matWorld.m[(_ulong)eAxis][0], m_matWorld.m[(_ulong)eAxis][1], m_matWorld.m[(_ulong)eAxis][2]);
}

void CTransform::setScale(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;
	m_dwFlag |= FLAG_SCALE;
}

void CTransform::setPos(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
	m_vPos.z = fZ;
	m_dwFlag |= FLAG_POS;
}

void CTransform::setAngle(MATRIXINFO eInfo, _float fAngle)
{
	*(((_float*)&m_vAngle) + (_ulong)eInfo) = fAngle;
	switch (eInfo)
	{
		case MATRIXINFO::MAT_RIGHT:
			D3DXMatrixRotationX(&m_matRotate, (m_vAngle.x));
			break;
		case MATRIXINFO::MAT_UP:
			D3DXMatrixRotationY(&m_matRotate, (m_vAngle.y));
			break;
		case MATRIXINFO::MAT_LOOK:
			D3DXMatrixRotationZ(&m_matRotate, (m_vAngle.z));
			break;
	}
	m_dwFlag |= FLAG_ANGLE;
}

void CTransform::setAngle(const _vec3& vAngle)
{
	m_vAngle = vAngle;
	_matrix matX, matY, matZ;
	D3DXMatrixIdentity(&matX);
	D3DXMatrixIdentity(&matY);
	D3DXMatrixIdentity(&matZ);

	D3DXMatrixRotationX(&matX, (m_vAngle.x));

	D3DXMatrixRotationY(&matY, (m_vAngle.y));

	D3DXMatrixRotationZ(&matZ, (m_vAngle.z));

	m_matRotate = matZ * matX * matY;

	m_dwFlag |= FLAG_ANGLE;
}

void CTransform::setRevolve(MATRIXINFO eInfo, _float fAngle)
{
	*(((_float*)&m_vRevolve) + (_ulong)eInfo) = fAngle;
	switch (eInfo)
	{
	case MATRIXINFO::MAT_RIGHT:
		D3DXMatrixRotationX(&m_matRevolve, (m_vRevolve.x));
		break;
	case MATRIXINFO::MAT_UP:
		D3DXMatrixRotationY(&m_matRevolve, (m_vRevolve.y));
		break;
	case MATRIXINFO::MAT_LOOK:
		D3DXMatrixRotationZ(&m_matRevolve, (m_vRevolve.z));
		break;
	}
	m_dwFlag |= FLAG_REVOLVE;
}

void Engine::CTransform::setToolAngle(const _vec3& vecToolAngle)
{
	memcpy(&m_ToolvAngle, &vecToolAngle, sizeof(_vec3));
}
