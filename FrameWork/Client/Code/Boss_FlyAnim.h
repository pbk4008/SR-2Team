#pragma  once
#ifndef __Boss_FlyAnim_h__
#define __Boss_FlyAnim_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CBoss_FlyAnim : public CAnimation
{
	explicit CBoss_FlyAnim();
	explicit CBoss_FlyAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_FlyAnim(const CBoss_FlyAnim& rhs);
	virtual ~CBoss_FlyAnim();

public:
	HRESULT Init_Boss_FlyAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CBoss_FlyAnim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Boss_FlyAnim_h__