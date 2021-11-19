#pragma  once
#ifndef __Boss_ChargeAnim_h__
#define __Boss_ChargeAnim_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CBoss_ChargeAnim : public CAnimation
{
	explicit CBoss_ChargeAnim();
	explicit CBoss_ChargeAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_ChargeAnim(const CBoss_ChargeAnim& rhs);
	virtual ~CBoss_ChargeAnim();

public:
	HRESULT Init_Boss_ChargeAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CBoss_ChargeAnim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Boss_ChargeAnim_h__