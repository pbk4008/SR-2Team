#pragma once
#ifndef __PLAYER_ATTACK_H__
#define __PLAYER_ATTACK_H__
#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END
class CPlayer_AttackAnim final : public CAnimation
{
private:
	explicit CPlayer_AttackAnim();
	explicit CPlayer_AttackAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer_AttackAnim(const CPlayer_AttackAnim& rhs);
	virtual ~CPlayer_AttackAnim();
public:
	HRESULT Init_PlayerAttackAnim(CTexture* pTexture);
	virtual _int Update_Component(const _float& fDeltaTime);
	
	virtual CComponent* Clone_Component();
public:
	static CPlayer_AttackAnim* Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture);
protected:
	virtual void Free();
};
#endif