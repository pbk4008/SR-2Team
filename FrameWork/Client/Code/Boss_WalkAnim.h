#pragma  once
#ifndef __Boss_WalkAnim_h__
#define __Boss_WalkAnim_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CBoss_WalkAnim : public CAnimation
{
	explicit CBoss_WalkAnim();
	explicit CBoss_WalkAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_WalkAnim(const CBoss_WalkAnim& rhs);
	virtual ~CBoss_WalkAnim();

public:
	HRESULT Init_Boss_WalkAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CBoss_WalkAnim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Boss_WalkAnim_h__