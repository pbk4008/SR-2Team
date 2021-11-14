#ifndef Boss_MeleeAnim_h__
#define Boss_MeleeAnim_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CBoss_MeleeAnim : public CAnimation
{
	explicit CBoss_MeleeAnim();
	explicit CBoss_MeleeAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_MeleeAnim(const CBoss_MeleeAnim& rhs);
	virtual ~CBoss_MeleeAnim();

public:
	HRESULT Init_MeleeMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CBoss_MeleeAnim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Boss_MeleeAnim_h__
