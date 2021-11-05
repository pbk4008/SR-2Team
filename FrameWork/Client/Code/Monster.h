#pragma once
#ifndef Monster_h__
#define Monster_h__

#include "GameObject.h"
BEGIN(Engine)

class CMonster abstract: public CGameObject
{
protected:
	explicit CMonster();
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonster();

private:
	void	Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void	ComputeLookAtTarget(const _vec3* pTargetPos);

public:


private:

};
END

#endif // Monster_h__
