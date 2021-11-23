#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CTransform final : public CComponent
{
private:
	const _ulong FLAG_SCALE;
	const _ulong FLAG_POS;
	const _ulong FLAG_ANGLE;
	const _ulong FLAG_REVOLVE;
	const _ulong FLAG_AXISROTATE;
	const _ulong FLAG_PARENT;
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
	//해당 벡터의 크기가 0인지 아닌지 판단하는 함수
	_bool IsZero(const _vec3& vVector);
	void TerrainOverMove();
	void Jump(const _float& fDeltaTime,const _float& fJumpPow, _bool& bJumpCheck, _int iPlayer = 0);
	void UsingGravity(const _float& fDeltaTime);
private:
	//벡터 초기화
	void ChangeParentMatrix();
	void matParentReMoveScale(_matrix& pMatirx);
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
	void getAxis(VECAXIS eAxis, _vec3& pVec);
	_float getBottomY() { return m_fBottomY; }
	// === 툴
	inline const _vec3& getToolAngle() const { return m_ToolvAngle; }
public:
	inline void setParent(CTransform* pParent) { m_pParent = pParent;  m_pParent->AddRef(); }
	inline void setScale(const _vec3 vScale) { m_vScale = vScale;  m_dwFlag |= FLAG_SCALE; }
	void setScale(const _float& fX, const _float& fY, const _float& fZ);
	inline void setPos(const _vec3 vPos) { m_vPos = vPos; m_dwFlag |= FLAG_POS;}
	void setPos(const _float& fX, const _float& fY, const _float& fZ);
	void setAngle(MATRIXINFO eInfo, _float fAngle);
	void setAngle(const _vec3& vAngle);
	void setRevolve(MATRIXINFO eInfo, _float fAngle);
	inline void setRotate(const _matrix& matRotate) { m_matRotateAxis = matRotate; m_dwFlag |= FLAG_AXISROTATE; }//임의의 축 회전
	void setToolAngle(const _vec3& vecToolAngle);
private:
	//부모 오브젝트의 트랜스폼을 적용시키기 위한 트랜스폼
	//ex)건물이라는 오브젝트의 트랜스폼이 있다면 그것을 이루고있는 창문, 문, 계단 등
	//건물을 이루고 있는 구성 요소 오브젝트 트랜스폼은 건물이 회전하면 같이 회전을 해야함
	//건물의 트랜스폼을 가져오기 위한 트랜스폼
	CTransform* m_pParent;

	_float m_fBottomY;
	_float m_fJumpTime;
	_float m_fGravityTime;

	_vec3 m_vScale;//크기
	_vec3 m_vPos;//위치
	_vec3 m_vAngle;//각도
	_vec3 m_vRevolve;//공전
	_matrix m_matRotateAxis;//임의의 축 회전 용 변수
	_matrix m_matRevolve;
	_matrix m_matRotate;
	_matrix m_matWorld;//월드메트릭스
	_matrix m_matOldParent;//부모 이전 메트릭스
	_ulong m_dwFlag;

	_vec3 m_ToolvAngle;
};
END
#endif