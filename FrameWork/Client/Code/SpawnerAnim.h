#pragma once
#ifndef __SPAWNERANIM_H__
#define __SPAWNERANIM_H__
#include "Animation.h"
class CSpawnerAnim final : public CAnimation
{
private:
	explicit CSpawnerAnim();
	explicit CSpawnerAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpawnerAnim(const CSpawnerAnim& rhs);
	virtual ~CSpawnerAnim();
public:
	HRESULT Init_SpawnerAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();
public:
	static CSpawnerAnim* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void ResetTimer();
	virtual void Free();
};
#endif