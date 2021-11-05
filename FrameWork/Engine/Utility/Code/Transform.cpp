#include "Engine_Include.h"
#include "Transform.h"

CTransform::CTransform() : m_pParent(nullptr)
{
	ZeroMemory(&m_vPos, sizeof(_vec3));
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vAngle, sizeof(_vec3));
	ZeroMemory(&m_vRevolve, sizeof(_vec3));
	ZeroMemory(&m_matWorld, sizeof(_matrix));
	ZeroMemory(&m_matRotate, sizeof(_matrix));
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pParent(nullptr)
{
	ZeroMemory(&m_vPos, sizeof(_vec3));
	ZeroMemory(&m_vScale, sizeof(_vec3));
	ZeroMemory(&m_vAngle, sizeof(_vec3));
	ZeroMemory(&m_vRevolve, sizeof(_vec3));
	ZeroMemory(&m_matWorld, sizeof(_matrix));
	ZeroMemory(&m_matRotate, sizeof(_matrix));
}

CTransform::CTransform(const CTransform& rhs) : CComponent(rhs),m_vPos(rhs.m_vPos), m_vScale(rhs.m_vScale)
, m_vAngle(rhs.m_vAngle), m_vRevolve(rhs.m_vRevolve), m_matWorld(rhs.m_matWorld), m_matRotate(rhs.m_matRotate)
,m_pParent(rhs.m_pParent)
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
	return S_OK;
}

_int CTransform::Update_Component(const _float& fDeltaTime)
{
	D3DXMatrixIdentity(&m_matWorld);
	ReSetVector();

	//������ �� ����
	for (_int i = 0; i < _int(MATRIXINFO::MAT_POS); i++)
		m_matWorld.m[i][i] = *(((_float*)&m_vScale) + i);

	//ȸ�� �� ����
	_matrix matRot;
	if (D3DXMatrixIsIdentity(&m_matRotate))//������ �� ȸ�� ��������
	{
		D3DXMatrixRotationX(&matRot, D3DXToRadian(m_vAngle.x));
		D3DXMatrixRotationY(&matRot, D3DXToRadian(m_vAngle.y));
		D3DXMatrixRotationZ(&matRot, D3DXToRadian(m_vAngle.z));
	}
	else//������ �� ȸ�� ������
	{
		matRot = m_matRotate;
		D3DXMatrixIdentity(&m_matRotate);
	}

	m_matWorld *= matRot;

	//��ġ �� ����
	for (_int i = 0; i < (_int)MATRIXINFO::MAT_POS; i++)
		m_matWorld.m[3][i] = *(((_float*)&m_vPos)+i);

	//���� ����
	_matrix matRevolve;
	D3DXMatrixRotationX(&matRevolve, D3DXToRadian(m_vRevolve.x));
	D3DXMatrixRotationY(&matRevolve, D3DXToRadian(m_vRevolve.y));
	D3DXMatrixRotationZ(&matRevolve, D3DXToRadian(m_vRevolve.z));
	m_matWorld *= matRevolve;

	//�θ� ������Ʈ�� ������ �����Ѵ�
	//�����̰�(�θ�)
	if (m_pParent)
		m_matWorld *= m_pParent->getWorldMatrix();

	MatrixToVector();
	
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

void CTransform::ReSetVector()
{
	m_vScale = _vec3(1.f, 1.f, 1.f);
	m_vAngle = _vec3(0.f, 0.f, 0.f);
	m_vRevolve = _vec3(0.f, 0.f, 0.f);
}

void CTransform::MatrixToVector()
{
	for (_int i = 0; i < _int(MATRIXINFO::MAT_POS); i++)
		*(((_float*)&m_vScale) + i) = m_matWorld.m[i][i];

	for (_int i = 0; i < (_int)MATRIXINFO::MAT_POS; i++)
		*(((_float*)&m_vPos) + i) = m_matWorld.m[3][i];
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

const _vec3& CTransform::getAxis(VECAXIS eAxis)
{
	return _vec3(m_matWorld.m[(_ulong)eAxis][0], m_matWorld.m[(_ulong)eAxis][1], m_matWorld.m[(_ulong)eAxis][2]);
	// TODO: ���⿡ return ���� �����մϴ�.
}

void CTransform::setScale(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;
}

void CTransform::setPos(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
	m_vPos.z = fZ;
}

void CTransform::setAngle(MATRIXINFO eInfo, _float fAngle)
{
	*(((_float*)&m_vAngle) + (_ulong)eInfo) = fAngle;
}

void CTransform::setRevolve(MATRIXINFO eInfo, _float fAngle)
{
	*(((_float*)&m_vRevolve) + (_ulong)eInfo) = fAngle;
}
