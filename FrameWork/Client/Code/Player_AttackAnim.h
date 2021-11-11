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
	HRESULT Init_PlayerAttackAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();
	virtual void Render_Animation();
private:
	void LeftMove(const _float& fDeltaTime);
	void UpMove(const _float& fDeltaTime);
	void DownMove(const _float& fDeltaTime);
	virtual void ResetTimer() override;
private:
	void TypeBySetAnimation(const _float& fDeltaTime);
	void SwordSettingAnimation(const _float& fDeltaTime);
	void ShurikenSettingAnimation(const _float& fDeltaTime);
	void GunSettingAnimation(const _float& fDeltaTime);
public:
	static CPlayer_AttackAnim* Create(LPDIRECT3DDEVICE9 pDevice);
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