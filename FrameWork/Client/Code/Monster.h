#pragma once
#ifndef Monster_h__
#define Monster_h__

#include "GameObject.h"

class CMonster abstract: public CGameObject
{
protected:
	explicit CMonster();
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonster();

	void	Set_Target(const _vec3* pTargetPos);
	_vec3*	Get_Target() { return pTarget; }

public:
	void		Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	_matrix*	ComputeLookAtTarget(const _vec3* pTargetPos);

private:
	_vec3*		pTarget;
	_vec3		m_vInfo;
	_matrix		m_matWorld;
	_vec3		m_vScale;
};

#endif // Monster_h__
