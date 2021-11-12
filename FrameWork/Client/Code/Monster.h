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

public:
	void		Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void		Chase_Target_Ranged(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void		Chase_Target_Fly(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	_matrix*	ComputeLookAtTarget(const _vec3* pTargetPos);
};

#endif // Monster_h__
