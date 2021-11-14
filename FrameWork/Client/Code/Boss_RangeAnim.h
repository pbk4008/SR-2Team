#ifndef Boss_RangeAnim_h__
#define Boss_RangeAnim_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CBoss_RangeAnim : public CAnimation
{
	explicit CBoss_RangeAnim();
	explicit CBoss_RangeAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_RangeAnim(const CBoss_RangeAnim& rhs);
	virtual ~CBoss_RangeAnim();

public:
	HRESULT Init_ShootMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CBoss_RangeAnim* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // Boss_RangeAnim_h__
