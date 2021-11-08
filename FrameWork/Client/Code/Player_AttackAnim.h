#pragma once
#ifndef __PLAYER_ATTACK_ANIM_H__
#define __PLAYER_ATTACK_ANIM_H__
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
private:
	void LeftMove(const _float& fDeltaTime);
	void UpMove(const _float& fDeltaTime);
	void DownMove(const _float& fDeltaTime);
	void SettingAnimation(const _float& fDeltaTime);
	void ResetTimer();
public:
	static CPlayer_AttackAnim* Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture);
private:
	virtual void Free();
public:
	void setTransform(CTransform* pTransform);
private:
	CTransform* m_pTransform;
	_float m_fSpeed;
	_float m_fDelayTime;
	_float m_fDirChangeTime;
};
#endif