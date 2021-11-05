#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CTransform final : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pDevice);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();
public:
	HRESULT Init_Transform();
	_int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();
public:
	//�ش� ������ ũ�Ⱑ 0���� �ƴ��� �Ǵ��ϴ� �Լ�
	_bool IsZero(const _vec3& vVector);
private:
	//���� �ʱ�ȭ
	void ReSetVector();
public:
	static CTransform* Create();
private:
	virtual void Free();
public:
	inline const _matrix& getWorldMatrix() const { return m_matWorld; }
	inline const _vec3& getPos() const { return m_vPos; }
	inline const _vec3& getScale() const { return m_vScale; }
	inline const _vec3& getAngle() const { return m_vAngle; }
	inline const _vec3& getRevolve() const { return m_vRevolve; }
	const _vec3& getAxis(VECAXIS eAxis);
public:
	inline void setParent(CTransform& pParent) { m_pParent = &pParent;  m_pParent->AddRef(); }
	inline void setScale(const _vec3 vScale) { m_vScale = vScale; }
	void setScale(const _float& fX, const _float& fY, const _float& fZ);
	inline void setPos(const _vec3 vPos) { m_vPos = vPos; }
	void setPos(const _float& fX, const _float& fY, const _float& fZ);
	void setAngle(MATRIXINFO eInfo, _float fAngle);
	void setRevolve(MATRIXINFO eInfo, _float fAngle);
	inline void setRotate(const _matrix& matRotate) { m_matRotate = matRotate; }
private:
	//�θ� ������Ʈ�� Ʈ�������� �����Ű�� ���� Ʈ������
	//ex)�ǹ��̶�� ������Ʈ�� Ʈ�������� �ִٸ� �װ��� �̷���ִ� â��, ��, ��� ��
	//�ǹ��� �̷�� �ִ� ���� ��� ������Ʈ Ʈ�������� �ǹ��� ȸ���ϸ� ���� ȸ���� �ؾ���
	//�ǹ��� Ʈ�������� �������� ���� Ʈ������
	CTransform* m_pParent;

	_vec3 m_vScale;//ũ��
	_vec3 m_vPos;//��ġ
	_vec3 m_vAngle;//����
	_vec3 m_vRevolve;//����
	_matrix m_matRotate;//������ �� ȸ�� �� ����
	_matrix m_matWorld;//�����Ʈ����
};
END
#endif