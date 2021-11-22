#pragma once
#ifndef Monster_h__
#define Monster_h__

#include "GameObject.h"
class CAstar;
class CMonster abstract: public CGameObject
{
protected:
	explicit CMonster();
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

protected:
	void		Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void		Chase_Target_Ranged(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void		Chase_Target_Fly(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	_matrix*	ComputeLookAtTarget(const _vec3* pTargetPos);
private:
	void MoveRoute(_vec3& vStart, const _vec3& vEnd, const _float& fDeltaTime, const _float& fSpeed);
protected:
	virtual void ResetObject();
	virtual void Free();
public:
	void setItemCheck(_bool bCheck) { m_bItemCheck = bCheck; }
protected:
	CAstar* m_pAstar;
	_vec3 m_pTargetPos;
	_bool m_bItemCheck;
};

#endif // Monster_h__
