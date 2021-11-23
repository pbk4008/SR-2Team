#pragma  once
#ifndef __Boss_DeathAnim_h__
#define __Boss_DeathAnim_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CBoss_DeathAnim : public CAnimation
{
	explicit CBoss_DeathAnim();
	explicit CBoss_DeathAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_DeathAnim(const CBoss_DeathAnim& rhs);
	virtual ~CBoss_DeathAnim();

public:
	HRESULT Init_Boss_DeathAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();

public:
	static CBoss_DeathAnim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Boss_DeathAnim_h__